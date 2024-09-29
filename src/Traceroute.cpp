#include <Traceroute.hpp>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <HostnameResolutionException.hpp>
#include <SocketCreationException.hpp>
#include <iostream>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <thread>
#include <vector>

Traceroute::Traceroute(const std::string& target_name, int max_hops):
  hostname_(target_name), max_hops_(max_hops) {
    // initialize target_ip_ using hostname_
    ResolveHostnameToIp();
    // create ICMP socket
    CreateSocket();
}

std::string Traceroute::GetTargetIp() const {
  return target_ip_;
}

void Traceroute::CreateSocket() {
  // Create raw socket for ICMP
  const int kSocketDescriptor = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (kSocketDescriptor == -1) {
    throw SocketCreationException(
        "Error while creating socket: " + std::string(strerror(errno)));
  }
  icmp_socket_descriptor_ = kSocketDescriptor;
}

// Resolve hostname to IP address
void Traceroute::ResolveHostnameToIp() {
  // Struct to hold options for address resolution
  addrinfo hints{};
  hints.ai_family = AF_INET; // IPv4
  addrinfo* res;
  // Resolve the hostname to an address
  if (getaddrinfo(hostname_.c_str(), nullptr, &hints, &res) != 0) {
    throw HostnameResolutionException("Error while converting hostname to IP");
  }

  char ip[INET_ADDRSTRLEN];
  // Convert the resolved address to a string (IPv4)
  if (inet_ntop(
          AF_INET, &(reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr),
          ip,
          sizeof(ip)) == nullptr) {
    freeaddrinfo(res);
    throw HostnameResolutionException(
        "Error converting IP to string: " + std::string(strerror(errno)));
  }

  target_ip_ = std::string(ip);

  freeaddrinfo(res);
}


// Set timeout for the socket to ensure recvfrom doesn't block indefinitely
void Traceroute::SetSocketTimeout(int seconds) const {
  struct timeval timeout{};
  timeout.tv_sec = seconds;
  timeout.tv_usec = 0;

  // Apply timeout setting to the socket
  if (setsockopt(icmp_socket_descriptor_, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                 sizeof(timeout)) < 0) {
    throw SocketCreationException(
        "Error while setting socket timeout: " + std::string(strerror(errno)));
  }
}

// Receive an ICMP reply for the given TTL value
void Traceroute::ReceiveIcmpReply(int ttl) {
  // Buffer for receiving the ICMP packet
  char buffer[1024];
  // Struct to store sender address information
  sockaddr_in sender_addr;
  socklen_t sender_len = sizeof(sender_addr);

  // Attempt to receive an ICMP reply from the socket
  int bytes_received = recvfrom(icmp_socket_descriptor_, buffer, sizeof(buffer),
                                0,
                                reinterpret_cast<sockaddr*>(&sender_addr),
                                &sender_len);
  if (bytes_received > 0) {
    char sender_ip[INET_ADDRSTRLEN];
    // Convert the sender's address to a readable IP string
    inet_ntop(AF_INET, &sender_addr.sin_addr, sender_ip, sizeof(sender_ip));
    // Lock access to the map
    std::lock_guard lock(mutex_);
    ttl_to_ip_[ttl] = sender_ip;
  }
}


// Main loop that handles the traceroute logic
void Traceroute::RunTraceroute() {
  // Set a 5-second timeout for receiving ICMP replies
  SetSocketTimeout(5);
  // Create a thread for each TTL to send an ICMP request and receive a reply
  for (int ttl = 1; ttl <= max_hops_; ++ttl) {
    std::thread([&, ttl]() {
      SendIcmpRequest(ttl);
      ReceiveIcmpReply(ttl);
    }).detach();
    usleep(10000);
    // Add a small delay between sending requests to avoid overloading the network
  }

  // Output all results in TTL order
  for (const auto& [ttl, ip] : ttl_to_ip_) {
    std::cout << ttl << ": " << ip << std::endl;
  }

  std::cout << "Traceroute finished for " << target_ip_ << std::endl;
}

// Send an ICMP request with a specified TTL value
void Traceroute::SendIcmpRequest(int ttl) const {
  // Set the TTL for the socket's packets
  setsockopt(icmp_socket_descriptor_, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

  // Prepare an ICMP Echo Request packet
  char packet[64] = {};

  auto* icmp_hdr = reinterpret_cast<struct icmphdr*>(packet);
  icmp_hdr->type = ICMP_ECHO;       // Type for Echo Request
  icmp_hdr->code = 0;               // Code for Echo Request
  icmp_hdr->checksum = 0;           // No checksum calculation here
  icmp_hdr->un.echo.id = getpid();  // Process ID for the ICMP packet
  icmp_hdr->un.echo.sequence = ttl; // Sequence number for the ICMP packet

  // Prepare the target address struct
  sockaddr_in target_addr{};
  target_addr.sin_family = AF_INET;
  // Convert the target IP string to binary format
  inet_pton(AF_INET, target_ip_.c_str(), &target_addr.sin_addr);
  // Send the ICMP request to the target
  sendto(icmp_socket_descriptor_, packet, sizeof(packet), 0,
         reinterpret_cast<sockaddr*>(&target_addr), sizeof(target_addr));
}