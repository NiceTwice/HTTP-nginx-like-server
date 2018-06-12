cmake_minimum_required(VERSION 3.2)
project(core)

find_package(OpenSSL REQUIRED ssl crypto)
set(OPENSSL_USE_STATIC_LIBS TRUE)

set(CMAKE_CXX_STANDARD 11)
if (UNIX)
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
endif (UNIX)

set(EXECUTABLE_OUTPUT_PATH "..")

include_directories(include ../shared/include ${OPENSSL_INCLUDE_DIR})

set(HEADER_FILES
        ../shared/include/api/Event/Event.hpp
        ../shared/include/api/EventHandler/EventHandler.hpp
        ../shared/include/api/EventList/EventList.hpp
        ../shared/include/api/HttpBody/IHttpBody.hh
        ../shared/include/api/HttpConf/IHttpConf.hpp
        ../shared/include/api/HttpHeader/IHttpHeader.hh
        ../shared/include/api/HttpMessage/IHttpMessage.hh
        ../shared/include/api/HttpRequest/IHttpRequest.hh
        ../shared/include/api/HttpResponse/IHttpResponse.hh
        ../shared/include/api/Logger/FileLogger.hh
        ../shared/include/api/Logger/Logger.hh
        ../shared/include/api/Module/AModule.hpp
        ../shared/include/api/Network/INetworkStatus.hpp
        ../shared/include/api/Network/IZiaConnection.hpp
        ../shared/include/api/Enum.hh
        include/api/HttpBody/HttpBody.hh
        include/api/HttpHeader/HttpHeader.hh
        include/api/HttpMessage/HttpMessage.hh
        include/api/HttpConf/HttpConf.hpp
        include/api/HttpRequest/HttpRequest.hh
        include/api/HttpResponse/HttpResponse.hh
        include/api/Network/ZiaConnection.hpp
        ../shared/include/net/TcpSocket.hpp
        include/net/TcpListener.hpp
        ../shared/include/net/Buffer.hpp
        include/net/IListener.hpp
        ../shared/include/net/ISocket.hpp
        ../shared/include/net/Blockable.hpp
        include/thread/IThread.hpp
        include/thread/Threaded.hpp
        include/Worker.hpp
        include/Core.hpp
        include/Exceptions/HttpParserException.hh
        include/net/Poller.hpp
        include/net/NetClient.hpp
        include/net/IPoller.hpp
        include/loader/ModuleFactory.hpp
        include/parser/HttpParser.hh
        include/json/Parser.hpp
        include/json/JSONObject.hpp
        include/json/JSONArray.hpp
        include/json/JSONException.hpp
        include/json/JSONParser.hpp
        include/api/Network/NetworkStatus.hpp ../shared/include/utils/ParserUtils.hpp include/loader/Pipeline.hpp include/parser/ZiaMD5.hpp include/thread/Clock.hpp)

set(SOURCE_FILES
        ../shared/src/api/Logger/FileLogger.cpp
        ../shared/src/api/Logger/Logger.cpp
        src/api/HttpBody/HttpBody.cpp
        src/api/HttpHeader/HttpHeader.cpp
        src/api/HttpMessage/HttpMessage.cpp
        src/api/HttpConf/HttpConf.cpp
        src/api/HttpRequest/HttpRequest.cpp
        src/api/HttpResponse/HttpResponse.cpp
        src/Main.cpp
        ../shared/src/net/TcpSocket.cpp
        src/net/TcpListener.cpp
        ../shared/src/net/Buffer.cpp
        ../shared/src/net/Blockable.cpp
        src/thread/Threaded.cpp
        src/Worker.cpp
        src/Core.cpp
        src/Exceptions/HttpParserException.cpp
        src/net/NetClient.cpp
        src/net/Poller.cpp
        src/loader/ModuleFactory.cpp
        src/parser/HttpParser.cpp
        src/json/Parser.cpp
        src/json/JSONObject.cpp
        src/json/JSONArray.cpp
        src/json/JSONParser.cpp
        src/api/Network/ZiaConnection.cpp
        src/api/Network/NetworkStatus.cpp
        ../shared/src/utils/ParserUtils.cpp src/loader/Pipeline.cpp src/parser/ZiaMD5.cpp src/thread/Clock.cpp)

if (UNIX)
    set(HEADER_FILES ${HEADER_FILES})
    set(SOURCE_FILES ${SOURCE_FILES})
    set(LDFLAGS ${LDFLAGS} "-ldl")
    set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -Wextra -O2 -fomit-frame-pointer \
    -ffast-math -malign-double -msseregparm -msse3 -minline-all-stringops \
    -fgcse-lm -fgcse-sm -fforce-addr -pedantic-errors")
    set(CMAKE_CXX_FLAGS_DEBUG "-W -Wall -Wextra -O2 -fomit-frame-pointer \
    -ffast-math -malign-double -msseregparm -msse3 -minline-all-stringops \
    -fgcse-lm -fgcse-sm -fforce-addr -pedantic-errors -g3")
endif (UNIX)

if (WIN32)
    set(HEADER_FILES
	${HEADER_FILES}
	include/net/Epoll.hpp
	include/net/WindowsNT.hpp)
    set(SOURCE_FILES
	${SOURCE_FILES}
	src/net/Epoll.cpp
	)
endif (WIN32)

add_executable(zia ${SOURCE_FILES} ${HEADER_FILES})
if (UNIX)
set_target_properties(
	zia
	PROPERTIES 
		OUTPUT_NAME "zia"
		SUFFIX ".x86_64"
)
endif (UNIX)
find_package(Threads)
target_link_libraries(zia ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${LDFLAGS})

