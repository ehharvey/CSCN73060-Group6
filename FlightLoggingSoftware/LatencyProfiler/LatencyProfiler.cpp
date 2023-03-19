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
LatencyMeasurement::getElapsedNanoSeconds() const {
  return stop - begin;
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

  for (const auto &measurements_dict_item : measurements) {
    std::filesystem::path workload_file =
        std::filesystem::path(std::to_string(measurements_dict_item.first) + ".json");

    std::filesystem::create_directories(path / todays_date_dir);

    json j;
    j["id"] = measurements_dict_item.first;

    std::vector<int64_t> nanosecond_measurements;

    for (auto& latency_measurement : measurements_dict_item.second)
    {
      nanosecond_measurements.push_back(latency_measurement.getElapsedNanoSeconds().count());
    }

    j["measurements"] = nanosecond_measurements;

    std::ofstream output(path / todays_date_dir / workload_file );
    output << j;
    output.close();
  }
}
} // namespace performance_profiler