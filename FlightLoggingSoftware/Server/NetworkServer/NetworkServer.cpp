#include "NetworkServer.h"
#include "Connection/Connection.h"

// void Server::NetworkServer::stop() {}

// Server::NetworkServer::NetworkServer(
//     std::function<void(std::unique_ptr<std::byte>)> receive_callback) {}

// void Server::NetworkServer::waitUntilStops() {}

// Server::NetworkServer::~NetworkServer() {}

Server::NetworkServer::NetworkServer(boost::asio::io_service &io_service,
                                     short port)
    : io_service_(io_service),
      acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port)) {

  Connection *new_session = new Connection(io_service_);
  acceptor_.async_accept(new_session->socket(),
                         boost::bind(&NetworkServer::handle_accept, this,
                                     new_session,
                                     boost::asio::placeholders::error));
}

void Server::NetworkServer::handle_accept(
    Connection *new_session, const boost::system::error_code &error) {
  if (!error) {
    new_session->start();
    new_session = new Connection(io_service_);
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&NetworkServer::handle_accept, this,
                                       new_session,
                                       boost::asio::placeholders::error));
  } else {
    delete new_session;
  }
}


int main(int argc, char *argv[]) {
  try {

    // Represents a IO Service that can send/receive data
    // Encapsulates an "event-loop" service architecture
    boost::asio::io_service io_service;
    Server::NetworkServer s(io_service, 8080); // io service, port

    // Single-threaded run
    // Have more than 1 thread call this to multithread
    io_service.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}