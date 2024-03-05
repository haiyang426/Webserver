#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#define PORT 2024
#define MAX_EVENTS_NUMBER 5

using namespace std;


int main(){
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    int flag = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    int ret;
    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));

    ret = listen(listenfd, 5);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof( client );
    int sockfd = accept(listenfd, (struct sockaddr*)(&address), &client_addrlength);
    
    char buf_size[1024] = {0};
	int recv_size = 0;
	recv_size = recv( sockfd, buf_size, sizeof(buf_size) , 0);
	
	int send_size = 0;
	send_size = send( sockfd, buf_size , recv_size , 0);
	
	close(sockfd);
	close(listenfd);

    return 0;
  }