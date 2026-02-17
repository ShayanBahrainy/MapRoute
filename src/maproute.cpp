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

#include "IP.h"
#include "AS.h"

const int BUFFER_SIZE = 1500; //MTU

int send_ping(struct addrinfo* addrinfo, int ttl, char*& response, int& responselen ) {
    std::cout << "maproute.cpp: " << addrToString(addrinfo) << std::endl;
    char *sendbuf, *recvbuf, *controlbuf;

    sendbuf = new char[BUFFER_SIZE];
    recvbuf = new char[BUFFER_SIZE];
    controlbuf = new char[BUFFER_SIZE];

    icmp *icmp;
    iovec iov;
    msghdr msg;

    int packet_len, recv_len;

    int protocol;
    protocol = IPPROTO_ICMP;

    int sock = socket(addrinfo->ai_family, SOCK_RAW, protocol);

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

    delete[] controlbuf;
    delete[] sendbuf;

    return 0;
}


const char* getIPAt(int ttl, std::string target) {
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


    socklen_t size = INET_ADDRSTRLEN;
    char* buffer = new char[size];

    inet_ntop(AF_INET, &ip->ip_src, buffer, size);

    delete response;

    return buffer;
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
            const char* raw_ip = getIPAt(ttl, target);
            curr_ip = raw_ip != nullptr ? std::string(raw_ip) : "null ptr ip";
            if (curr_ip != "null ptr ip")
                AS asn = asn.fromIP(IPV4(curr_ip));

            std::cout << curr_ip << (curr_ip == prev_ip ? " equal" : " not equal") << " to " << prev_ip << std::endl;
        }
        while (curr_ip != prev_ip && !(curr_ip == "null ptr ip" && prev_ip == "null ptr ip"));

        std::cout << "Numbers of hops: " << ttl  - 1<< std::endl;
    }

    return 0;
}