#pragma once

#include <functional>
#include <cstddef>
#include <memory>

namespace Client {
    class NetworkClient
    {
    private:
        /* data */
    public:
        NetworkClient(std::function<void(std::unique_ptr<std::byte> buf)> recv_callback = nullptr);
        ~NetworkClient();

        void connect(const char* ip);
        void send(std::unique_ptr<std::byte> buf, std::size_t size);
    };
}