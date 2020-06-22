#ifndef BASESOCKET_H
#define BASESOCKET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include <string>
#include <functional>

constexpr uint16_t BUFFER_SIZE = 0xFFFF;

#define FDR_UNUSED(expr){ (void)(expr); } 
#define FDR_ON_ERROR std::function<void(int, std::string)> onError = [](int errorCode, std::string errorMessage){FDR_UNUSED(errorCode); FDR_UNUSED(errorMessage)}

class BaseSocket
{
public:
    enum SocketType
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };

    sockaddr_in m_address;
    bool m_is_closed = false;

    explicit BaseSocket(FDR_ON_ERROR, SocketType t_sock_type = TCP, int t_socket_id = -1);
    BaseSocket(const BaseSocket& t_other) = delete;
    BaseSocket& operator = (BaseSocket t_other) = delete;
    BaseSocket& operator = (const BaseSocket& t_other) = delete;
    virtual ~BaseSocket() = default;

    virtual void closeRaw();

    std::string remoteAddress();
    int remotePort();

protected:
    int m_sock = 0;
    static std::string ipToString(sockaddr_in t_addr);
};

#endif
