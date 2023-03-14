#include "NetworkServer.h"

void Server::NetworkServer::handle_accept(
    Connection *new_session, const boost::system::error_code &error) {
  if (!error) {
    new_session->start();

    // Right now, we allocate a new Connection on Heap
    // (including its buffer)
    new_session = new Connection(io_service_, callback);
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&NetworkServer::handle_accept, this,
                                       new_session,
                                       boost::asio::placeholders::error));
  } else {
    delete new_session;
  }
}

Server::NetworkServer::NetworkServer(boost::asio::io_service &io_service,
                                     short port, CallbackFunction callback)
    : io_service_(io_service),
      acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port)),
      callback(callback) {

  // Right now, we allocate a new Connection on Heap
  // (including its buffer)
  Connection *new_session = new Connection(io_service_, callback);
  acceptor_.async_accept(new_session->socket(),
                         boost::bind(&NetworkServer::handle_accept, this,
                                     new_session,
                                     boost::asio::placeholders::error));
}