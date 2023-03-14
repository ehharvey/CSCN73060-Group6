#include "NetworkClient.h"

#ifdef _FlightLoggingSoftwareDebug
#include <iostream>
#endif

Client::NetworkClient::NetworkClient(
    boost::asio::io_service &io_service,
    const boost::asio::ip::tcp::resolver::results_type &ENDPOINTS)
    : io_service(io_service), socket(io_service) {
  this->connect(ENDPOINTS);
}

Client::NetworkClient::~NetworkClient() {}

void Client::NetworkClient::connect(
    const boost::asio::ip::tcp::resolver::results_type &ENDPOINTS) {
  boost::asio::async_connect(
      this->socket, ENDPOINTS,
      [this](boost::system::error_code error, boost::asio::ip::tcp::endpoint) {
        if (error) {
#ifdef _FlightLoggingSoftwareDebug
          std::cout << "NetworkClient: Error on connect. " << error.message()
                    << std::endl;
#endif
        } else {
#ifdef _FlightLoggingSoftwareDebug
          std::cout << "NetworkClient: Successfully connected" << std::endl;
#endif
        }
      });
}

void Client::NetworkClient::send(
    std::unique_ptr<DataProtocol::Transmission> transmission) {
  // Post a write that sends the transmission over

  DataProtocol::Transmission *_raw = transmission.release();

  boost::asio::async_write(
      this->socket,
      boost::asio::buffer(_raw->getPayload(), DataProtocol::PACKET_SIZE),
// Callback to perform when write concludes
#ifdef _FlightLoggingSoftwareDebug
      [this, &transmission, &_raw](boost::system::error_code error,
                                   std::size_t /* length */)
#else
      [this, &_raw](boost::system::error_code error, std::size_t /* length */)
#endif
      {
        if (error) {
#ifdef _FlightLoggingSoftwareDebug
          std::cout << "NetworkClient: Error on sending data. "
                    << error.message() << std::endl;
#endif
        } else {
#ifdef _FlightLoggingSoftwareDebug
          char *message = new char[DataProtocol::PACKET_SIZE + 1];
          memcpy(message, transmission.get()->getPayload(),
                 DataProtocol::PACKET_SIZE);
          message[DataProtocol::PACKET_SIZE] = '\0';
          std::cout << "NetworkClient: Sent" << message << std::endl;
#endif
        }

        free(_raw);
      });
}

void Client::NetworkClient::close() {
  boost::asio::post(this->io_service, [this]() { this->socket.close(); });
}

boost::asio::io_context &Client::NetworkClient::getIoContext() {
  return this->io_service;
}
