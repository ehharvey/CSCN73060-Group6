#include "../../DataProtocol/DataProtocol.h"
#include "../../LatencyProfiler/LatencyProfiler.h"
#include "../NetworkServer/NetworkServer.h"
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <signal.h>
#include <thread>

#include <unordered_map>
#include <condition_variable>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

// #define PROFILE_LATENCY

#ifdef PROFILE_LATENCY
performance_profiler::LatencyRecorder recorder;
#endif

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

void saveProfilingData() {
  // Saving Profile Data --> make sure to have profile data global
  std::cout << "Saving Profiling Data" << std::endl;
#ifdef PROFILE_LATENCY
  recorder.saveToDisk();
#endif
}

void signal_callback_handler(int signum) {
  std::cout << "Saving Profiling Data before ending program... " << signum
            << std::endl;
  // Saving Profile Data
  saveProfilingData();
  // Terminate program
  std::cout << "Successfully saved Profiling Data. Ending program." << signum
            << std::endl;
  exit(signum);
}

void print_transmission(DataProtocol::ClientTransmission t)
{
  static std::mutex m;

  m.lock();
  std::cout << "-----------------------------------------------" \
            << std::endl \
            << "   FLIGHT ID: " << t.getFlightId() << std::endl \
            << "SECOND DELTA: " << t.getSecondDelta() << std::endl \
            << "  FUEL LEVEL: " << t.getFuelLevel() << std::endl \
            << "-----------------------------------------------" \
            << std::endl;
  m.unlock();
}

int main(void) {
  // Register signal and signal handler
  signal(SIGINT, signal_callback_handler);
  std::mutex m;
  std::condition_variable condition;
  // Flight ID -> FuelAverage
  std::unordered_map<uint_fast64_t, FuelAverage> fuel_averages;
  const std::size_t num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  // Represents a IO Service that can send/receive data
  // Encapsulates an "event-loop" service architecture
  boost::asio::io_service io_service;

  std::function<void(std::byte *)> processTransmission =
      [&](std::byte *payload) {
        DataProtocol::ClientTransmission transmission =
            DataProtocol::ClientTransmission(payload);
#ifdef PROFILE_LATENCY
        performance_profiler::LatencyMeasurement measurement(
            workload_ids::SERVER_PROCESS_TRANSMISSION);
#endif
        auto flight_id = transmission.getFlightId();
        bool created_entry = false;

        if (fuel_averages.count(flight_id) <= 0)
        {
          std::unique_lock lock(m);
          if (fuel_averages.count(flight_id) <= 0)
          {
            if (transmission.getSecondDelta() == 0)
            {
              fuel_averages.emplace(flight_id, transmission.getFuelLevel());
              created_entry = true;
              lock.unlock();
              condition.notify_all();
            }
            else
            {
              condition.wait(lock, [&] { return fuel_averages.count(flight_id) <= 0; });
              lock.unlock();
            }
          }
          else
          {
            lock.unlock();
          }
        }

        // ENTRY SHOULD EXIST BY NOW

        if ((transmission.getSecondDelta()) == 0 && (!created_entry)) {
          fuel_averages.at(flight_id).calculateAverage();
          std::cout << "Flight ID: " << flight_id << " Fuel Average: "
                    << fuel_averages.at(flight_id).getAverage() << std::endl;
        } else {
          fuel_averages.at(flight_id).setFuelCurrent(
              transmission.getFuelLevel());
          fuel_averages.at(flight_id).setSecondDelta(
              transmission.getSecondDelta());
        }

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