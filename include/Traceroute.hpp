#ifndef TRACEROUTE_HPP
#define TRACEROUTE_HPP
#include <string>
#include <sys/socket.h>
#include <mutex>
#include <map>
class Traceroute {
  std::string hostname_;        // Target hostname
  std::string target_ip_;       // Target IP address (resolved from hostname_)
  sockaddr target_address_;     // Target address
  int max_hops_;                // Maximum number of hops
  int icmp_socket_descriptor_;  // ICMP socket descriptor
  std::mutex mutex_;            // Mutex for thread safety
  // Map to store IP addresses for each TTL (for order printing purposes)
  std::map<int, std::string> ttl_to_ip_;

  void CreateSocket();                        // Create ICMP socket
  void ResolveHostnameToIp();                 // Resolve hostname to IP address
  void SendIcmpRequest(int ttl) const;        // Send ICMP request
  void ReceiveIcmpReply(int ttl);             // Receive ICMP reply
  void SetSocketTimeout(int seconds) const;   // Set socket timeout

public:
  Traceroute(const std::string& target_name, int max_hops);
  ~Traceroute() = default;
  void RunTraceroute();                       // Run the traceroute logic
  std::string GetTargetIp() const;            // Get the target IP address

};
#endif //TRACEROUTE_HPP