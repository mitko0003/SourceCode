#ifndef INTROSORT_H
#define INTROSORT_H

#include <algorithm>
#include <iostream>
#include "Heapsort.h"

using std::pair;

namespace 
{ // Lets admit it, we prefer to hide these functions
	
	// Median of three	 
	// Avoids worst case scenario
	template <class randomit>
	void ChoosePivot(randomit left, randomit right, const unsigned int& size)
	{
		randomit mid = left; 
		advance(mid, size / 2);

		if (*left > *mid)
			swap(*left, *mid);

		if (*mid > *right)
			swap(*mid, *right);

		if (*left > *mid)
			swap(*left, *mid);

		swap(*mid, *right);
	}

	// Bentley-mcilroy 3-way partitioning
	// Better perfomence with equal elements
	template <class randomit>
	pair<randomit, randomit> Partition(randomit left, randomit right, const unsigned int& size COMMA C_S( unsigned long& steps))
	{
		--right; // Pointing Iterator to the last element
		ChoosePivot(left, right, size);

		randomit lPtr = left - 1, rPtr = right, lPivots = lPtr, rPivots = right;

		while (lPtr <= rPtr)
		{
			while (*(++lPtr) < *right && lPtr <= rPtr) C_S(steps++);
			if (lPtr > rPtr) break;
			while (*(--rPtr) > *right && lPtr <= rPtr) C_S(steps++);
			if (lPtr > rPtr) break;

			swap(*lPtr, *rPtr);

			if (*lPtr == *right)
				swap(*lPtr, *(++lPivots));

			if (*rPtr == *right && rPtr != lPivots) // rPtr != lPivots - after 5 hours of debugging found it is inevitable
				swap(*rPtr, *(--rPivots));
		}

		while (lPivots >= left)
			swap(*(lPivots--), *(rPtr--));
		while (rPivots <= right)
			swap(*(rPivots++), *(lPtr++));

		return std::pair<randomit, randomit>(++rPtr, lPtr);
	}

}

namespace practicum_project
{
	template <class randomit>
	RETURN_TYPE Introsort(randomit left, randomit right, unsigned int const& qsortdepth)
	{
		unsigned int size = distance(left, right);
		C_S(unsigned long steps = 0;)

		if (size <= 1) return C_S(steps);
		else if (size > qsortdepth)
		{
			pair<randomit, randomit> pivots = Partition(left, right, size COMMA C_S((steps))); // Two pivots, all equal elements go in between them
			C_S(steps += )Introsort(left, pivots.first, qsortdepth);
			C_S(steps += )Introsort(pivots.second, right, qsortdepth);
		}
		else
			C_S(steps += )Heapsort(left, right);
		return C_S(steps);
	}
}

#endif
