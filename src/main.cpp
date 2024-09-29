#include <Traceroute.hpp>
#include <iostream>
#include <HostnameResolutionException.hpp>
#include <SocketCreationException.hpp>
int main() {
  try {
    Traceroute traceroute("185.117.0.85", 30);
    std::cout << "Target ip: " << traceroute.GetTargetIp() << std::endl;
    traceroute.RunTraceroute();
  } catch (const HostnameResolutionException& e) {
    std::cerr << "Hostname resolution error: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  } catch (const SocketCreationException& e) {
    std::cerr << "Socket creation error: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

}
