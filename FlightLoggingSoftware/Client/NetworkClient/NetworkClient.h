#pragma once

#include <functional>
#include <cstddef>
#include <mutex>
#include <memory>
#include <boost/asio.hpp>
#include <queue>
#include <thread>
#include "../../DataProtocol/DataProtocol.h"

namespace Client {
    class NetworkClient
    {
    private:
        boost::asio::io_service& io_service;
        boost::asio::ip::tcp::socket socket;

        NetworkClient();
    public:
        NetworkClient(boost::asio::io_service& io_service,
        const boost::asio::ip::tcp::resolver::results_type& ENDPOINTS);
        ~NetworkClient();

        void connect(const boost::asio::ip::tcp::resolver::results_type& ENDPOINTS);
        void send(
            std::unique_ptr<DataProtocol::Transmission> transmission
            );
        void close();

        boost::asio::io_context& getIoContext();
    };
}