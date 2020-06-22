#include "tcpAcceptor.h"

TCPAcceptor::TCPAcceptor(std::function<void(int, std::string)> t_on_error) 
    : BaseSocket(t_on_error, TCP)
{
}

void TCPAcceptor::bind(const int t_port, std::function<void(int, std::string)> t_on_error)
{
    this->bind("0.0.0.0", t_port, t_on_error);
}

void TCPAcceptor::bind(const std::string t_address, const int t_port, std::function<void(int, std::string)> t_on_error)
{
    if (inet_pton(AF_INET, t_address.c_str(), &this->m_address.sin_addr) <= 0)
    {
        t_on_error(errno, "Invalid address. Address type not supported.");
        return;
    }

    this->m_address.sin_family = AF_INET;
    this->m_address.sin_port = htons(t_port);

    if (::bind(this->m_sock, (const sockaddr *)&this->m_address, sizeof(this->m_address)) < 0)
    {
        t_on_error(errno, "Cannot bind the socket.");
        return;
    }
}

void TCPAcceptor::listen(std::function<void(int, std::string)> t_on_error)
{
    if (::listen(this->m_sock, 10) < 0)
    {
        t_on_error(errno, "Error: Server can't listen the socket.");
        return;
    }

    std::thread acceptThread(TCPAcceptor::accept, this, t_on_error);
    acceptThread.detach();
}

void TCPAcceptor::closeRaw()
{
    shutdown(this->m_sock, SHUT_RDWR);
    
    BaseSocket::closeRaw();
}

void TCPAcceptor::accept(TCPAcceptor *t_server, std::function<void(int, std::string)> t_on_error)
{
    sockaddr_in newSocketInfo;
    socklen_t newSocketInfoLength = sizeof(newSocketInfo);

    int newSock;
    while (!t_server->m_is_closed)
    {
        while ((newSock = ::accept(t_server->m_sock, (sockaddr *)&newSocketInfo, &newSocketInfoLength)) < 0)
        {
            if (errno == EBADF || errno == EINVAL) return;

            t_on_error(errno, "Error while accepting a new connection.");
            return;
        }

        if (!t_server->m_is_closed && newSock >= 0)
        {
            TCPSocket *newSocket = new TCPSocket([](int e, std::string er) { FDR_UNUSED(e); FDR_UNUSED(er); }, newSock);
            newSocket->setAddressStruct(newSocketInfo);

            t_server->onNewConnection(newSocket);
            newSocket->listen();
        }
    }
}
