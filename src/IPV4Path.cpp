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
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <iostream>

#include "IPV4Path.h"
#include "IP.h"

const int BUFFER_SIZE = 1500; //MTU

IPV4Path::IPV4Path() {
    path = std::vector<IPV4>();
}

void IPV4Path::add(IPV4 ip) {
    path.push_back(ip);
}

int IPV4Path::size() const {
    return path.size();
}

IPV4Path* IPV4Path::fromDomain(std::string domain) {
    struct addrinfo* addrinfo;

    struct addrinfo hint = {};

    hint.ai_family = AF_INET;

    if (getaddrinfo(domain.c_str(), NULL, &hint, &addrinfo) < 0) {
        perror("address resolve ");
        return nullptr;
    }

    return fromAddress(addrinfo);
}


IPV4Path* IPV4Path::fromIP(IPV4 target) {
    struct addrinfo* addrinfo;

    struct addrinfo hint = {};

    hint.ai_family = AF_INET;

    if (getaddrinfo(target.toString().c_str(), NULL, &hint, &addrinfo) < 0) {
        perror("address resolve ");
        return nullptr;
    }

    return fromAddress(addrinfo);
}

int send_ping(struct addrinfo* addrinfo, int ttl, char*& response, int& responselen ) {
    char* sendbuf = new char[BUFFER_SIZE];
    char* recvbuf = new char[BUFFER_SIZE];
    char* controlbuf = new char[BUFFER_SIZE];

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

    ip* ip;
    int ip_len;
    int data_len;

    while (1) {
        recv_len = recvmsg(sock, &msg, 0);

        if (recv_len < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
            perror("recieve");
            return -1;
        }
        else if (recv_len < 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
            return -1;
        }

        ip = (struct ip*) recvbuf;
        ip_len = ip->ip_hl << 2;

        icmp = (struct icmp*) (recvbuf + ip_len);
        data_len = (recv_len - ip_len);
        if (icmp->icmp_type == ICMP_TIME_EXCEEDED || icmp->icmp_type == ICMP_ECHOREPLY) break;
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    //Return data
    response = recvbuf;
    responselen = recv_len;

    delete[] sendbuf;
    delete[] controlbuf;

    return 0;
}

IPV4Path* IPV4Path::fromAddress(struct addrinfo* addrinfo) {
    IPV4Path* ipPath = new IPV4Path;


    char* response;
    int recvlen;

    struct icmp* icmp;
    ip *ip;
    int ip_len, data_len;


    int ttl = 0;
    std::string prev_ip;
    std::string curr_ip;

    do {
        prev_ip = curr_ip;
        ttl++;

        if (send_ping(addrinfo, ttl, response, recvlen) < 0) {
            ipPath->add(IPV4(NO_IP));
            continue;
        }

        ip = (struct ip*) response;
        ip_len = ip->ip_hl << 2;

        icmp = (struct icmp*) (response + ip_len);
        data_len = (recvlen - ip_len);

        socklen_t size = INET_ADDRSTRLEN;
        char* buffer = new char[size];

        inet_ntop(AF_INET, &ip->ip_src, buffer, size);

        ipPath->add(IPV4(std::string(buffer)));

        delete[] buffer;

        //If it is a reply, we are done, otherwise, keep going
        if (icmp->icmp_type == ICMP_ECHOREPLY) {
            break;
        }
    }

    while (ttl < 30);

    return ipPath;
}

IPV4& IPV4Path::at(int i) {
    return path.at(i);
}

const IPV4& IPV4Path::at(int i) const{
    return path.at(i);
}

void IPV4Path::print() const {
    for (int i = 0; i < path.size(); i++) {
        std::cout << path.at(i).toString() << " ";
        if (i < path.size() - 1) std::cout << "---> ";
    }
    std::cout << std::endl;
}