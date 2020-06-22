#include "udp-client.hpp"

UDPClient::UDPClient(const std::string t_host, int t_port)
    : m_socket(true)
{
    m_socket.onMessageReceived = std::bind(&UDPClient::newMessageHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_socket.connect(t_host, t_port, std::bind(&UDPClient::errorHandler, this, std::placeholders::_1, std::placeholders::_2));
}

UDPClient::~UDPClient()
{
    m_socket.closeRaw();
}

void UDPClient::write(std::string t_message)
{
    m_socket.write(t_message);
}

void UDPClient::newMessageHandler(std::string t_message, std::string t_host, uint16_t t_port)
{
    std::cout << "[INFO] New message from [" << t_host << ":" << t_port << "] => " << t_message << "\n";
}

void UDPClient::errorHandler(int t_error_code, std::string t_error_message) const
{
    std::cout << "[ERROR] " << t_error_code << " : " << t_error_message << "\n";
}