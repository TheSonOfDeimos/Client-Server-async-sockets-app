#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <string>
#include <functional>
#include <thread>

#include "baseSocket.h"

class UDPSocket : public BaseSocket
{
public:
    std::function<void(std::string, std::string, std::uint16_t)> onMessageReceived;
    std::function<void(const char*, int, std::string, std::uint16_t)> onRawMessageReceived;

    explicit UDPSocket(bool t_use_connect = false, FDR_ON_ERROR, int t_socket_id = -1);
    virtual ~UDPSocket() = default;

    void writeTo(std::string t_message, std::string t_host, uint16_t t_port, FDR_ON_ERROR);
    void writeTo(const char *t_bytes, size_t t_bytes_length, std::string t_host, uint16_t t_port, FDR_ON_ERROR);

    int write(std::string t_message);
    int write(const char *t_bytes, size_t t_bytes_length);

    void connect(std::string t_host, uint16_t t_port, FDR_ON_ERROR);
    void connect(uint32_t t_ipv4, uint16_t t_port, FDR_ON_ERROR);

private:
    static void receive(UDPSocket *t_udp_socket);
    static void receiveFrom(UDPSocket *t_udp_socket);
};

#endif
