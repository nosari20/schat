#include <iostream>

#include <string>
#include <string.h>
#include <regex>

#include "lib/channel.h"

#include "include/cli/app.h"




using namespace std;


void doc(){
    cout << "Usage : schat [OPTIONS]" << endl << endl;
    cout << "Options : " << endl;
    cout << '\t' << "--port=PORT," << '\t' << "port (default 66666)" << endl;

}




int main(int argc, char *argv[])
{
    Cli::App app = Cli::App(argc, argv);
    if(app.hasFlag("help") || app.hasOption("help")){
        doc();
        return 0;
    }

    int port = 66666;
    if(app.hasOption("port")){
        if(app.option("port").compare("") != 0 ){
            port = atoi(app.option("port").c_str());
        }
    }

    cout << "\x1b[31;4mListening on port " << port << "\x1b[0m" << endl;

    Channel chan(port);

    chan.onRcv(
        [=](Host peer, PDU pdu) -> void
        {
            PDU::RAW_PDU rpdu = pdu.getRawPDU();
            cout << "\x1b[32;1m" << peer.getHost()  << " : \x1b[0m";
            for(int i = 0; i < rpdu.size(); i++){
                std::cout << rpdu[i] ;
            }
            std::cout  << std::endl;
        }
    );
   chan.open();

    std::string in;
    bool end = false;
    int error;
    do{
       getline (cin, in);
       cout << in << endl;
       //if(in.compare(std::string("")) == 0){continue;}


       if(in[0] == '/'){
           if(in.compare(std::string("/exit")) == 0){end =true; continue;}

            std::string prefix("/join ");
            std::string command = std::string(in);
            cout << command << endl;
            if (!command.compare(0, prefix.size(), prefix)){
                cout << "join" << endl;
                std::string argsString = command.substr(prefix.size());
                std::string cargs[2];
                int i = 0;
                std::stringstream ssin(argsString);
                while (ssin.good() && i < 2){
                    ssin >> cargs[i];
                    ++i;
                }
                cout << cargs[i] << endl;
                if((error=chan.join(Host(cargs[0],atoi(cargs[1].c_str())))) < 0){
                    cerr << "\x1b[31;1mCannot join host " << cargs[0] << " on port " << cargs[1] << " (" << error << ")\x1b[0m" << endl;
                }else{
                    cout << "\x1b[36;1mHost " << cargs[0] << " on port " << cargs[1] << "connected\x1b[0m" << endl;
                }

            }


       }else{
           PDU::RAW_PDU rpdu;
           std::copy(std::begin(in), std::end(in), std::begin(rpdu));
           PDU pdu(rpdu);
           chan.write(pdu);
       }
    }while(!end);

    chan.close();

    std::cerr << "error " << std::endl;

    return 0;
}

