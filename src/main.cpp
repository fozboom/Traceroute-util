#include <Traceroute.hpp>
#include <iostream>
int main() {
  Traceroute traceroute("185.117.0.85", 30);
  std::cout << "Target ip: " << traceroute.GetTargetIp() << std::endl;
  traceroute.RunTraceroute();
  return 0;
}