#include "Connection.h"

void Connection::handle_read(const boost::system::error_code &error,
                             size_t bytes_transferred) {
  if (!error) {
    // for (std::size_t i = 0; i < bytes_transferred; i++)
    //   {
    //     std::cout << char(this->data_[i]);
    //   }
    callback(this->data_, bytes_transferred);

    socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            boost::bind(&Connection::handle_read, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    // The following code writes back ("echo")
    // boost::asio::async_write(socket_,
    //                          boost::asio::buffer(data_, bytes_transferred),
    //                          boost::bind(&Connection::handle_write, this,
    //                                      boost::asio::placeholders::error));
  } else {
    delete this;
  }
}

Connection::Connection(boost::asio::io_service &io_service, CallbackFunction callback)
    : socket_(io_service),
      callback(callback) {}

boost::asio::ip::tcp::socket &Connection::socket() { return socket_; }

void Connection::start() {
  socket_.async_read_some(
      boost::asio::buffer(data_, max_length),
      boost::bind(&Connection::handle_read, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}
