cmake_minimum_required(VERSION 3.2)
project(mod_php)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "-g3")

set(LIBRARY_OUTPUT_PATH "..")

include_directories(include ../shared/include ../core/include)

set(LDFLAGS)

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
        ../core/include/api/HttpResponse/HttpResponse.hh
        ../shared/include/api/Logger/FileLogger.hh
        ../shared/include/api/Logger/Logger.hh
        ../shared/include/api/Module/AModule.hpp
        ../shared/include/api/Network/INetworkStatus.hpp
        ../shared/include/api/Network/IZiaConnection.hpp
        ../shared/include/api/Enum.hh
		../shared/include/net/Buffer.hpp
        ../core/include/parser/HttpParser.hh
		../core/include/Exceptions/HttpParserException.hh
        ./include/PhpModuleException.hh
        ./include/PhpModule.hpp)

set(SOURCE_FILES
        ../shared/src/api/Logger/FileLogger.cpp
        ../shared/src/api/Logger/Logger.cpp
		./src/PhpModule.cpp
        ./src/PhpModuleException.cpp
		../core/src/api/HttpResponse/HttpResponse.cpp
		../core/src/parser/HttpParser.cpp
		../core/src/Exceptions/HttpParserException.cpp
		../shared/src/net/Buffer.cpp
		)

ADD_LIBRARY(mod_php SHARED ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(
        mod_php
        PROPERTIES
        OUTPUT_NAME "mod_php")
