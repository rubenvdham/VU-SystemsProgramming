//
// Created by ruben on 8-9-18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define DEFAULT_PORT 2012
#define BUFFER_SIZE 2048



int setup_socket(int port){
    int fd,err;
    struct sockaddr_in addr;
    fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (fd<0) {
        perror("Couln't create socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port  = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    err = bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (err<0) {
        perror("Couln't bind to socket");
        exit(1);
    }
    return fd;
}



int main(){
    int fd, msg_length,sent_length, counter;
    char buff[BUFFER_SIZE];
    struct sockaddr_in from;
    socklen_t from_len;

    fd = setup_socket(DEFAULT_PORT);

    from_len = sizeof(from);
    printf("LISTENING ON PORT %d\n",DEFAULT_PORT);
    counter = 1;
    while(1){
        msg_length =  recvfrom(fd, &buff, BUFFER_SIZE, 0,(struct sockaddr *) &from, &from_len);
        if(msg_length<0){
            perror("Error retrieving bytes from UDP packet");
            exit(1);
        }
        counter++;
        if(counter%2==0){
            printf("Received %d bytes from host %s port %d: %s\n", msg_length, inet_ntoa(from.sin_addr), ntohs(from.sin_port), buff);
        }else{
            printf("DROPPING PACKET OF %d bytes from host %s port %d: %s\n", msg_length, inet_ntoa(from.sin_addr), ntohs(from.sin_port), buff);
            continue;
        }
        sent_length = sendto(fd, buff, msg_length, 0, (struct sockaddr *) &from, from_len);

        if(sent_length!=msg_length){
            perror("Did not sent the same amount of bytes as received");
            exit(1);
        }
    }
}