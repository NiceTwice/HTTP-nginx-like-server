cmake_minimum_required(VERSION 3.2)
project(mod_fs)

find_package(OpenSSL REQUIRED ssl crypto)
set(OPENSSL_USE_STATIC_LIBS TRUE)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "-g3")

set(LIBRARY_OUTPUT_PATH "..")

include_directories(include ../shared/include ../core/include ${OPENSSL_INCLUDE_DIR})

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
        ../shared/include/api/Logger/FileLogger.hh
        ../shared/include/api/Logger/Logger.hh
        ../shared/include/api/Module/AModule.hpp
        ../shared/include/api/Network/INetworkStatus.hpp
        ../shared/include/api/Network/IZiaConnection.hpp
        ../shared/include/api/Enum.hh
        ./include/FsModule.hpp
        ./include/MimeType.hpp)

set(SOURCE_FILES
        ../shared/src/api/Logger/FileLogger.cpp
        ../shared/src/api/Logger/Logger.cpp
        ./src/FsModule.cpp
        ./src/MimeType.cpp)

ADD_LIBRARY(mod_fs SHARED ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(
        mod_fs
        PROPERTIES
        OUTPUT_NAME "mod_fs")
target_link_libraries(mod_fs ${OPENSSL_LIBRARIES})