#ifndef TCP_ACCEPTOR_H
#define TCP_ACCEPTOR_H

#include <string>
#include <functional>
#include <thread>

#include "tcpSocket.h"

class TCPAcceptor : public BaseSocket
{
public:
    // Event Listeners:
    std::function<void(TCPSocket *)> onNewConnection = [](TCPSocket* t_sock){FDR_UNUSED(t_sock)};

    explicit TCPAcceptor(FDR_ON_ERROR);

    // Binding the server.
    void bind(const int t_port, FDR_ON_ERROR);
    void bind(const std::string t_address, const int t_port, FDR_ON_ERROR);

    // Start listening the server.
    void listen(FDR_ON_ERROR);

    // Overriding Close to add shutdown():
    void closeRaw() override;

private:
    static void accept(TCPAcceptor *t_server, FDR_ON_ERROR);
};

#endif
