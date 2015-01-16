#ifndef HEAP_H
#define HEAP_H

#include <vector>

namespace data_structures
{
	using namespace std;

	template <typename T>
	class Heap
	{
	public:
		Heap();
		Heap(const T*, const int&);
		void Add(const T&);
		void Remove();
		T FindMax() const;

	private:
		vector<T> mNodes;
		void ShiftUp(int);
		void ShiftDown(const int&);
	};

	template <class T>
	void Heap<T>::ShiftUp(int index)
	{
		int parent = (index - 1) / 2;
		while (index > 0 && mNodes[parent] < mNodes[index]){
			swap(mNodes[parent], mNodes[index]);
			index = parent;
			parent = (parent - 1) / 2;
		}
	}

	template <class T>
	void Heap<T>::ShiftDown(const int& index)
	{
		int left = index * 2 + 1;
		int right = index * 2 + 2;
		int largest = index;
		if (left < mNodes.size() && mNodes[left] > mNodes[index]){
			largest = left;
		}
		if (right < mNodes.size() && mNodes[right] > mNodes[largest]){
			largest = right;
		}
		if (largest != index){
			swap(mNodes[index], mNodes[largest]);
			this->ShiftDown(largest);
		}
	}

	template <class T>
	T Heap<T>::FindMax() const
	{
		return mNodes[0];
	}

	template <class T>
	void Heap<T>::Remove()
	{
		mNodes[0] = mNodes.back();
		mNodes.pop_back();
		this->ShiftDown(0);
	}

	template <class T>
	void  Heap<T>::Add(const T& element)
	{
		mNodes.push_back(element);
		this->ShiftUp(mNodes.size() - 1);
	}

	template <class T>
	Heap<T>::Heap()
	{
	}

	template <class T>
	Heap<T>::Heap(const T* elements, const int& count) : mNodes(vector<T>(elements, elements + count))
	{
		for (int i = count / 2; i >= 0; --i)
		{
			this->ShiftDown(i);
		}
	}
}

#endif