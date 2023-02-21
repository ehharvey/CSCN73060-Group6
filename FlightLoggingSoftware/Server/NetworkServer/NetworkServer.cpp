#include "NetworkServer.h"
#include "Connection/Connection.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

class Server {
private:
  boost::asio::io_service &io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;

public:
  Server(boost::asio::io_service &io_service, short port)
      : io_service_(io_service),
        acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(), port)) {
    Connection *new_session = new Connection(io_service_);
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&Server::handle_accept, this,
                                       new_session,
                                       boost::asio::placeholders::error));
  }

  void handle_accept(Connection *new_session,
                     const boost::system::error_code &error) {
    if (!error) {
      new_session->start();
      new_session = new Connection(io_service_);
      acceptor_.async_accept(new_session->socket(),
                             boost::bind(&Server::handle_accept, this,
                                         new_session,
                                         boost::asio::placeholders::error));
    } else {
      delete new_session;
    }
  }
};

int main(int argc, char *argv[]) {
  try {

    // Represents a IO Service that can send/receive data
    boost::asio::io_service io_service;
    Server s(io_service, 8080); // io service, port

    // Single-threaded run
    // Have more than 1 thread call this to multithread
    io_service.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

void Server::NetworkServer::stop() {}

Server::NetworkServer::NetworkServer(
    std::function<void(std::unique_ptr<std::byte>)> receive_callback) {}

void Server::NetworkServer::waitUntilStops() {}

Server::NetworkServer::~NetworkServer() {}
