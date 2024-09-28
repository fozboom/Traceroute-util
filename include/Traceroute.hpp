#ifndef TRACEROUTE_HPP
#define TRACEROUTE_HPP
#include <iostream>
#include <string>
#include <sys/socket.h>

class Traceroute {
  std::string hostname_;
  std::string target_ip_;
  sockaddr target_address_;
  int max_hops_;
  int icmp_socket_descriptor_;

  void CreateSocket();
  void ResolveHostnameToIp();

public:
  Traceroute(const std::string& target_name, int max_hops);
  ~Traceroute() = default;
  std::string GetTargetIp() const;
};
#endif //TRACEROUTE_HPP