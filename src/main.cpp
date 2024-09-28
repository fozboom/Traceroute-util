#include <Traceroute.hpp>

int main() {
  const Traceroute kTraceroute("darkrrsrewrpg.ru", 30);
  std::cout << "Target ip: " << kTraceroute.GetTargetIp() << std::endl;
  return 0;
}