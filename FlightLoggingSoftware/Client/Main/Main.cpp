#include <iostream>
#include "../NetworkClient/NetworkClient.h"
#include "../../DataProtocol/DataProtocol.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <thread>


#define NUM_THREADS 8

int main(void)
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve("localhost", "1234");
    std::thread t([&io_context]() { io_context.run(); });

    Client::NetworkClient network_client(io_context, endpoints);

    for (int i = 0; i < 10; i++)
    {
        std::unique_ptr<DataProtocol::Transmission> dummy \
            = std::make_unique<DataProtocol::DummyTransmission>();

        network_client.send(std::move(dummy));
    }

    network_client.close();
    t.join();

    return 0;
}