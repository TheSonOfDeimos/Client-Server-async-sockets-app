#include "baseSocket.h"

#include <cerrno>

std::string BaseSocket::ipToString(sockaddr_in addr)
{
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

    return std::string(ip);
}

BaseSocket::BaseSocket(std::function<void(int, std::string)> onError, SocketType t_sock_type, int t_socket_id) 
    : m_sock(t_socket_id)
{
    if (t_socket_id < 0)
    {
        if ((this->m_sock = socket(AF_INET, t_sock_type, 0)) < 0)
        {
            onError(errno, "Socket creating error.");
        }
    }
}
void BaseSocket::closeRaw()
{
    if(m_is_closed) return;

    m_is_closed = true;
    close(this->m_sock);    
}

std::string BaseSocket::remoteAddress()
{
    return ipToString(this->m_address);
}

int BaseSocket::remotePort()
{
    return ntohs(this->m_address.sin_port);
}
