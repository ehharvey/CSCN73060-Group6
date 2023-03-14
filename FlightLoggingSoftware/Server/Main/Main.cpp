#include "../../DataProtocol/DataProtocol.h"
#include "../NetworkServer/NetworkServer.h"
#include <mutex>
#include <unordered_map>
#include <iostream>

class FuelAverage {
  private:
  uint_fast64_t fuel_start;
  uint_fast64_t fuel_current;
  uint_fast64_t second_delta;
  uint_fast64_t fuel_average;
  std::mutex lock;

  public:
  FuelAverage() { }

  FuelAverage(uint_fast64_t fuel_start)
  : fuel_start(fuel_start) { }

  void setFuelCurrent(uint_fast64_t fuel_current) {
    lock.lock();
    this->fuel_current = fuel_current;
    lock.unlock();
  }

  void setSecondDelta(uint_fast64_t second_delta) {
    lock.lock();
    this->second_delta = second_delta;
    lock.unlock();
  }

  void calculateAverage()
  {
    lock.lock();
    this->fuel_average = (fuel_start - fuel_current) / second_delta;
    lock.unlock();
  }
};

int main(void) {
  std::mutex lock;
  std::unordered_map<uint_fast64_t, FuelAverage> fuel_averages;

  // Represents a IO Service that can send/receive data
  // Encapsulates an "event-loop" service architecture
  boost::asio::io_service io_service;
  //Server::NetworkServer s(io_service, 8080); // io service, port

  std::function<void(std::byte*)> test = [&](std::byte* payload) {
        for (std::size_t i = 0; i < DataProtocol::PACKET_SIZE; i++)
        {
          // for (int i = 0; i < DataProtocol::PACKET_SIZE; i++)
          // {
          //   std::cout << char(payload[i]);
          //   std::cout << std::endl;
          // }

          DataProtocol::ClientTransmission transmission = DataProtocol::ClientTransmission(payload);
          auto flight_id = transmission.getFlightId();

          lock.lock();
          if (fuel_averages.count(flight_id) > 0)
          {
            fuel_averages.at(flight_id).setFuelCurrent(transmission.getFuelLevel());
            fuel_averages.at(flight_id).setSecondDelta(transmission.getSecondDelta());
          }
          else
          {
            if (transmission.getSecondDelta() == 0) {
              io_service.stop();
            }
            else {
              fuel_averages.emplace(flight_id, transmission.getFuelLevel());
            }
          }
          lock.unlock();
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