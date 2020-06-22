#include "udpSocket.h"
#include <string.h>

UDPSocket::UDPSocket(bool useConnect, ErrorProcessor t_on_error, int t_socket_Id) 
    : BaseSocket(t_on_error, UDP, t_socket_Id)
{
    if (!useConnect)
    {
        std::thread recvfromThread(receiveFrom, this);
        recvfromThread.detach();
    }
    else
    {
        std::thread recvThread(receive, this);
        recvThread.detach();
    }
}

void UDPSocket::writeTo(std::string t_message, std::string t_host, uint16_t t_port, ErrorProcessor t_on_error)
{
    this->writeTo(t_message.c_str(), t_message.length(), t_host, t_port, t_on_error);
}

void UDPSocket::writeTo(const char *t_bytes, size_t t_bytes_length, std::string t_host, uint16_t t_port, ErrorProcessor t_on_error)
{
    sockaddr_in hostAddr;

    struct addrinfo hints, *res, *it;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    if ((status = getaddrinfo(t_host.c_str(), NULL, &hints, &res)) != 0)
    {
        t_on_error(errno, "Invalid address." + std::string(gai_strerror(status)));
        return;
    }

    for (it = res; it != NULL; it = it->ai_next)
    {
        if (it->ai_family == AF_INET)
        { // IPv4
            memcpy((void *)(&hostAddr), (void *)it->ai_addr, sizeof(sockaddr_in));
            break; // for now, just get first ip (ipv4).
        }
    }

    freeaddrinfo(res);

    hostAddr.sin_port = htons(t_port);
    hostAddr.sin_family = AF_INET;

    if (sendto(this->m_sock, t_bytes, t_bytes_length, 0, (sockaddr *)&hostAddr, sizeof(hostAddr)) < 0)
    {
        t_on_error(errno, "Cannot send message to the address.");
        return;
    }
}

int UDPSocket::write(std::string t_message)
{
    return this->write(t_message.c_str(), t_message.length());
}
int UDPSocket::write(const char *t_bytes, size_t t_bytes_length)
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

void UDPSocket::connect(std::string t_host, uint16_t t_port, ErrorProcessor t_on_error)
{
    struct addrinfo hints, *res, *it;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    if ((status = getaddrinfo(t_host.c_str(), NULL, &hints, &res)) != 0)
    {
        t_on_error(errno, "Invalid address." + std::string(gai_strerror(status)));
        return;
    }

    for (it = res; it != NULL; it = it->ai_next)
    {
        if (it->ai_family == AF_INET)
        { // IPv4
            memcpy((void *)(&this->m_address), (void *)it->ai_addr, sizeof(sockaddr_in));
            break; // for now, just get first ip (ipv4).
        }
    }

    freeaddrinfo(res);

    this->connect((uint32_t)this->m_address.sin_addr.s_addr, t_port, t_on_error);
}
void UDPSocket::connect(uint32_t ipv4, uint16_t t_port, ErrorProcessor t_on_error)
{
    this->m_address.sin_family = AF_INET;
    this->m_address.sin_port = htons(t_port);
    this->m_address.sin_addr.s_addr = ipv4;

    // Try to connect.
    if (::connect(this->m_sock, (const sockaddr *)&this->m_address, sizeof(sockaddr_in)) < 0)
    {
        t_on_error(errno, "Connection failed to the host.");
        return;
    }
}

void UDPSocket::receive(UDPSocket *udpSocket)
{
    char tempBuffer[BUFFER_SIZE];

    while (true)
    {
        int messageLength;
        messageLength = recv(udpSocket->m_sock, tempBuffer, BUFFER_SIZE, 0);

        if (messageLength < 0)
        {
            perror("recvfrom");
            return;
        }
        else
        {
            tempBuffer[messageLength] = '\0';
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength), ipToString(udpSocket->m_address), ntohs(udpSocket->m_address.sin_port));

            if (udpSocket->onRawMessageReceived)
                udpSocket->onRawMessageReceived(tempBuffer, messageLength, ipToString(udpSocket->m_address), ntohs(udpSocket->m_address.sin_port));
        }
    }
}

void UDPSocket::receiveFrom(UDPSocket *udpSocket)
{
    sockaddr_in hostAddr;
    socklen_t hostAddrSize = sizeof(hostAddr);

    char tempBuffer[BUFFER_SIZE];

    while (true)
    {
        int messageLength;
        messageLength = recvfrom(udpSocket->m_sock, tempBuffer, BUFFER_SIZE, 0, (sockaddr *)&hostAddr, &hostAddrSize);

        if (messageLength < 0)
        {
            perror("recvfrom");
            return;
        }
        else
        {
            tempBuffer[messageLength] = '\0';
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength), ipToString(hostAddr), ntohs(hostAddr.sin_port));

            if (udpSocket->onRawMessageReceived)
                udpSocket->onRawMessageReceived(tempBuffer, messageLength, ipToString(hostAddr), ntohs(hostAddr.sin_port));
        }
    }
}
