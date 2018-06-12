#include "net/Epoll.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>

#define ARRAY_COUNT(a) (sizeof(a) / (sizeof((a)[0])))

#define EPOLL__EVENT_MASK     0xffff

#define EPOLL__SOCK_LISTED    0x1
#define EPOLL__SOCK_DELETED   0x2


typedef struct epoll_port_data_s epoll_port_data_t;
typedef struct epoll_op_s epoll_op_t;
typedef struct epoll_sock_data_s epoll_sock_data_t;

static SOCKET epoll__get_peer_socket(epoll_port_data_t* port_data,
  WSAPROTOCOL_INFOW* protocol_info);
static int epoll__submit_poll_op(epoll_port_data_t* port_data,
  epoll_sock_data_t* sock_data);


static int epoll__initialized = 0;
static PNTDEVICEIOCONTROLFILE pNtDeviceIoControlFile;

struct epoll_port_data_s {
  HANDLE iocp;
  SOCKET peer_sockets[ARRAY_COUNT(AFD_PROVIDER_IDS)];
  epoll_sock_data_t* attn_list;
  size_t pending_ops_count;
};

typedef struct epoll_sock_data_s {
  SOCKET sock;
  SOCKET base_sock;
  SOCKET peer_sock;
  uint32_t registered_events;
  uint32_t submitted_events;
  uint32_t flags;
  uint32_t op_generation;
  uint64_t user_data;
  epoll_op_t* free_op;
  epoll_sock_data_t* attn_list_prev;
  epoll_sock_data_t* attn_list_next;
};

struct epoll_op_s {
  OVERLAPPED overlapped;
  AFD_POLL_INFO poll_info;
  uint32_t generation;
  epoll_sock_data_t* sock_data;
};

epoll_t epoll_create() {
  epoll_port_data_t* port_data;
  HANDLE iocp;
  if (!epoll__initialized) {
    if (epoll__initialize() < 0)
      return NULL;
    epoll__initialized = 1;
  }

  port_data = (epoll_port_data_t *)malloc(sizeof *port_data);
  if (port_data == NULL) {
    SetLastError(ERROR_OUTOFMEMORY);
    return NULL;
  }

  iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
    NULL,
    0,
    0);
  if (iocp == INVALID_HANDLE_VALUE) {
    free(port_data);
    return NULL;
  }

  port_data->iocp = iocp;
  port_data->attn_list = NULL;
  port_data->pending_ops_count = 0;

  memset(&port_data->peer_sockets, 0, sizeof port_data->peer_sockets);

  return (epoll_t)port_data;
}


int epoll_ctl(epoll_t port_handle, int op, SOCKET sock,
  struct epoll_event* ev) {
  epoll_port_data_t* port_data;
  SOCKET base_sock;

  port_data = (epoll_port_data_t*)port_handle;

  switch (op) {
  case EPOLL_CTL_ADD: {
    epoll_sock_data_t* sock_data;
    epoll_op_t* op;
    SOCKET peer_sock;
    WSAPROTOCOL_INFOW protocol_info;
    DWORD bytes;
    int len;
    base_sock = sock;
    WSAIoctl(sock,
      SIO_BASE_HANDLE,
      NULL,
      0,
      &base_sock,
      sizeof base_sock,
      &bytes,
      NULL,
      NULL);
    len = sizeof protocol_info;
    if (getsockopt(base_sock,
      SOL_SOCKET,
      SO_PROTOCOL_INFOW,
      (char*)&protocol_info,
      &len) != 0) {
      return -1;
    }

    peer_sock = epoll__get_peer_socket(port_data, &protocol_info);
    if (peer_sock == INVALID_SOCKET) {
      return -1;
    }

    sock_data = (epoll_sock_data_t *)malloc(sizeof *sock_data);
    if (sock_data == NULL) {
      SetLastError(ERROR_OUTOFMEMORY);
      return -1;
    }

    op = (epoll_op_t *)malloc(sizeof *op);
    if (op == NULL) {
      SetLastError(ERROR_OUTOFMEMORY);
      free(sock_data);
      return -1;
    }

    sock_data->sock = sock;
    sock_data->base_sock = base_sock;
    sock_data->op_generation = 0;
    sock_data->submitted_events = 0;
    sock_data->registered_events = ev->events | EPOLLERR | EPOLLHUP;
    sock_data->user_data = ev->data.u64;
    sock_data->peer_sock = peer_sock;
    sock_data->free_op = op;
    sock_data->flags = 0;
    sock_data->attn_list_prev = NULL;
    sock_data->attn_list_next = port_data->attn_list;
    if (port_data->attn_list)
      port_data->attn_list->attn_list_prev = sock_data;
    port_data->attn_list = sock_data;
    sock_data->flags |= EPOLL__SOCK_LISTED;

    return 0;
  }

  case EPOLL_CTL_MOD: {
    return 0;
  }

  case EPOLL_CTL_DEL: {
    return 0;
  }

  default:
    WSASetLastError(WSAEINVAL);
    return -1;
  }
}


int epoll_wait(epoll_t port_handle, struct epoll_event* events, int maxevents,
  int timeout) {
  epoll_port_data_t* port_data;
  DWORD due;
  DWORD gqcs_timeout;

  port_data = (epoll_port_data_t*)port_handle;
  if (timeout > 0) {
    due = GetTickCount() + timeout;
    gqcs_timeout = (DWORD)timeout;
  }
  else if (timeout == 0) {
    gqcs_timeout = 0;
  }
  else {
    gqcs_timeout = INFINITE;
  }
  do {
    DWORD result, max_entries;
    ULONG count, i;
    OVERLAPPED_ENTRY entries[64];
    int num_events = 0;
    while (port_data->attn_list != NULL) {
      epoll_sock_data_t* sock_data = port_data->attn_list;
      assert(sock_data->flags & EPOLL__SOCK_LISTED);
      if (sock_data->registered_events & EPOLL__EVENT_MASK &
        ~sock_data->submitted_events) {
        int r = epoll__submit_poll_op(port_data, sock_data);

        if (r) {
          if (WSAGetLastError() != WSAENOTSOCK)
            return -1;
          port_data->attn_list = sock_data->attn_list_next;
          sock_data->flags &= ~EPOLL__SOCK_LISTED;
          r = epoll_ctl(port_handle, EPOLL_CTL_DEL, sock_data->sock, NULL);
          assert(r == 0);

          continue;
        }
      }
      port_data->attn_list = sock_data->attn_list_next;
      sock_data->attn_list_prev = sock_data->attn_list_next = NULL;
      sock_data->flags &= ~EPOLL__SOCK_LISTED;
    }
    max_entries = ARRAY_COUNT(entries);
    if ((int)max_entries > maxevents)
      max_entries = maxevents;

    result = GetQueuedCompletionStatusEx(port_data->iocp,
      entries,
      max_entries,
      &count,
      gqcs_timeout,
      FALSE);

    if (!result) {
      DWORD error = GetLastError();
      if (error == WAIT_TIMEOUT) {
        return 0;
      }
      else {
        return -1;
      }
    }

    port_data->pending_ops_count -= count;
    for (i = 0; i < count; i++) {
      OVERLAPPED* overlapped;
      epoll_op_t* op;
      epoll_sock_data_t* sock_data;
      DWORD afd_events;
      int registered_events, reported_events;

      overlapped = entries[i].lpOverlapped;
      op = CONTAINING_RECORD(overlapped, epoll_op_t, overlapped);
      sock_data = op->sock_data;

      if (op->generation < sock_data->op_generation) {
        free(op);
        continue;
      }
      sock_data->op_generation = 0;
      sock_data->submitted_events = 0;
      sock_data->free_op = op;

      registered_events = sock_data->registered_events;
      reported_events = 0;
      if (sock_data->flags & EPOLL__SOCK_DELETED) {
        free(op);
        free(sock_data);
        continue;
      }
      if (!NT_SUCCESS(overlapped->Internal)) {
        struct epoll_event* ev = events + (num_events++);
        ev->data.u64 = sock_data->user_data;
        ev->events = EPOLLERR;
        continue;
      }

      if (op->poll_info.NumberOfHandles == 0) {
        afd_events = 0;
      }
      else {
        afd_events = op->poll_info.Handles[0].Events;
      }
      if (afd_events & AFD_POLL_LOCAL_CLOSE) {
        free(op);
        free(sock_data);
        continue;
      }
      if (afd_events & (AFD_POLL_RECEIVE | AFD_POLL_ACCEPT))
        reported_events |= (EPOLLIN | EPOLLRDNORM);
      if (afd_events & AFD_POLL_RECEIVE_EXPEDITED)
        reported_events |= (EPOLLIN | EPOLLRDBAND);
      if (afd_events & AFD_POLL_SEND)
        reported_events |= (EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND);
      if ((afd_events & AFD_POLL_DISCONNECT) && !(afd_events & AFD_POLL_ABORT))
        reported_events |= (EPOLLRDHUP | EPOLLIN | EPOLLRDNORM | EPOLLRDBAND);
      if (afd_events & AFD_POLL_ABORT)
        reported_events |= EPOLLHUP | EPOLLERR;
      if (afd_events & AFD_POLL_CONNECT)
        reported_events |= (EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND);
      if (afd_events & AFD_POLL_CONNECT_FAIL)
        reported_events |= EPOLLERR;
      reported_events &= registered_events;
      if (!registered_events & EPOLLONESHOT || reported_events == 0) {
        assert(!(sock_data->flags & EPOLL__SOCK_LISTED));
        if (port_data->attn_list == NULL) {
          sock_data->attn_list_next = sock_data->attn_list_prev = NULL;
          port_data->attn_list = sock_data;
        }
        else {
          sock_data->attn_list_prev = NULL;
          sock_data->attn_list_next = port_data->attn_list;
          port_data->attn_list->attn_list_next = sock_data;
          port_data->attn_list = sock_data;
        }
      }

      if (reported_events) {
        struct epoll_event* ev = events + (num_events++);
        ev->data.u64 = sock_data->user_data;
        ev->events = reported_events;
      }
    }

    if (num_events > 0)
      return num_events;
    if (timeout > 0) {
      gqcs_timeout = due - GetTickCount();
    }
  } while (timeout > 0);

  return 0;
}


int epoll_close(epoll_t port_handle) {
  epoll_port_data_t* port_data;
  epoll_sock_data_t* sock_data;
  int i;

  port_data = (epoll_port_data_t*)port_handle;
  for (i = 0; i < ARRAY_COUNT(port_data->peer_sockets); i++) {
    SOCKET peer_sock = port_data->peer_sockets[i];
    if (peer_sock != 0 && peer_sock != INVALID_SOCKET) {
      if (closesocket(peer_sock) != 0)
        return -1;

      port_data->peer_sockets[i] = 0;
    }
  }
  while (port_data->pending_ops_count > 0) {
    OVERLAPPED_ENTRY entries[64];
    DWORD result;
    ULONG count, i;

    result = GetQueuedCompletionStatusEx(port_data->iocp,
      entries,
      ARRAY_COUNT(entries),
      &count,
      INFINITE,
      FALSE);

    if (!result) {
      DWORD error = GetLastError();
      return -1;
    }

    port_data->pending_ops_count -= count;

    for (i = 0; i < count; i++) {
      epoll_op_t* op = CONTAINING_RECORD(entries[i].lpOverlapped,
        epoll_op_t,
        overlapped);
      free(op);
    }
  }
  CloseHandle(port_data->iocp);
  free(port_data);

  return 0;
}


int epoll__initialize() {
  HMODULE ntdll;
  int r;
  WSADATA wsa_data;

  r = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (r != 0)
    return -1;

  ntdll = LoadLibraryW(L"ntdll.dll");
  if (ntdll == NULL)
    return -1;

  pNtDeviceIoControlFile = (PNTDEVICEIOCONTROLFILE)GetProcAddress(ntdll,
    "NtDeviceIoControlFile");
  if (pNtDeviceIoControlFile == NULL)
    return -1;

  return 0;
}

SOCKET epoll__create_peer_socket(HANDLE iocp,
  WSAPROTOCOL_INFOW* protocol_info) {
  SOCKET sock = 0;

  sock = WSASocketW(protocol_info->iAddressFamily,
    protocol_info->iSocketType,
    protocol_info->iProtocol,
    protocol_info,
    0,
    WSA_FLAG_OVERLAPPED);
  if (sock == INVALID_SOCKET) {
    return INVALID_SOCKET;
  }
}

SOCKET epoll__get_peer_socket(epoll_port_data_t* port_data,
  WSAPROTOCOL_INFOW* protocol_info) {
  int index, i;
  SOCKET peer_socket;

  index = -1;
  for (i = 0; i < ARRAY_COUNT(AFD_PROVIDER_IDS); i++) {
    if (memcmp((void*)&protocol_info->ProviderId,
      (void*)&AFD_PROVIDER_IDS[i],
      sizeof protocol_info->ProviderId) == 0) {
      index = i;
    }
  }

  if (index < 0) {
    SetLastError(ERROR_NOT_SUPPORTED);
    return INVALID_SOCKET;
  }

  peer_socket = port_data->peer_sockets[index];
  if (peer_socket == 0) {
    peer_socket = epoll__create_peer_socket(port_data->iocp, protocol_info);
    port_data->peer_sockets[index] = peer_socket;
  }

  return peer_socket;
}


int epoll__ntstatus_to_winsock_error(NTSTATUS status) {
  switch (status) {
  case STATUS_SUCCESS:
    return ERROR_SUCCESS;

  case STATUS_PENDING:
    return ERROR_IO_PENDING;

  case STATUS_INVALID_HANDLE:
  case STATUS_OBJECT_TYPE_MISMATCH:
    return WSAENOTSOCK;

  case STATUS_INSUFFICIENT_RESOURCES:
  case STATUS_PAGEFILE_QUOTA:
  case STATUS_COMMITMENT_LIMIT:
  case STATUS_WORKING_SET_QUOTA:
  case STATUS_NO_MEMORY:
  case STATUS_CONFLICTING_ADDRESSES:
  case STATUS_QUOTA_EXCEEDED:
  case STATUS_TOO_MANY_PAGING_FILES:
  case STATUS_REMOTE_RESOURCES:
  case STATUS_TOO_MANY_ADDRESSES:
    return WSAENOBUFS;

  case STATUS_SHARING_VIOLATION:
  case STATUS_ADDRESS_ALREADY_EXISTS:
    return WSAEADDRINUSE;

  case STATUS_LINK_TIMEOUT:
  case STATUS_IO_TIMEOUT:
  case STATUS_TIMEOUT:
    return WSAETIMEDOUT;

  case STATUS_GRACEFUL_DISCONNECT:
    return WSAEDISCON;

  case STATUS_REMOTE_DISCONNECT:
  case STATUS_CONNECTION_RESET:
  case STATUS_LINK_FAILED:
  case STATUS_CONNECTION_DISCONNECTED:
  case STATUS_PORT_UNREACHABLE:
  case STATUS_HOPLIMIT_EXCEEDED:
    return WSAECONNRESET;

  case STATUS_LOCAL_DISCONNECT:
  case STATUS_TRANSACTION_ABORTED:
  case STATUS_CONNECTION_ABORTED:
    return WSAECONNABORTED;

  case STATUS_BAD_NETWORK_PATH:
  case STATUS_NETWORK_UNREACHABLE:
  case STATUS_PROTOCOL_UNREACHABLE:
    return WSAENETUNREACH;

  case STATUS_HOST_UNREACHABLE:
    return WSAEHOSTUNREACH;

  case STATUS_CANCELLED:
  case STATUS_REQUEST_ABORTED:
    return WSAEINTR;

  case STATUS_BUFFER_OVERFLOW:
  case STATUS_INVALID_BUFFER_SIZE:
    return WSAEMSGSIZE;

  case STATUS_BUFFER_TOO_SMALL:
  case STATUS_ACCESS_VIOLATION:
    return WSAEFAULT;

  case STATUS_DEVICE_NOT_READY:
  case STATUS_REQUEST_NOT_ACCEPTED:
    return WSAEWOULDBLOCK;

  case STATUS_INVALID_NETWORK_RESPONSE:
  case STATUS_NETWORK_BUSY:
  case STATUS_NO_SUCH_DEVICE:
  case STATUS_NO_SUCH_FILE:
  case STATUS_OBJECT_PATH_NOT_FOUND:
  case STATUS_OBJECT_NAME_NOT_FOUND:
  case STATUS_UNEXPECTED_NETWORK_ERROR:
    return WSAENETDOWN;

  case STATUS_INVALID_CONNECTION:
    return WSAENOTCONN;

  case STATUS_REMOTE_NOT_LISTENING:
  case STATUS_CONNECTION_REFUSED:
    return WSAECONNREFUSED;

  case STATUS_PIPE_DISCONNECTED:
    return WSAESHUTDOWN;

  case STATUS_INVALID_ADDRESS:
  case STATUS_INVALID_ADDRESS_COMPONENT:
    return WSAEADDRNOTAVAIL;

  case STATUS_NOT_SUPPORTED:
  case STATUS_NOT_IMPLEMENTED:
    return WSAEOPNOTSUPP;

  case STATUS_ACCESS_DENIED:
    return WSAEACCES;

  default:
    if ((status & (FACILITY_NTWIN32 << 16)) == (FACILITY_NTWIN32 << 16) &&
      (status & (ERROR_SEVERITY_ERROR | ERROR_SEVERITY_WARNING))) {
      /* It's a windows error that has been previously mapped to an */
      /* ntstatus code. */
      return (DWORD)(status & 0xffff);
    }
    else {
      /* The default fallback for unmappable ntstatus codes. */
      return WSAEINVAL;
    }
  }
}

int epoll__afd_poll(SOCKET socket, AFD_POLL_INFO* info,
  OVERLAPPED* overlapped) {
  IO_STATUS_BLOCK iosb;
  IO_STATUS_BLOCK* iosb_ptr;
  HANDLE event = NULL;
  void* apc_context;
  NTSTATUS status;
  DWORD error;

  if (overlapped != NULL) {
    /* Overlapped operation. */
    iosb_ptr = (IO_STATUS_BLOCK*)&overlapped->Internal;
    event = overlapped->hEvent;

    /* Do not report iocp completion if hEvent is tagged. */
    if ((uintptr_t)event & 1) {
      event = (HANDLE)((uintptr_t)event & ~(uintptr_t)1);
      apc_context = NULL;
    }
    else {
      apc_context = overlapped;
    }

  }
  else {
    /* Blocking operation. */
    iosb_ptr = &iosb;
    event = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (event == NULL) {
      return SOCKET_ERROR;
    }
    apc_context = NULL;
  }

  iosb_ptr->Status = STATUS_PENDING;
  status = pNtDeviceIoControlFile((HANDLE)socket,
    event,
    NULL,
    apc_context,
    iosb_ptr,
    IOCTL_AFD_POLL,
    info,
    sizeof *info,
    info,
    sizeof *info);

  if (overlapped == NULL) {
    /* If this is a blocking operation, wait for the event to become */
    /* signaled, and then grab the real status from the io status block. */
    if (status == STATUS_PENDING) {
      DWORD r = WaitForSingleObject(event, INFINITE);

      if (r == WAIT_FAILED) {
        DWORD saved_error = GetLastError();
        CloseHandle(event);
        WSASetLastError(saved_error);
        return SOCKET_ERROR;
      }

      status = iosb.Status;
    }

    CloseHandle(event);
  }

  switch (status) {
  case STATUS_SUCCESS:
    error = ERROR_SUCCESS;
    break;

  case STATUS_PENDING:
    error = WSA_IO_PENDING;
    break;

  default:
    error = epoll__ntstatus_to_winsock_error(status);
    break;
  }

  WSASetLastError(error);

  if (error == ERROR_SUCCESS) {
    return 0;
  }
  else {
    return SOCKET_ERROR;
  }
}

int epoll__submit_poll_op(epoll_port_data_t* port_data,
  epoll_sock_data_t* sock_data) {
  epoll_op_t* op;
  int registered_events;
  DWORD result, afd_events;

  op = sock_data->free_op;
  registered_events = sock_data->registered_events;

  /* epoll_ctl should ensure that there is a free op struct. */
  assert(op != NULL);

  /* These events should always be registered. */
  assert(registered_events & EPOLLERR);
  assert(registered_events & EPOLLHUP);
  afd_events = AFD_POLL_ABORT | AFD_POLL_CONNECT_FAIL | AFD_POLL_LOCAL_CLOSE;

  if (registered_events & (EPOLLIN | EPOLLRDNORM))
    afd_events |= AFD_POLL_RECEIVE | AFD_POLL_ACCEPT;
  if (registered_events & (EPOLLIN | EPOLLRDBAND))
    afd_events |= AFD_POLL_RECEIVE_EXPEDITED;
  if (registered_events & (EPOLLOUT | EPOLLWRNORM | EPOLLRDBAND))
    afd_events |= AFD_POLL_SEND | AFD_POLL_CONNECT;

  op->generation = sock_data->op_generation + 1;
  op->sock_data = sock_data;

  memset(&op->overlapped, 0, sizeof op->overlapped);

  op->poll_info.Exclusive = TRUE;
  op->poll_info.NumberOfHandles = 1;
  op->poll_info.Timeout.QuadPart = INT64_MAX;
  op->poll_info.Handles[0].Handle = (HANDLE)sock_data->base_sock;
  op->poll_info.Handles[0].Status = 0;
  op->poll_info.Handles[0].Events = afd_events;

  result = epoll__afd_poll(sock_data->peer_sock,
    &op->poll_info,
    &op->overlapped);
  if (result != 0) {
    DWORD error = WSAGetLastError();
    if (error != WSA_IO_PENDING) {
      /* If this happens an error happened and no overlapped operation was */
      /* started. */
      return -1;
    }
  }

  sock_data->submitted_events = registered_events;
  sock_data->op_generation = op->generation;
  sock_data->free_op = NULL;
  port_data->pending_ops_count++;

  return 0;
}