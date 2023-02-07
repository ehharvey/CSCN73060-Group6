#include "json.hpp" // Retrieved from https://github.com/nlohmann/json
#include "PerformanceProfiler.h"

using json = nlohmann::json;

namespace performance_profiler {

	LatencyMeasurement::LatencyMeasurement(ID id) : id(id)
	{
		QueryPerformanceCounter(&start_t);
	};

	void LatencyMeasurement::end()
	{
		QueryPerformanceCounter(&end_t);
		// compute the elapsed time
		elapsed_ticks.QuadPart = (end_t.QuadPart - start_t.QuadPart);
		//elapsedTime.QuadPart *= 1000000;
		//elapsedTime.QuadPart /= frequency.QuadPart;
	}

	ID LatencyMeasurement::getId() const
	{
		return id;
	}

	LARGE_INTEGER LatencyMeasurement::getElapsedTicks() const
	{
		return elapsed_ticks;
	}

	LatencyRecorder::LatencyRecorder() 
	{
		QueryPerformanceFrequency(&frequency);
	};

	void LatencyRecorder::add(LatencyMeasurement lm)
	{
		// Constructs a vector if none exists
		this->measurements[lm.getId()].push_back(lm);
	}


	void LatencyRecorder::saveToDisk(std::filesystem::path path)
	{
		// NOTE: Only implemented with MSVC (e.g., Windows). Only works in Windows
		auto const now = std::chrono::current_zone()
			->to_local(std::chrono::system_clock::now());
		std::filesystem::path todays_date_dir = std::filesystem::path(std::format("{:%Y-%m-%d %H.%M.%S}", now));

		for (const auto& map : measurements)
		{
			std::filesystem::path workload_dir = std::filesystem::path(std::to_string(map.first));

			std::filesystem::create_directories(path / todays_date_dir / workload_dir);

			size_t measurement_counter = 0; // Defines the name of measurement file
			for (const auto& latency_measurement : map.second)
			{
				json to_disk;
				to_disk["id"] = latency_measurement.getId();
					
				auto latency_microseconds = (latency_measurement.getElapsedTicks().QuadPart * 1000000) / frequency.QuadPart;

				to_disk["latency"] = latency_microseconds;

				std::filesystem::path filename = std::filesystem::path(std::to_string(measurement_counter) + ".json");
				measurement_counter++;

				std::ofstream output(path / todays_date_dir / workload_dir / filename);
				output << to_disk;
				output.close();
			}
		}
	}
}