#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>  
#define PORT 2022
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

    // struct sockaddr_in client;
    // socklen_t client_addrlength = sizeof( client );
    // int sockfd = accept(listenfd, (struct sockaddr*)(&address), &client_addrlength);

    epoll_event events[MAX_EVENTS_NUMBER];
    int epollfd = epoll_create(5);
    epoll_event event;
    event.data.fd = listenfd;
    event.events = EPOLLIN;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);


    while(1){
        int eventCnt = epoll_wait(epollfd, events, MAX_EVENTS_NUMBER, -1);
        for (int i = 0; i < eventCnt; i++){
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd){
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int clientfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);

                struct epoll_event event;
                event.data.fd = clientfd;
                event.events = EPOLLIN|EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event);
            }
            else if(events[i].events & EPOLLIN ){
                char buf[1024] = {0};
				while(1)
				{
					memset(buf, '\0', sizeof(buf));
					int recv_size  = recv(sockfd, buf, sizeof(buf), 0 );
					if(recv_size == 0)
					{
					   	close(sockfd);
						break;	
					}
					else
					{
						send(sockfd, buf, recv_size, 0);
					}	
				}
            }
        }
    }

	return 0;
}