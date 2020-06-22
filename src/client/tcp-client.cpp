#include "tcp-client.hpp"

TCPClient::TCPClient(const std::string t_host, int t_port)
    : m_socket()
{
    m_socket.onMessageReceived = std::bind(&TCPClient::newMessageHandler, this, std::placeholders::_1);
    m_socket.connect(t_host, t_port, []{}, std::bind(&TCPClient::errorHandler, this, std::placeholders::_1, std::placeholders::_2));
}

TCPClient::~TCPClient()
{
    m_socket.closeRaw();
}

void TCPClient::write(std::string t_message)
{
    m_socket.write(t_message);
}

void TCPClient::newMessageHandler(std::string t_message)
{
    std::cout << "[INFO] New message from [" << m_socket.remoteAddress() << ":" << m_socket.remotePort() << "] => " << t_message << "\n";
}

void TCPClient::errorHandler(int t_error_code, std::string t_error_message) const
{
    std::cout << "[ERROR] " << t_error_code << " : " << t_error_message << "\n";
}