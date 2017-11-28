#ifndef PDU_H
#define PDU_H

#include <string>

#include "host.h"
#include <vector>

class PDU {

    public :
        typedef  std::vector<char> RAW_PDU;
        PDU(){}
        PDU(RAW_PDU raw){
            rawPDU = raw;
        }
        RAW_PDU getRawPDU(){return rawPDU;}
    private:
        RAW_PDU rawPDU;


};

#endif // PDU_H
