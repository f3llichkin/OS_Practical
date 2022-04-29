#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <chrono>
#include <vector>
#include <cstring>
using namespace std;
//dev by f3llichkin A.Savyolov
struct sockaddr_in servSockAddr;
struct th_client_args
{
    char flag;
    int client_socket;
};


void client_get(th_client_args& args)
{
    while(args.flag != 10)
    {
        const int RCV_SIZE = 1024;
        vector<char> buf_message(RCV_SIZE);
        socklen_t servAdrSize=sizeof(servSockAddr);
        int reccount = recvfrom(args.client_socket, &buf_message[0], RCV_SIZE,MSG_WAITALL,(struct sockaddr *)&servSockAddr, &servAdrSize);

        if (reccount == -1) 
        {
            perror("recv error");
            this_thread::sleep_for(chrono::seconds(1));
        }
        else if (reccount == 0) 
        {
            this_thread::sleep_for(chrono::seconds(1));
        }
        else
        {
            for (const auto& symb : buf_message)
                cout << symb;
            cout << endl;

            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}


void client_send( th_client_args& args)
{
    int count = 0;
    string buf = "Message from client ";
    
    while(args.flag != 10)
    {
        vector<char> buf_request(buf.begin(), buf.end());
        for (const auto& symb : to_string(count))
            buf_request.push_back(symb);
	socklen_t servtAdrSize=sizeof(servSockAddr);
        int sendcount = sendto(args.client_socket, &buf_request[0], buf_request.size(),MSG_CONFIRM,(struct sockaddr*)&servSockAddr,servtAdrSize);
        if (sendcount == -1) 
        {
            perror("send error");
        }
        else
        {
            cout << "Send OK" << endl;
        }

        ++count;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    th_client_args params;
    
    params.client_socket = socket(AF_INET,SOCK_DGRAM,0);
    fcntl(params.client_socket,F_SETFL,O_NONBLOCK);
    memset(&servSockAddr, 0, sizeof(servSockAddr));
    servSockAddr.sin_family = AF_INET;
    servSockAddr.sin_port = htons(8080);
    servSockAddr.sin_addr.s_addr = INADDR_ANY;
    cout << "Client started\n" << endl;
    thread th_client_send(client_send, ref(params));
    thread th_client_get(client_get, ref(params));
    params.flag = getchar();
    th_client_send.join();
    th_client_get.join();

    cout << "\nClient finished" << endl;
    close(params.client_socket);
    return 0;
}
