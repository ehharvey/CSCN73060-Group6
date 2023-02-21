#include "../../DataProtocol/DataProtocol.h"
#include "../AverageCalculator/AverageCalculator.h"
#include "../DataStore/DataStore.h"
#include "../NetworkServer/NetworkServer.h"
#include <boost/thread/thread.hpp>
#include <iostream>

int main(void) {
  boost::thread_group server_thread_pool;
  Server::AverageCalculator average_calculator;
  Server::DataStore data_store;

  Server::NetworkServer network_server(
      // This lambda defines what to do whenever the server receives a packet
      // This NEEDS TO BE THREAD SAFE
      [&](std::unique_ptr<std::byte> payload) {
        auto parsed = DataProtocol::ClientTransmission(std::move(payload));

        // Post Transmission to JobQueue

        // Notify CalcAvg + Send Response

        // Notify DataStore
      });

  network_server.waitUntilStops();

  return 0;
}