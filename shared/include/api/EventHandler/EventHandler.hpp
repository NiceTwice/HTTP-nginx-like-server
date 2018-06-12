//
// Created by Kevin Malot on 07/01/17.
//

#ifndef ZIA_EVENTHANDLER_HH
#define ZIA_EVENTHANDLER_HH

/*!
 * \file HttpHeader.hh
 * \brief Http Object Header, contains header of a request or response
 * \author Kevin Malot
 * \version 0.1
 */

#include <map>
#include <functional>
#include <vector>
#include "api/EventList/EventList.hpp"
#include "api/HttpResponse/IHttpResponse.hh"
#include "api/HttpConf/IHttpConf.hpp"
#include "api/HttpRequest/IHttpRequest.hh"
#include "api/Network/INetworkStatus.hpp"
#include "api/Network/IZiaConnection.hpp"

/*! \namespace apouche
 *
 * namespace that contains all http component
 */
namespace apouche {
    /*! \class EventHandler
    * \brief class that represent an EventHandler
    *
    */
    class EventHandler {
    public:
        EventList<ReturnState (IHttpConf *, INetworkStatus *)> _onNetworkIO; // will handle network input/output operations
        EventList<ReturnState (IZiaConnection *, IHttpConf *)> _afterConnect;
        EventList<ReturnState (IHttpConf *)> _beforeParsingRequest;
        EventList<ReturnState (IHttpRequest *, IHttpConf *)> _afterParsingRequest;
        EventList<ReturnState (IHttpRequest *, IHttpConf *, IZiaConnection *)> _requestReceived;
        EventList<ReturnState (IHttpRequest *, IHttpConf *)> _beforeCreateResponse;
        EventList<ReturnState (IHttpRequest *, IHttpResponse *, IHttpConf *)> _afterCreateResponse;
        EventList<ReturnState (IHttpResponse *, IHttpConf *)> _beforeSendResponse;
        EventList<ReturnState (IZiaConnection*)> _afterSendResponse;

        /*
         * Clear All EventLists
         */
        void clear() {
            _onNetworkIO.clear();
            _afterConnect.clear();
            _requestReceived.clear();
            _beforeParsingRequest.clear();
            _afterParsingRequest.clear();
            _beforeCreateResponse.clear();
            _afterCreateResponse.clear();
            _beforeSendResponse.clear();
            _afterSendResponse.clear();
        }
    };
}

#endif //ZIA_EVENTHANDLER_HH
