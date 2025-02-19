#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <thread>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

vector<int> clients;

void handleClient(int clientSocket){
    char buffer[BUFFER_SIZE];
    while(true){
        memset(buffer, 0, BUFFER_SIZE);
        int bytesRead = read(clientSocket, buffer, BUFFER_SIZE);
        if(bytesRead<=0){
            break;
        }
        cout<<"Received:"<<buffer<<endl;

        for(int client : clients){
            if(client != clientSocket){
                send(client, buffer, bytesRead, 0);
            }
        }
    }
    close(clientSocket);
}

int main(){
    int serverSocket, clinetSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket<0){
        cerr<<"Socket creation failed!"<<endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    //if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    //    cerr<<"Bind failed"<<endl;
    //    return -1;
    //}

    listen(serverSocket, 3);
    cout<<"Server listening on port"<<endl;

    while(true){
        clinetSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if(clinetSocket<0){
            cerr<<"Accept Failed"<<endl;
            continue;
        }
        clients.push_back(clinetSocket);
        thread(handleClient, clinetSocket).detach();

    }
    close(serverSocket);
    return 0;

}