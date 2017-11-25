#include <iostream>

#include <string>
#include <string.h>

#include "lib/channel.h"

#include "include/cli/app.h"


using namespace std;


void doc(){
    cout << "Usage : schat HOST PORT [OPTIONS]" << endl << endl;
    cout << "Options : " << endl;
    cout << '\t' << "--name=NAME," << '\t' << "nickname" << endl;
}


int main(int argc, char *argv[])
{
    Cli::App app = Cli::App(argc, argv);
    if(app.hasFlag("help") || app.hasOption("help") || app.nbArgs() < 2){
        doc();
        return 0;
    }
    Host dest(app.arg(0), std::stoi(app.arg(1)));
    Host host("localhost", 0);
    cout << "\x1b[31;4mListening to " << app.arg(0) <<"(" << dest.getIp() <<")"  << " on port " << app.arg(1) << "\x1b[0m" << endl;

    Channel chan(dest,1664);
    Message::PDU::RAW_PDU rpdu = {"hello world!"};
    Message::PDU pdu(rpdu);
    chan.write(Message(host,pdu));

    chan.onRcv(
        [=](Message m) -> void
        {
            Message::PDU::RAW_PDU pdu = m.getPDU().getRawPDU();
            cout << "\x1b[32;1m" << m.getHost().getHost()  << " : \x1b[0m";
            for(int i = 0; i < pdu.size(); i++){
                std::cout << pdu[i] ;
            }
            std::cout  << std::endl;
        }
    );
    chan.open();

    char in[Message::PDU::RAW_PDU_SIZE];
    bool end = false;

    do{
       std::cin >> in;
       if(strcmp(in, "/exit") == 0){
           end =true;
       }else{

           rpdu = {};
           std::copy(std::begin(in), std::end(in), std::begin(rpdu));
           Message::PDU pdu(rpdu);
           chan.write(Message(host,pdu));
       }
    }while(!end);

    chan.close();

    return 0;
}

