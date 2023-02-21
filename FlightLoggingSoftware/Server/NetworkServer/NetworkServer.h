#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

#include <cstddef>
#include <functional>
#include <memory>

namespace Server {

class NetworkServer {
private:
  boost::asio::io_service &io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;

  void handle_accept(Connection *new_session,
                     const boost::system::error_code &error);

public:
  NetworkServer(boost::asio::io_service &io_service, short port);
};

} // namespace Server