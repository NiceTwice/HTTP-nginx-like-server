cmake_minimum_required(VERSION 3.2)
project(mod_ssl)

find_package(OpenSSL REQUIRED ssl crypto)
set(OPENSSL_USE_STATIC_LIBS TRUE)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "-g3")

set(LIBRARY_OUTPUT_PATH "..")

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
        ../shared/include/utils/ParserUtils.hpp include/SSLModule.hpp include/SSLSocket.hpp
		../shared/include/net/TcpSocket.hpp
		../shared/include/net/Blockable.hpp
		../shared/include/net/Buffer.hpp)

set(SOURCE_FILES
        ../shared/src/api/Logger/FileLogger.cpp
        ../shared/src/api/Logger/Logger.cpp
        ../shared/src/utils/ParserUtils.cpp src/SSLModule.cpp src/SSLSocket.cpp
		../shared/src/net/TcpSocket.cpp
		../shared/src/net/Blockable.cpp
		../shared/src/net/Buffer.cpp)

ADD_LIBRARY(mod_ssl SHARED ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(
        mod_ssl
        PROPERTIES
        OUTPUT_NAME "mod_ssl")
target_link_libraries(mod_ssl ${OPENSSL_LIBRARIES})
