#include "Connection.h"

void Connection::handle_read(const boost::system::error_code &error,
                             size_t bytes_transferred) {
  if (!error) {
    callback(this->data_);

    socket_.write_some(
      boost::asio::buffer(DataProtocol::ACK, sizeof(DataProtocol::ACK))
    );

    socket_.async_read_some(
            boost::asio::buffer(data_, DataProtocol::PACKET_SIZE),
            boost::bind(&Connection::handle_read, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
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
      boost::asio::buffer(data_, DataProtocol::PACKET_SIZE),
      boost::bind(&Connection::handle_read, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}
