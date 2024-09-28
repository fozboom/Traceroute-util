#ifndef SOCKETCREATIONEXCEPTION_HPP
#define SOCKETCREATIONEXCEPTION_HPP
#include <stdexcept>

class SocketCreationException final : public std::runtime_error
{
public:
    explicit SocketCreationException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};
#endif //SOCKETCREATIONEXCEPTION_HPP
