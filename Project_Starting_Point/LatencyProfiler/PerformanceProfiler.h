/*
* This module defines profiling code.
* 
* Currently there exists 2 classes:
*   LatencyMeasurement: A class that, when constructed, times latency. Use an object's end() method to 
*		signify when the workload completes
*	
*	LatencyRecorder: A class that
*/

#pragma once
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <chrono>


namespace performance_profiler {
	typedef size_t ID;

	class LatencyMeasurement {
	private:
		LARGE_INTEGER frequency;
		LARGE_INTEGER start_t{ 0 }, end_t{ 0 }, elapsed_time{ 0 };
		ID id;

	public:
		LatencyMeasurement(ID id);

		void end();

		ID getId() const;

		LARGE_INTEGER getElapsedTime() const;
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
	const performance_profiler::ID WORKLOAD_ONE = 1;
	const performance_profiler::ID WORKLOAD_FIVE = 5;
}