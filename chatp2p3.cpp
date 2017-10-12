#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <stdio_ext.h>
#include <iostream>

using namespace std;

string ip;
std::thread readt, writet;
//#define IP "127.0.0.1"
//#define PORT_S 9930
//#define PORT_C 9931
#define BUFLEN 512

int s; // s -server, t -client
int ports, portc;
struct sockaddr_in me, other, self_addr;

socklen_t len = sizeof(struct sockaddr);

void readfunc(){
	struct sockaddr_in si_me, si_other;
    int s, i;
    socklen_t slen=sizeof(si_other);
    char buf[BUFLEN];

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) printf("socket\n");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(ports);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me))==-1) printf("bind() failed\n");

    while(1){
    	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)==-1) printf("recvfrom() failed \n");
   		printf("Received: %s\n\n", buf);
    }

    close(s);
}

void writefunc(){
	struct sockaddr_in si_other2;
    int s, i;
    socklen_t slen = sizeof(struct sockaddr);
    char buf[BUFLEN];

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) printf("socket failed\n");
    
    memset((char *) &si_other2, 0, sizeof(si_other2));
    si_other2.sin_family = AF_INET;
    si_other2.sin_port = htons(portc);
    if (inet_aton(ip.c_str(), &si_other2.sin_addr)==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(1){
    	printf("msg: ");
		scanf("%s", buf);    
	    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other2, slen)==-1) printf("sendto() failed\n");
    }

    close(s);
}


int main(){
    printf("ip: ");
    cin >> ip;

	ports = 9931;
	portc = 9931;

	std::thread writet(readfunc);
	std::thread readt(writefunc);

	readt.join();
	writet.join();



	return 0;
}