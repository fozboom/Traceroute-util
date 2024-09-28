#ifndef HOSTNAMERESOLUTIONEXCEPTION_HPP
#define HOSTNAMERESOLUTIONEXCEPTION_HPP
#include <stdexcept>

class HostnameResolutionException final : public std::runtime_error
{
public:
    explicit HostnameResolutionException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};
#endif //HOSTNAMERESOLUTIONEXCEPTION_HPP
