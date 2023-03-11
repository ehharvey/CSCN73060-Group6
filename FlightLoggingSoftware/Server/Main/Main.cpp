#include "../../DataProtocol/DataProtocol.h"
#include "../NetworkServer/NetworkServer.h"
#include <iostream>

int main(void) {

  // Represents a IO Service that can send/receive data
  // Encapsulates an "event-loop" service architecture
  boost::asio::io_service io_service;
  //Server::NetworkServer s(io_service, 8080); // io service, port

  std::function<void(std::byte*)> test = [&](std::byte* payload) {
        for (std::size_t i = 0; i < DataProtocol::PACKET_SIZE; i++)
        {
          std::cout << char(payload[i]);
        }
      };

  Server::NetworkServer network_server(
      io_service,
      1234,
      test);

  // Single-threaded run
  // Have more than 1 thread call this to multithread
  io_service.run();

  return 0;
}