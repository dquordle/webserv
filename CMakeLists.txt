cmake_minimum_required(VERSION 3.17.5)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv
        main.cpp
        Server.class/Server.cpp
        Server.class/UtilsServer/StructManager.cpp
        Server.class/UtilsServer/PollStruct.cpp
        Server.class/ServerException/ServerException.cpp
        Debug.class/Debug.cpp
        IHTTPMessage.interface/Request.class/Request.cpp
        IHTTPMessage.interface/Response.class/Response.cpp
        IHTTPMessage.interface/Methods.cpp
        Config_parser/Route.cpp
        Config_parser/Host.cpp
        Config_parser/Parser.cpp
        Errors/Errors.cpp)
