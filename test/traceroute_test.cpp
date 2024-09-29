#include <gtest/gtest.h>
#include <Traceroute.hpp>
#include <HostnameResolutionException.hpp>
#include <SocketCreationException.hpp>\


class TracerouteTestable : public Traceroute {
public:

  TracerouteTestable(const std::string& target_name, int max_hops)
      : Traceroute(target_name, max_hops) {}


  void PublicResolveHostnameToIp() {
    ResolveHostnameToIp();
  }

  void PublicCreateSocket() {
    CreateSocket();
  }

  void PublicSendIcmpRequest(int ttl) const {
    SendIcmpRequest(ttl);
  }

  void PublicReceiveIcmpReply(int ttl) {
    ReceiveIcmpReply(ttl);
  }

  void PublicSetSocketTimeout(int seconds) const {
    SetSocketTimeout(seconds);
  }
};


TEST(TracerouteTestable, ResolveHostnameToIp) {
  TracerouteTestable traceroute("google.com", 30);
  ASSERT_NO_THROW(traceroute.PublicResolveHostnameToIp());
}

TEST(TracerouteTestable, InvalidHostnameThrowsException) {
  ASSERT_THROW(TracerouteTestable traceroute("invalid.hostname", 30), HostnameResolutionException);
}


TEST(TracerouteTestable, CreateSocket) {
  TracerouteTestable traceroute("google.com", 30);
  ASSERT_NO_THROW(traceroute.PublicCreateSocket());
}

TEST(TracerouteTestable, SetSocketTimeout) {
  TracerouteTestable traceroute("google.com", 30);
  traceroute.PublicCreateSocket();  // Ensure socket is created before setting timeout
  ASSERT_NO_THROW(traceroute.PublicSetSocketTimeout(5));
}

TEST(TracerouteTestable, SendIcmpRequest) {
  TracerouteTestable traceroute("google.com", 30);
  ASSERT_NO_THROW(traceroute.PublicSendIcmpRequest(1));
}

TEST(TracerouteTestable, ReceiveIcmpReply) {
  TracerouteTestable traceroute("google.com", 30);
  traceroute.PublicSendIcmpRequest(1);
  ASSERT_NO_THROW(traceroute.PublicReceiveIcmpReply(1));
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}