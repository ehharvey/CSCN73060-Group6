#pragma once
#include "Connection/Connection.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

#include <cstddef>
#include <functional>
#include <memory>

namespace Server {

typedef std::function<void(std::byte*)> CallbackFunction;

class NetworkServer {
private:
  boost::asio::io_service &io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  CallbackFunction callback;

public:
  NetworkServer(boost::asio::io_service &io_service, short port,
    CallbackFunction callback);

  void handle_accept(Connection *new_session,
                     const boost::system::error_code &error);

};

} // namespace Server