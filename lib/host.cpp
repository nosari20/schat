#include "host.h"

#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>


Host::Host()
{
}

Host::Host(std::string h, int p)
{
    host = h;
    port = p;

    struct hostent *he;
    struct in_addr **addr_list;
    if ((he = gethostbyname(host.c_str())) == NULL){
        std::cerr << "Unknown host : " << host << std::endl;
        exit(1);
    }
    addr_list = (struct in_addr **) he->h_addr_list;

    ip = std::string(inet_ntoa(*addr_list[0]));
}
std::string Host::getIp()
{
    return ip;
}

int Host::getPort(){
    return port;
}

std::string Host::getHost(){
    return host;
}
