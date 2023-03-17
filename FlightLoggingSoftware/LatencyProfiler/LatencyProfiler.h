/*
* This module defines profiling code. In microseconds
* 
* Currently there exists 2 classes:
*   LatencyMeasurement: A class that, when constructed, times latency. Use an object's end() method to 
*		signify when the workload completes
*	
*	LatencyRecorder: A class that tracks LatencyMeasurement objects and can save them to disk as JSONs
* 
*	NOTE: This module has been tested on 64 bit architectures. Might not work on 32bit
*	When saving latencies to disk, LatencyRecorder uses microsecond precision
*/

#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <chrono>


namespace performance_profiler {
	typedef std::chrono::time_point<std::chrono::steady_clock> time_point_t;
	typedef size_t ID;

	class LatencyMeasurement {
	private:
		time_point_t begin;
		time_point_t stop;
		ID id;

	public:
		LatencyMeasurement(ID id);

		void end();

		ID getId() const;

		std::chrono::duration<int64_t, std::nano> getElapsedTicks() const;
	};

	class LatencyRecorder {
	private:
		std::unordered_map<ID, std::vector<LatencyMeasurement>> measurements;

	public:
		LatencyRecorder();

		void add(LatencyMeasurement lm);

		void saveToDisk(std::filesystem::path path = "performance_data/");
	};
}


namespace workload_ids {
	const performance_profiler::ID CLIENT_FILE_READ = 1;
	const performance_profiler::ID CLIENT_ROUND_TRIP = 2;
	const performance_profiler::ID SERVER_DETERMINE_CATEGORY = 3;
	const performance_profiler::ID SERVER_UPDATE_RECEIVED = 4;
	const performance_profiler::ID SERVER_CALCULATE_AVERAGE = 5;
	const performance_profiler::ID SERVER_PROCESS_TRANSMISSION = 6;
}