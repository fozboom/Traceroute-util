#include <Traceroute.hpp>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <HostnameResolutionException.hpp>
#include <SocketCreationException.hpp>

Traceroute::Traceroute(const std::string& target_name, int max_hops):
  hostname_(target_name), max_hops_(max_hops) {
  try {
    ResolveHostnameToIp();
    CreateSocket();
  } catch (const HostnameResolutionException& e) {
    std::cerr << "Hostname resolution error: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  } catch (const SocketCreationException& e) {
    std::cerr << "Socket creation error: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

std::string Traceroute::GetTargetIp() const {
  return target_ip_;
}

void Traceroute::CreateSocket() {
  const int kSocketDescriptor = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (kSocketDescriptor == -1) {
    throw SocketCreationException(
        "Error while creating socket: " + std::string(strerror(errno)));
  }
  icmp_socket_descriptor_ = kSocketDescriptor;
}

void Traceroute::ResolveHostnameToIp() {
  addrinfo hints{};
  hints.ai_family = AF_INET;
  addrinfo* res;
  if (getaddrinfo(hostname_.c_str(), nullptr, &hints, &res) != 0) {
    throw HostnameResolutionException("Error while converting hostname to IP");
  }

  char ip[INET_ADDRSTRLEN];
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