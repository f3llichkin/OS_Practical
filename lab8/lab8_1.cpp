#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <queue>
#include <cstring>
//dev by f3llichkin A.Savyolov
using namespace std;

struct sockaddr_in servSockAddr, cliAddr;
struct th_server_args
{
    char flag;
    int listen_socket;
    int conn_server;
    mutex mx;
    queue<string> querySet;
};
socklen_t sizecliAddr;

void server_send(th_server_args& args)
{
	int count = 0;
    while(args.flag != 10)
    {
        args.mx.lock();
        if(!args.querySet.empty())
        {
            string message = args.querySet.front();
            args.querySet.pop();
            args.mx.unlock();
		char mydomainbuf[256];
    		getdomainname(mydomainbuf,(int)sizeof(mydomainbuf)); 
            string response = string(mydomainbuf) + " ";

            vector<char> resp_c(response.begin(), response.end());
            for (const auto& symb : to_string(count))
            resp_c.push_back(symb);
            socklen_t sizecliAddr=sizeof(struct sockaddr_in);
            int sendcount = sendto(args.listen_socket, &resp_c[0], resp_c.size(),MSG_CONFIRM,(struct sockaddr*)&cliAddr, sizecliAddr);

            if (sendcount == -1) 
            {
                perror("send error");
            } 
            else
            {
                cout << "Send OK" << endl;
            }
            count++;
        }
        else
        {
            args.mx.unlock();
            this_thread::sleep_for(chrono::seconds(1));
        }   
    }
}


void server_get(th_server_args& args)
{
    const int RCV_SIZE = 1024;
    
    while(args.flag != 10)
    {
        vector<char> buf(RCV_SIZE);
        string rcv;
         sizecliAddr=sizeof(struct sockaddr_in);
        int reccount = recvfrom(args.listen_socket, &buf[0], RCV_SIZE, MSG_WAITALL,(struct sockaddr*)&cliAddr, &sizecliAddr);
        
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
            rcv.append(buf.begin(), buf.end());
            cout << rcv << endl;

            args.mx.lock();
            args.querySet.push(rcv);
            args.mx.unlock();
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}



int main()
{
    
    int optval = 1;
    th_server_args params;
    params.listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
    fcntl(params.listen_socket, F_SETFL, O_NONBLOCK);
    memset(&servSockAddr, 0, sizeof(servSockAddr));
    memset(&cliAddr, 0, sizeof(cliAddr));
    servSockAddr.sin_family = AF_INET;
    servSockAddr.sin_port = htons(8080);
    servSockAddr.sin_addr.s_addr = INADDR_ANY;
    bind(params.listen_socket, (struct sockaddr*)&servSockAddr, sizeof(servSockAddr));
    setsockopt(params.listen_socket, SOL_SOCKET, SO_RCVTIMEO, &optval, sizeof(optval));    

    cout << "Server started\n" << endl;
    thread th_server_send(server_send, ref(params));
    thread th_server_get(server_get, ref(params));
  
    params.flag = getchar();
    th_server_send.join();
    th_server_get.join();

    cout << "\nServer finished" << endl;
    close(params.listen_socket);
    return 0;
}
