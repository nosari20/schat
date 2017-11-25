#include "message.h"

Message::Message(Host h, PDU p)
{
    host = h;
    pdu = p;
}

Host Message::getHost()
{
    return host;
}

Message::PDU Message::getPDU()
{
    return pdu;
}
