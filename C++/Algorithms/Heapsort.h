#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <algorithm>

// Because I'm not into the whole count steps concept...
#define COUNT_STEPS 0
#if COUNT_STEPS
	#define C_S(code) code
	#define C_S_R(code)
	#define COMMA ,
	#define RETURN_TYPE unsigned long
#else
	#define C_S(code)
	#define C_S_R(code) code
	#define COMMA
	#define RETURN_TYPE void
#endif

using std::swap;
using std::advance;
using std::distance;

namespace
{	// Lets admit it, we prefer to hide these functions

	template <class randomit>
	RETURN_TYPE ShiftDown(randomit first, randomit last, int index)
	{
		randomit left, right, largest, curr = first;
		advance(curr, index);
		C_S(unsigned long steps = 0;)

		while (true) // Making algorithm iterative
		{
			left = first; right = first;

			advance(left, index * 2 + 1);
			advance(right, index * 2 + 2);
			largest = curr;

			if (left < last && *left > *largest) 
				largest = left;

			if (right < last && *right > *largest) 
				largest = right;

			if (largest != curr){
				index = index * 2 + 1; // fixing index for next iteretion
				if (largest == right)
					++index;
				
				swap(*curr, *largest);
				curr = largest;
				C_S(steps++);
			}
			else
				return C_S(steps);
		}
	}

	template <class randomit>
	RETURN_TYPE Heapify(randomit first, randomit last)
	{
		int size = distance(first, last);
		C_S(unsigned long steps = 0;)

		for (int i = size / 2 - 1; i >= 0; --i)
		{
			C_S(steps += )ShiftDown(first, last, i);
		}

		return C_S(steps);
	}

	template <class randomit>
	RETURN_TYPE Sort(randomit first, randomit last)
	{
		C_S(unsigned long steps = 0;)

		for (; first != last; --last)
		{						
			swap(*first, *last);
			C_S(steps += ShiftDown(first, last, 0);)
		}

		return C_S(steps);
	}

}

namespace practicum_project
{		
	template <class randomit>
	RETURN_TYPE Heapsort(randomit first, randomit last) // inplace heapsort
	{
		C_S(unsigned long steps = 0;)

		C_S(steps += )Heapify(first, last);
		C_S(steps += )Sort(first, --last); // The first element is sorted anyway

		C_S(return steps;)
	}
}

#endif
