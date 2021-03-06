//
// Created by ruben on 29-9-18.
//

#include "networking.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int setup_server_socket(int port){
    int fd,err;
    struct sockaddr_in addr;
    fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (fd<0) {
        return -2;
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

int setup_socket(){
    int fd;
    fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (fd<0) {
        perror("Couln't create socket");
        exit(1);
    }
    return fd;
}

struct in_addr* get_ip(const char *name) {
    struct hostent *resolv;
    struct in_addr *addrp = (struct in_addr*) malloc(sizeof(struct in_addr));

    if(addrp==NULL){
        perror("Can't allocate memory on heap");
        exit(1);
    }
    resolv = gethostbyname(name);
    if (resolv==NULL) {
        perror("IP Address not found/invalid");
        exit(1);
    }
    memcpy(addrp,resolv->h_addr_list[0], sizeof(struct in_addr));
    return addrp;
}


int send_packet(int fd,struct sockaddr_in *to,char *buff, int msg_length){
    int sent_length;
    socklen_t to_len;
    to_len = sizeof(*to);

    //send the packet
    sent_length = sendto(fd, buff, msg_length, 0,(struct sockaddr *) to, to_len);

    if(sent_length!=msg_length){
        printf("sent %d, expected %d\n", sent_length, msg_length);
        perror("Error sending packet\n\n");
        return -1;
    }
    return sent_length;
}


int receive_packet_with_timeout(int fd,int max_reply_length, struct sockaddr_in *from, char *buff){
    socklen_t from_len;
    struct timeval timeout;
    fd_set read_set;
    int sel,received_length;
    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;
    from_len = sizeof(from);

    //wait for the reply (until reply or timeout)
    sel = select(fd+1, &read_set, NULL, NULL, &timeout);
    if (sel<0) {
        perror("Error");
        return -1;
    }
    if (sel==0) {
        return 0; //time-out
    }

    if (FD_ISSET(fd,&read_set)) {
        received_length = recvfrom(fd, buff, max_reply_length, 0, (struct sockaddr *) from, &from_len);
        if (received_length < 0) {
            perror("Error retrieving bytes from UDP packet");
            return -1;
        }
    }else{
        perror("File descriptor is not set as readable, though no timeout occurred");
        return -1;
    }
    return received_length;
}

