#include "udpAcceptor.h"

UDPAcceptor::UDPAcceptor()
{
}

void UDPAcceptor::bind(int port, std::function<void(int, std::string)> t_on_error) {
    this->bind("0.0.0.0", port, t_on_error);
}

void UDPAcceptor::bind(std::string IPv4, std::uint16_t port, std::function<void(int, std::string)> t_on_error)
{
    if (inet_pton(AF_INET, IPv4.c_str(), &this->m_address.sin_addr) <= 0)
    {
        t_on_error(errno, "Invalid address. Address type not supported.");
        return;
    }

    this->m_address.sin_family = AF_INET;
    this->m_address.sin_port = htons(port);

    if (::bind(this->m_sock, (const sockaddr *)&this->m_address, sizeof(this->m_address)) < 0)
    {
        t_on_error(errno, "Cannot bind the socket.");
        return;
    }
}
