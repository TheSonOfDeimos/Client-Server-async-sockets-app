#ifndef UDP_ACCEPTOR_H
#define UDP_ACCEPTOR_H

#include <string>

#include "udpSocket.h"

class UDPAcceptor : public UDPSocket
{
public:
    UDPAcceptor();

    void bind(int t_port, FDR_ON_ERROR);
    void bind(std::string t_ipv4, std::uint16_t t_port, FDR_ON_ERROR);
};

#endif
