#include "channel.h"

#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Channel::Channel(int pIn)
{
    portIn = pIn;
}

Channel::~Channel()
{

}

int Channel::join(Host peer)
{
    return -1;
}

void Channel::write(PDU pdu)//TODO
{
    PDU::RAW_PDU rpdu = pdu.getRawPDU();
    std::cout << "\x1b[33;1m" << "me"  << " : \x1b[0m";
    for(int i = 0; i < rpdu.size(); i++){
        std::cout << rpdu[i] ;
    }
    std::cout  << std::endl;
}

void Channel::onRcv(std::function<void(Host, PDU)> c)
{
    callback = c;
}

/*
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
*/
/* TODO */
int Channel::open(int bufferSize)
{

    if(opened == true) return -1;
    int sock;
    socklen_t addr_len;
    struct sockaddr_in server_addr, client_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);


    if(sock < 0){
        std::cerr << "Error establishing socket ..." << std::endl;
        return -1;
    }

    bzero((char*) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portIn);

    if(bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        std::cerr << "Error binding socket ..." << std::endl;
        return -2;
    }


    listen(sock, 5);

    addr_len = sizeof(client_addr);
    int connFd;
    std::vector<std::thread> server_threads;
    std::thread server([&]() {
        while(opened){
            connFd = accept(sock, (struct sockaddr *)&client_addr, &addr_len);
            std::thread listen([=]() {
                std::thread::id this_id = std::this_thread::get_id();

                char buffer[bufferSize];
                PDU::RAW_PDU rpdu;
                while(opened){

                    bzero(buffer, bufferSize);

                    read(connFd, buffer, bufferSize);

                    for(int i = 0; i < bufferSize; i++){
                        rpdu[i] = buffer[i];
                    }

                    PDU pdu(rpdu);

                    callback(Host(std::string(inet_ntoa(client_addr.sin_addr)),ntohs(client_addr.sin_port)),pdu);
                }
            });
            server_threads.push_back(std::move(listen));
        }

        for(int i = 0; i < server_threads.size(); i++){
            server_threads[i].join();
        }
    });

    server.join();
    opened = true;
}



void Channel::close()
{
    opened = false;
}
