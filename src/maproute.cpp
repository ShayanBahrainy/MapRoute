#include <asm-generic/socket.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h> //ip hdeader library (must come before ip_icmp.h)
#include <netinet/ip_icmp.h> //icmp header
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h>

#include "internet.h"

const int BUFFER_SIZE = 1500; //MTU

unsigned short chksum(unsigned short* packet, int len) {
    int i;
    unsigned int sum = 0;
    unsigned short *ptr;
    unsigned short cksm;

    for (i = len, ptr = packet; i > 1 ; i-=2) {
        sum += *ptr;
        ptr++;
    }

    if (i == 1) {
        sum += *((unsigned char*) ptr);
    }

    sum = (sum & 0xffff) + (sum >> 16);

    sum += (sum >> 16);

    cksm = ~sum;

    return cksm;
}

int send_ping(struct addrinfo* addrinfo, int ttl, char*& response, int& responselen ) {
    char sendbuf[BUFFER_SIZE], recvbuf[BUFFER_SIZE], controlbuf[BUFFER_SIZE];
    icmp *icmp;
    iovec iov;
    msghdr msg;

    int packet_len, recv_len;

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sock < 0) {
        perror("socket");
        return -1;
    }

    icmp = (struct icmp*) sendbuf;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_seq = 0;
    icmp->icmp_id = getpid() & 0xffff;
    icmp->icmp_cksum = 0;

    icmp->icmp_cksum = chksum((unsigned short *) icmp, sizeof(struct icmp));

    packet_len = sizeof(struct icmp);

    //Set TTL
    setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    //Set Timeout
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    if (sendto(sock, icmp, packet_len, 0, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
        perror("send");
        return -1;
    }

    iov.iov_base = recvbuf;
    iov.iov_len = BUFFER_SIZE;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = controlbuf;
    msg.msg_controllen = BUFFER_SIZE;

    if ((recv_len = recvmsg(sock, &msg, 0)) < 0 && errno != EWOULDBLOCK) {
        perror("recieve");
        return -1;
    }
    else if (errno == EWOULDBLOCK) {
        //std::cout << "-----------" << "Timeout" << "-----------" << std::endl;
        return -1;
    }


    shutdown(sock, SHUT_RDWR);
    close(sock);

    //Return data
    response = recvbuf;
    responselen = recv_len;

    return 0;
}


char* getIPAt(int ttl, std::string target) {
    struct addrinfo* addrinfo;

    if (getaddrinfo(target.c_str(), NULL, NULL, &addrinfo) < 0) {
        perror("address resolve ");
        exit(1);
        return nullptr;
    }

    char* response;
    int recvlen;


    struct icmp* icmp;
    ip *ip;
    int ip_len, data_len;


    if (send_ping(addrinfo, ttl, response, recvlen) < 0) {
        return nullptr;
    }

    ip = (struct ip*) response;
    ip_len = ip->ip_hl << 2;

    icmp = (struct icmp*) (response + ip_len);
    data_len = (recvlen - ip_len);

    return inet_ntoa(ip->ip_src);
}


int main() {
    while (true) {
        std::string target;
        std::cout << "Maproute target: ";
        std::getline(std::cin, target);

        int ttl = 0;
        std::string prev_ip;
        std::string curr_ip;

        do {
            prev_ip = curr_ip;
            ttl++;
            char* raw_ip = getIPAt(ttl, target);
            curr_ip = raw_ip != nullptr ? std::string(raw_ip) : "<no-ip>";
            if (curr_ip != "<no-ip>")
                AS asn = asn.fromIP(IPV4(curr_ip));
            if (curr_ip != "<no-ip>") {
                std::cout << "Translate to and from IPv4 object: " << IPV4(curr_ip).toString() << std::endl;
            }

            std::cout << curr_ip << std::endl;
        }
        while (curr_ip != prev_ip || (curr_ip == "<no-ip>" && prev_ip == "<no-ip>"));
    }

    return 0;
}