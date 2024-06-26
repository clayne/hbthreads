#pragma once

#include <netinet/in.h>

namespace hbthreads {

// Creates a tcp socket
int createTCPSocket();

// Creates a TCP socket and binds to a given address/port
int createAndBindTCPSocket(const char *address, int port);

// Creates a datagram socket
int createUDPSocket();

// Creates a datagram socket and binds to address/port
bool bindSocket(int fd, const char *address, int port);

// Sets non blocking flag on socket
bool setSocketNonBlocking(int sockid);

//! Parse an IP address as numeric octets (x.x.x.x) or interface name
bool parseIPAddress(const char *address, in_addr *addr);

//! Parses an IP address and port
bool parseIPAddress(const char *address, int server_port, sockaddr_in *addr);

//! Subscribes socket to given multicast IP an port on an interface
//! If the interface is null it subscribes on all interfaces
bool setSocketMulticastJoin(int sockid, const char *ipaddr, int port,
                            const char *interface = nullptr);

//! Sets the reuse flag on a socket
bool setSocketReuseFlag(int sockid);

//! Sets socket receive buffer size
bool setSocketReceiveBufferSize(int sockfd, uint32_t bufsize);

//! Retrieves the socket buffer size
uint32_t getSocketReceiveBufferSize(int sockfd);

//! Sets the maximum number of hops messages could go
//! Affects only outgoing multicast packets
bool setSocketMulticastTTL(int sockid, int ttl);

}  // namespace hbthreads