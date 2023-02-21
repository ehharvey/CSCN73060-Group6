#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <iostream>

 // Duplicate of NetworkServer.h
// TODO: Move this typedef into a separate file (away from NetworkServer or Connection)
typedef std::function<void(std::byte*, std::size_t)> CallbackFunction;

class Connection {
private:
  CallbackFunction callback;
  boost::asio::ip::tcp::socket socket_;

  enum { max_length = 1024 };
  std::byte data_[max_length];

  void handle_read(const boost::system::error_code &error,
                   size_t bytes_transferred);

public:
  Connection(boost::asio::io_service &io_service, CallbackFunction callback);

  boost::asio::ip::tcp::socket &socket();

  void start();
};