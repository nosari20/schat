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

#include <future>

Channel::Channel(int pIn)
{
    portIn = pIn;
    opened = false;
}

Channel::~Channel()
{

}

int Channel::join(Host peer)
{
    if(peers.count(peer) !=0) return 0;

    int connFd;
    struct sockaddr_in	peer_addr;

    if ((connFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return(-1);
    }

    peers[peer] = connFd;

    bzero(&peer_addr, sizeof(peer_addr));

    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port   = htons(peer.getPort());

    if (inet_pton(AF_INET, peer.getIp().c_str(), &peer_addr.sin_addr) <= 0){
        return(-2);
    }

    if (connect(connFd, (struct sockaddr *) &peer_addr, sizeof(peer_addr)) < 0){
         return(-3);
    }

    return 1;
}

/*
void Channel::write(PDU pdu)//TODO
{
    PDU::RAW_PDU rpdu = pdu.getRawPDU();
    std::cout << "\x1b[33;1m" << "me"  << " : \x1b[0m";
    for(int i = 0; i < rpdu.size(); i++){
        std::cout << rpdu[i] ;
    }
    std::cout  << std::endl;
}*/
void Channel::write(PDU pdu)//TODO
{
    PDU::RAW_PDU rpdu = pdu.getRawPDU();
    char c_pdu[rpdu.size()];
    for(int i = 0; i < rpdu.size(); i++){
        c_pdu[i] = rpdu[i];
    }
    for(auto const &conn : peers) {
        ::write(conn.second, c_pdu, rpdu.size());
    }
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
    opened = true;
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

    auto future = std::async(std::launch::async, [&]() {
        while(opened){
            connFd = accept(sock, (struct sockaddr *)&client_addr, &addr_len);

            Host peer(std::string(inet_ntoa(client_addr.sin_addr)),ntohs(client_addr.sin_port));
            peers[peer] = connFd;

            std::thread listen([=]() {
                std::thread::id this_id = std::this_thread::get_id();

                char buffer[bufferSize];
                PDU::RAW_PDU rpdu;
                bzero(buffer, bufferSize);
                while(read(connFd, buffer, bufferSize) > 0){

                    bzero(buffer, bufferSize);

                    read(connFd, buffer, bufferSize);

                    for(int i = 0; i < bufferSize; i++){
                        rpdu.push_back(buffer[i]);
                         std::cerr << buffer[i] << std::endl;
                    }

                    PDU pdu(rpdu);

                    callback(peer,pdu);
                    bzero(buffer, bufferSize);
                }
                std::map<Host,int>::iterator iter = peers.find(peer) ;
                if(iter != peers.end())peers.erase( iter );

            });
            server_threads.push_back(std::move(listen));
        }

        for(int i = 0; i < server_threads.size(); i++){
            server_threads[i].join();

        }

    });
    std::cout << "listening" << std::endl;
    //server.detach();

}



void Channel::close()
{
    for(auto const &conn : peers) {
        ::close(conn.second);
    }
    opened = false;
    //server.join();
    //future.
}
