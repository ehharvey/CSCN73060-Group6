#pragma once

#include <functional>
#include <cstddef>
#include <memory>

namespace Server {
    class NetworkServer
    {
    private:
        void stop();
    public:
        // Callback function is called whenever the server receives a packet
        NetworkServer(
            std::function< std::unique_ptr<std::byte> (std::unique_ptr<std::byte>) > receive_callback
        );

        void waitUntilStops();

        ~NetworkServer();
    };
}