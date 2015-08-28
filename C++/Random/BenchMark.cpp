#include <chrono>
#include "BenchMark.h"


BenchMark::BenchMark() : mBegin(clock::now())
{
}

void BenchMark::reset()
{
	mBegin = clock::now();
}

BenchMark::Time BenchMark::elapsed() const
{
	return std::chrono::duration_cast<second>(clock::now() - mBegin).count();
}
