//
// Created by ruben on 29-9-18.
//

#ifndef SYSTEMSPROGRAMMING_PROTOCOL_H
#define SYSTEMSPROGRAMMING_PROTOCOL_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

extern const int DEFAULT_PORT;



int handle_control_message(int fd, struct sockaddr_in *from,char* datafile, char* libfile);
int setup_control_message(int fd, struct sockaddr_in *from,char* datafile, char* libfile,int *sample_size,int *sample_rate,int *channels);
int confirm_control_message(int fd, struct sockaddr_in *from,int sample_size,int sample_rate, int channels);
int handle_helo_connection(int fd, struct sockaddr_in *from);
int setup_helo_connection(int fd, struct sockaddr_in *from);
int initiate_rst(int fd,struct sockaddr_in *from);
int reply_to_rst(int fd,struct sockaddr_in *from);
int tokenize_control_message(char* message,char* datafile, char* libfile);
int tokenize_audio_props(char *message,int *sample_size,int *sample_rate,int *channels);

int send_message(int fd, struct sockaddr_in *to, char* buff, int buf_size);
int receive_message(int fd, struct sockaddr_in *from, char* buff,int buf_size);

void *allocate_memory(size_t size);




#endif //SYSTEMSPROGRAMMING_PROTOCOL_H
