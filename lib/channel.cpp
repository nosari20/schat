#include "channel.h"

#include <thread>

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Channel::Channel(Host dest, int p)
{
    destination = dest;
    port = p;
}

Channel::~Channel()
{

}

void Channel::write(Message m)
{
    Message::PDU::RAW_PDU pdu = m.getPDU().getRawPDU();
    std::cout << "\x1b[33;1m" << m.getHost().getHost()  << " : \x1b[0m";
    for(int i = 0; i < pdu.size(); i++){
        std::cout << pdu[i] ;
    }
    std::cout  << std::endl;
}

void Channel::onRcv(std::function<void(Message)> c)
{
    callback = c;
}

void Channel::open(int bufferSize)
{
    opened = true;
    std::thread listen([=]() {
            Message::PDU::RAW_PDU rpdu;
            for(int i = 0; i < Message::PDU::RAW_PDU_SIZE; i++){
                rpdu[i] = (char)(i+51);
            }
            Message::PDU pdu(rpdu);
            while(opened){
                callback(Message(destination,pdu));
                usleep(5000000);
            }
    });
    listen.join();
}
/* TODO
void Channel::open(int bufferSize)
{
     int host;
     Message::PDU::RAW_PDU buffer[Message::PDU::RAW_PDU_SIZE];
     struct sockaddr_in dest_addr;
     socklen_t size;
     host = socket(AF_INET, SOCK_STREAM, 0);

     if (host < 0) {
         std::cerr << "Error establishing socket ..." << std::endl;
         return;
     }

     dest_addr.sin_family = AF_INET;
     dest_addr.sin_port = htons(port);
     inet_pton(AF_INET, destination.getIp().c_str(), &dest_addr.sin_addr);

     if(connect(host, (struct sockaddr *) &dest_addr, sizeof(dest_addr))< 0){
        std::cerr << "- Connection to the server port number: " << portNum << endl;
     }

     recv(host, buffer, bufsize, 0);
     std::cout << buffer << std::endl;


    close(host);


}
*/


void Channel::close()
{
    opened = false;
}
