#ifndef PDU_H
#define PDU_H

#include <string>

#include "host.h"
#include "array"

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

#endif // PDU_H
