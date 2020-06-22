#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <string>
#include <functional>
#include <thread>

#include "baseSocket.h"

class TCPSocket : public BaseSocket
{
public:
    typedef std::function<void()>  ConnectionProcessor;
    typedef BaseSocket::ErrorProcessor ErrorProcessor;

    std::function<void(std::string)> onMessageReceived;
    std::function<void(const char*, int)> onRawMessageReceived;
    std::function<void()> onSocketClosed;

    explicit TCPSocket(FDR_ON_ERROR, int t_socket_id = -1);
    virtual ~TCPSocket() = default;

    int write(std::string t_message);
    int write(const char *t_bytes, size_t t_bytes_length);

    void connect(std::string t_host, uint16_t t_port, ConnectionProcessor t_on_connected = [](){}, FDR_ON_ERROR);
    void connect(uint32_t t_ipv4, uint16_t t_port, ConnectionProcessor t_on_connected = [](){}, FDR_ON_ERROR);

    void listen();

    void setAddressStruct(sockaddr_in t_addr);

private:
    static void receive(TCPSocket *t_socket);
};

#endif
