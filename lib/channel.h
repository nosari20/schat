#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include <functional>

#include "host.h"
#include "message.h"

class Channel
{
public:
    Channel(Host dest, int port);
    ~Channel();
    void write(Message data);
    void onRcv(std::function<void(Message)> callback);
    void open(int bufferSize = 128);
    void close();

private:
    Host destination;
    int port;
    bool opened;
    std::function<void(Message)> callback;

    
};

#endif // CHANNEL_H
