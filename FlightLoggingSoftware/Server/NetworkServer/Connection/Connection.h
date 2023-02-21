#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <iostream>

class Connection {
private:
  boost::asio::ip::tcp::socket socket_;

  enum { max_length = 1024 };
  char data_[max_length];

  void handle_read(const boost::system::error_code &error,
                   size_t bytes_transferred);

  void handle_write(const boost::system::error_code &error);

public:
  Connection(boost::asio::io_service &io_service);

  boost::asio::ip::tcp::socket &socket();

  void start();
};