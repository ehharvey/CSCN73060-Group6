#include "../../DataProtocol/DataProtocol.h"
#include "../../LatencyProfiler/LatencyProfiler.h"
#include "../NetworkServer/NetworkServer.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

// #define PROFILE_LATENCY

class FuelAverage {
private:
  uint_fast64_t fuel_start;
  uint_fast64_t fuel_current;
  uint_fast64_t second_delta;
  uint_fast64_t fuel_average;
  std::mutex lock;

public:
  FuelAverage() {}

  FuelAverage(uint_fast64_t fuel_start) : fuel_start(fuel_start) {}

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

  void calculateAverage() {
    lock.lock();
    // Fuel used per hour
    this->fuel_average = ((fuel_start - fuel_current) / second_delta) * 3600;
    lock.unlock();
  }

  uint_fast64_t getAverage() { return fuel_average; }
};

int main(void) {
  std::mutex lock;
  // Flight ID -> FuelAverage
  std::unordered_map<uint_fast64_t, FuelAverage> fuel_averages;
  const std::size_t num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

#ifdef PROFILE_LATENCY
  performance_profiler::LatencyRecorder recorder;
#endif

  // Represents a IO Service that can send/receive data
  // Encapsulates an "event-loop" service architecture
  boost::asio::io_service io_service;

  std::function<void(std::byte *)> processTransmission =
      [&](std::byte *payload) {
        DataProtocol::ClientTransmission transmission =
            DataProtocol::ClientTransmission(payload);

#ifdef PROFILE_LATENCY
        performance_profiler::LatencyMeasurement measurement(workload_ids::SERVER_PROCESS_TRANSMISSION);
#endif
        auto flight_id = transmission.getFlightId();

        lock.lock();
        if (fuel_averages.count(flight_id) > 0) {
          if (transmission.getSecondDelta() == 0) {
            fuel_averages.at(flight_id).calculateAverage();
            std::cout << "Flight ID: " << flight_id << " Fuel Average: "
                      << fuel_averages.at(flight_id).getAverage() << std::endl;
          } else {
            fuel_averages.at(flight_id).setFuelCurrent(
                transmission.getFuelLevel());
            fuel_averages.at(flight_id).setSecondDelta(
                transmission.getSecondDelta());
          }
        }
        // No fuel averages exist yet
        else {
          fuel_averages.emplace(flight_id, transmission.getFuelLevel());
        }
        lock.unlock();

#ifdef PROFILE_LATENCY
        measurement.end();
        recorder.add(measurement);
#endif
      };

  Server::NetworkServer network_server(io_service, 1234, processTransmission);

  for (int i = 0; i < num_threads; i++) {
    threads.push_back(std::thread([&io_service]() { io_service.run(); }));
  }

  for (auto &t : threads) {
    t.join();
  }


#ifdef PROFILE_LATENCY
        recorder.saveToDisk();
#endif

  return 0;
}