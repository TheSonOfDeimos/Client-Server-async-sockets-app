cmake_minimum_required(VERSION 3.16)
project(async_sockets_cpp)

set(CMAKE_CXX_STANDARD 14)

set(SRC_DIR src/)
set(COMMON_DIR common/)
set(ASYNC_SOCKET_DIR ${COMMON_DIR}/asyncSockets/)
set(SERVER_SRC_DIR ${SRC_DIR}/server/)
set(CLIENT_SRC_DIR ${SRC_DIR}/client/)
set(CXX_HEADER_FILENAME "/*.hpp")
set(CXX_SOURCE_FILENAME "/*.cpp")

# Build sockets lib
file(GLOB ASYNC_SOCKETS_SRC 
        ${ASYNC_SOCKET_DIR}${CXX_SOURCE_FILENAME}
        ${ASYNC_SOCKET_DIR}${CXX_HEADER_FILENAME})

set(ASYNC_SOCKETS_TARG async_sockets)
add_library(${ASYNC_SOCKETS_TARG} STATIC ${ASYNC_SOCKETS_SRC})
target_include_directories(async_sockets PUBLIC ${ASYNC_SOCKET_DIR})

# Build client/server apps
file(GLOB SERVER_SRC 
        ${SERVER_SRC_DIR}${CXX_SOURCE_FILENAME}
        ${SERVER_SRC_DIR}${CXX_HEADER_FILENAME})

file(GLOB CLIENT_SRC 
        ${CLIENT_SRC_DIR}${CXX_SOURCE_FILENAME}
        ${CLIENT_SRC_DIR}${CXX_HEADER_FILENAME})

add_executable(client ${CLIENT_SRC})
add_executable(server ${SERVER_SRC})

target_link_libraries(client async_sockets)
target_link_libraries(server async_sockets)
