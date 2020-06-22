#include "tcpSocket.h"
#include <string.h>

TCPSocket::TCPSocket(std::function<void(int, std::string)> t_on_error, int t_socket_id) 
    : BaseSocket(t_on_error, TCP, t_socket_id)
{
}

int TCPSocket::write(std::string t_message)
{
    return this->write(t_message.c_str(), t_message.length());
}

int TCPSocket::write(const char *t_bytes, size_t t_bytes_length)
{
    if (this->m_is_closed)
        return -1;

    int sent = 0;
    if ((sent = send(this->m_sock, t_bytes, t_bytes_length, 0)) < 0)
    {
        perror("send");
    }
    return sent;
}

void TCPSocket::connect(std::string t_host, uint16_t t_port, std::function<void()> t_on_connected, std::function<void(int, std::string)> t_on_error)
{
    struct addrinfo hints, *res, *it;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo(t_host.c_str(), NULL, &hints, &res)) != 0) {
        t_on_error(errno, "Invalid address." + std::string(gai_strerror(status)));
        return;
    }

    for(it = res; it != NULL; it = it->ai_next)
    {
        if (it->ai_family == AF_INET) { 
            memcpy((void*)(&this->m_address), (void*)it->ai_addr, sizeof(sockaddr_in));
            break;
        }
    }

    freeaddrinfo(res);

    this->connect((uint32_t)this->m_address.sin_addr.s_addr, t_port, t_on_connected, t_on_error);
}

void TCPSocket::connect(uint32_t t_ipv4, uint16_t t_port, std::function<void()> t_on_connected, std::function<void(int, std::string)> t_on_error)
{
    this->m_address.sin_family = AF_INET;
    this->m_address.sin_port = htons(t_port);
    this->m_address.sin_addr.s_addr = t_ipv4;

    // Try to connect.
    if (::connect(this->m_sock, (const sockaddr *)&this->m_address, sizeof(sockaddr_in)) < 0)
    {
        t_on_error(errno, "Connection failed to the host.");
        return;
    }

    // Connected to the server, fire the event.
    t_on_connected();

    // Start listening from server:
    this->listen();
}

void TCPSocket::listen()
{
    // Start listening the socket from thread.
    std::thread receiveListening(receive, this);
    receiveListening.detach();
}

void TCPSocket::setAddressStruct(sockaddr_in t_addr)
{
    this->m_address = t_addr;
}

void TCPSocket::receive(TCPSocket *t_socket)
{
    char tempBuffer[BUFFER_SIZE];
    int messageLength;

    while ((messageLength = recv(t_socket->m_sock, tempBuffer, BUFFER_SIZE, 0)) > 0)
    {
        tempBuffer[messageLength] = '\0';
        if(t_socket->onMessageReceived)
            t_socket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength));
        
        if(t_socket->onRawMessageReceived)
            t_socket->onRawMessageReceived(tempBuffer, messageLength);
    }

    t_socket->closeRaw();
    if(t_socket->onSocketClosed)
        t_socket->onSocketClosed();
}
