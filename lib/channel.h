#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include <functional>
#include <vector>

#include "host.h"
#include "pdu.h"

class Channel
{
public:
    Channel(int portIn);
    ~Channel();
    int join(Host peer);
    void write(PDU data);
    void onRcv(std::function<void(Host, PDU)> callback);
    int open(int bufferSize = 128);
    void close();

private:
    std::vector<Host> peers;
    int portIn;
    bool opened;
    std::function<void(Host, PDU)> callback;

    
};

#endif // CHANNEL_H
