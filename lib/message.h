#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "host.h"
#include "array"

class Message
{
public:
    class PDU {

        public :
            static const int RAW_PDU_SIZE = 128;
            typedef  std::array<char,RAW_PDU_SIZE> RAW_PDU;
            PDU(){}
            PDU(RAW_PDU raw){
                rawPDU = raw;
            }
            RAW_PDU getRawPDU(){return rawPDU;}
        private:
            RAW_PDU rawPDU;


    };

    Message(Host host, PDU pdu);
    Host getHost();
    PDU getPDU();

private:
    Host host;
    PDU pdu;
};

#endif // MESSAGE_H
