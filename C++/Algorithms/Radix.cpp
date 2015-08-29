#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <map>
#include <bitset>
#include <random>
#include <chrono>
#include "BenchMark.h"

using std::generate;
typedef std::vector<unsigned int> IntVector;
typedef std::map<int, IntVector> Bucket;

BenchMark benchMark = BenchMark();

void radixSort(IntVector& elements)
{
	Bucket buckets;
	for (int i = 0; i < sizeof(int); ++i)
	{
		for (auto &element: elements)
		{
			buckets[(element >> (i << 3)) & 0xFF].push_back(element);
			
		}
		elements.clear();

		for (auto &bucket : buckets)
		{
			elements.insert(elements.end(), (bucket.second).begin(), (bucket.second).end());
			bucket.second.clear();
		}
	}
}

void main()
{
	IntVector randoms(20000); 
	generate(randoms.begin(), randoms.end(), std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()));
	benchMark.reset();
	//std::sort(randoms.begin(), randoms.end());
	radixSort(randoms);
	std::cout << "time: " << benchMark.elapsed() << std::endl;
}
