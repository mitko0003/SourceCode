#ifndef BENCHMARK_H
#define BENCHMARK_H

/**
 *Benchmark class which measures elapsed time.
**/
class BenchMark
{

public:

	using Time = double;

	BenchMark();

	void reset();
	Time elapsed() const;

private:

	using clock = std::chrono::high_resolution_clock;
	using second = std::chrono::duration <double, std::ratio<1>>;

	std::chrono::time_point<clock> mBegin;

};

#endif
