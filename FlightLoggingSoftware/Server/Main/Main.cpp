#include "../../DataProtocol/DataProtocol.h"
#include "../AverageCalculator/AverageCalculator.h"
#include "../DataStore/DataStore.h"
#include "../NetworkServer/NetworkServer.h"
// #include <boost/thread/thread.hpp>
#include <iostream>

int main(void) {

  Server::AverageCalculator average_calculator;
  Server::DataStore data_store;


  // Represents a IO Service that can send/receive data
  // Encapsulates an "event-loop" service architecture
  boost::asio::io_service io_service;
  //Server::NetworkServer s(io_service, 8080); // io service, port

  std::function<void(std::byte*, std::size_t)> test = [&](std::byte* payload, std::size_t size) {
        for (std::size_t i = 0; i < size; i++)
        {
          std::cout << char(payload[i]);
        }

        //  auto parsed = DataProtocol::ClientTransmission(std::move(payload));

        // Post Transmission to JobQueue

        // Notify CalcAvg + Send Response

        // Notify DataStore
      };

  Server::NetworkServer network_server(
      io_service,
      1234,
      // This lambda defines what to do whenever the server receives a packet
      // This NEEDS TO BE THREAD SAFE
      test);

   // Single-threaded run
  // Have more than 1 thread call this to multithread
  io_service.run();

  return 0;
}