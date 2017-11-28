#ifndef HOST_H
#define HOST_H

#include <string>

class Host
{
public:
    Host();
    Host(std::string host, int port);
    std::string getIp() const;
    int getPort() const;
    std::string getHost() const;

    bool operator< (const Host& comp) const
    {
        std::string h1 = this->getIp() + std::to_string(this->getPort());
        std::string h2 = comp.getIp() + std::to_string(comp.getPort());
        return h1 < h2;
    }

private:
    std::string host;
    std::string ip;
    int port;

};

#endif // HOST_H
