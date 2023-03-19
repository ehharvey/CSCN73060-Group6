#include "../../DataProtocol/DataProtocol.h"
#include "../CsvReader/CsvReader.h"
#include "../NetworkClient/NetworkClient.h"
#include <boost/asio.hpp>
#include <iostream>

namespace Client {
uint_fast64_t flight_id;
uint_fast64_t initialTime;
} // namespace Client

int main(int argc, char *argv[]) {
  try {
    // First argument is IP or hostname
    char *ip = argv[1];

    // Second is flight ID
    Client::flight_id = atoi(argv[2]);

    // Initialize
    Client::CsvReader input(
        "/workspaces/CSCN73060-Group6/FlightLoggingSoftware/Client/"
        "Telem_czba-cykf-pa28-w2_2023_3_1 12_31_27.txt");
    Client::initialTime = 0;

    // Create an io_context object
    boost::asio::io_context io_context;

    // Create a socket object
    boost::asio::ip::tcp::socket socket(io_context);

    // Resolve the localhost address and port number
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve(ip, "1234");

    // Connect to the server
    boost::asio::connect(socket, endpoints);

    while (!input.isAtEnd()) {
      DataProtocol::ClientTransmission *transmission =
          input.getNextTransmission().release();
      std::cout << transmission->getFlightId() << " "
                << transmission->getSecondDelta() << " "
                << transmission->getFuelLevel() << std::endl;

      boost::asio::write(socket,
                         boost::asio::buffer(transmission->getPayload(),
                                             DataProtocol::PACKET_SIZE));
    }

    DataProtocol::ClientTransmission transmission(Client::flight_id, 0, 0);
    boost::asio::write(socket, boost::asio::buffer(transmission.getPayload(),
                                                   DataProtocol::PACKET_SIZE));

    // Close the socket
    socket.shutdown(socket.shutdown_both);
    socket.close();

  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}