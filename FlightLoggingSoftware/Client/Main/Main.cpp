#include "../../DataProtocol/DataProtocol.h"
#include "../NetworkClient/NetworkClient.h"
#include <boost/asio.hpp>
#include <iostream>

int main() {
  try {
    // Create an io_context object
    boost::asio::io_context io_context;

    // Create a socket object
    boost::asio::ip::tcp::socket socket(io_context);

    // Resolve the localhost address and port number
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve("localhost", "1234");

    // Connect to the server
    boost::asio::connect(socket, endpoints);

    uint_fast64_t flight_id = 1;
    uint_fast64_t secondDelta = 0;
    uint_fast64_t fuelLevel = 10000;

    int numberOfTransmissions = 3000;

    for (int i = 0; i < numberOfTransmissions; i++) {
      DataProtocol::ClientTransmission dummy(flight_id, secondDelta, fuelLevel);
      secondDelta++;
      fuelLevel--;

      boost::asio::write(
          socket,
          boost::asio::buffer(dummy.getPayload(), DataProtocol::PACKET_SIZE));
    }

    DataProtocol::ClientTransmission endOfTransmission(flight_id, 0, fuelLevel);

    boost::asio::write(socket, boost::asio::buffer(endOfTransmission.getPayload(),
                                                   DataProtocol::PACKET_SIZE));

    // Close the socket
    socket.close();

  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}