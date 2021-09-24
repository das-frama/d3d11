#pragma once

#include <chrono>

class Timer {
public:
	Timer() {
		start = stop = std::chrono::high_resolution_clock::now();
	}

	bool Start() {
		if (running) return false;

		start = std::chrono::high_resolution_clock::now();
		running = true;

		return true;
	}

	bool Stop() {
		if (!running) return false;

		stop = std::chrono::high_resolution_clock::now();
		running = false;

		return true;
	}

	void Restart() {
		running = true;
		start = std::chrono::high_resolution_clock::now();
	}

	double Elapsed() {
		auto elapsed = std::chrono::duration<double, std::milli>(
			(running ? std::chrono::high_resolution_clock::now() : stop) - start);

		return elapsed.count();
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
	bool running = false;
};