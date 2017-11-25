#ifndef HOST_H
#define HOST_H

#include <string>

class Host
{
public:
    Host();
    Host(std::string host, int port);
    std::string getIp();
    int getPort();
    std::string getHost();

private:
    std::string host;
    std::string ip;
    int port;

};

#endif // HOST_H
