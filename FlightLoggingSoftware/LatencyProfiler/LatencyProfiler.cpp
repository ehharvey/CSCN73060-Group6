#include "LatencyProfiler.h"
#include "json.hpp" // Retrieved from https://github.com/nlohmann/json
#include <sstream>

using json = nlohmann::json;

namespace performance_profiler {

LatencyMeasurement::LatencyMeasurement(ID id)
    : id(id), begin(std::chrono::steady_clock::now()){};

void LatencyMeasurement::end() {
  this->stop = std::chrono::steady_clock::now();
}

ID LatencyMeasurement::getId() const { return id; }

std::chrono::duration<int64_t, std::nano>
LatencyMeasurement::getElapsedTicks() const {
  return begin - stop;
}

LatencyRecorder::LatencyRecorder(){};

void LatencyRecorder::add(LatencyMeasurement lm) {
  this->m.lock();
  // Constructs a vector if none exists
  this->measurements[lm.getId()].push_back(lm);
  this->m.unlock();
}

void LatencyRecorder::saveToDisk(std::filesystem::path path) {
  // NOTE: Only implemented with MSVC (e.g., Windows). Only works in Windows
  auto const now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream time_sstream;
	
	time_sstream << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H.%M.%S");

  std::filesystem::path todays_date_dir =
      std::filesystem::path(time_sstream.str());

  for (const auto &map : measurements) {
    std::filesystem::path workload_dir =
        std::filesystem::path(std::to_string(map.first));

    std::filesystem::create_directories(path / todays_date_dir / workload_dir);

    size_t measurement_counter = 0; // Defines the name of measurement file
    for (const auto &latency_measurement : map.second) {
      json to_disk;
      to_disk["id"] = latency_measurement.getId();

      to_disk["latency"] = latency_measurement.getElapsedTicks().count();

      std::filesystem::path filename =
          std::filesystem::path(std::to_string(measurement_counter) + ".json");
      measurement_counter++;

      std::ofstream output(path / todays_date_dir / workload_dir / filename);
      output << to_disk;
      output.close();
    }
  }
}
} // namespace performance_profiler