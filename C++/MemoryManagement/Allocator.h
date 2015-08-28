/*
	Name: Dimitar Trendafilov
	FN: 80976
*/

#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <memory>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <array>
#define NOMINMAX
#include <Windows.h>

namespace MMProject {

	const uint8_t SizeClasses = 5;
	const uint8_t Alignment = 8;

	template <
		uint8_t sizeClasses = SizeClasses
	>
	class Allocator {
	public:

		Allocator();
		Allocator(const Allocator&);
		Allocator(Allocator&&);
		Allocator& operator=(const Allocator&) = delete;

		void* MyMalloc(const size_t&);
		void MyFree(void*);
		void Dump();

	private:

		struct BoundaryTag;
		struct SharedData;

		std::shared_ptr<SharedData> mSharedData;
		HANDLE &mHeap;
		uint8_t *& mMemory;
		std::array<BoundaryTag*, sizeClasses> &mFree;

		BoundaryTag* FindBuddy(BoundaryTag* tag) const {
			uint32_t relativeAddr = (uint32_t) tag - (uint32_t) mMemory;
			uint32_t buddyRelativeAddr = relativeAddr ^ tag->size;
			BoundaryTag* buddy = reinterpret_cast<BoundaryTag*>(mMemory + buddyRelativeAddr);
			return buddy->used ? nullptr : buddy;
		}

		BoundaryTag* GetMemory() {
			static uint32_t alignment = 0;
			SIZE_T size = SMaxSize;

			mMemory -= alignment; // HeapAlloc does not give any promises about alignment
			if (!mMemory) {
				mMemory = static_cast<uint8_t*>(HeapAlloc(mHeap, NULL, SMaxSize));
				alignment = (uint32_t)mMemory % Alignment;
			}
			else {
				HeapFree(mHeap, NULL, HeapAlloc(mHeap, NULL, SMaxSize)); // HeapReAlloc if it has to commit memory
																		 // that way I know there is enough commited memory
				size += HeapSize(mHeap, NULL, (LPVOID) mMemory);
				HeapReAlloc(mHeap, HEAP_REALLOC_IN_PLACE_ONLY, (LPVOID) mMemory, size);
			}
			mMemory += alignment;

			if (!mMemory || HeapSize(mHeap, NULL, (LPVOID) mMemory) != size)
				return nullptr;

			BoundaryTag* tag = reinterpret_cast<BoundaryTag*>(mMemory + (size - SMaxSize));
			tag->used = false;
			tag->size = SMaxSize;
			tag->next = nullptr;
			tag->prev = nullptr;

			mFree[sizeClasses - 1] = tag;

			return tag;
		}

		BoundaryTag* Coalesce(BoundaryTag* buddyFree, BoundaryTag* buddyFreed) {
			uint8_t level = Level(buddyFree->size) - SMinLevel;
			BoundaryTag* result = buddyFreed;

			if (level < sizeClasses - 1) {
				Erase(buddyFree, level);
				result = std::min(buddyFree, buddyFreed);
				result->size <<= 1;
			}

			return result;
		}

		BoundaryTag* SplitToSize(BoundaryTag* buddy, const uint32_t& size) {
			uint8_t level = Level(buddy->size) - SMinLevel;

			mFree[level] = buddy->next;
			if (mFree[level]) mFree[level]->prev = nullptr;

			while (buddy->size != size) {
				level--;
				buddy->size >>= 1;

				BoundaryTag* rightBuddy = reinterpret_cast<BoundaryTag*>((uint8_t*)buddy + buddy->size);
				rightBuddy->used = false; 
				rightBuddy->size = buddy->size; 
				rightBuddy->next = mFree[level];
				rightBuddy->prev = nullptr;

				if (mFree[level]) mFree[level]->prev = rightBuddy;
				mFree[level] = rightBuddy;
			}

			buddy->used = true;
			return buddy;
		}

		// Free List Operations
		void Erase(BoundaryTag* node, const uint8_t& level) {
			if (mFree[level] == node) mFree[level] = node->next;

			if (node->next)
				node->next->prev = node->prev;
			if (node->prev)
				node->prev->next = node->next;
		}
		
		void PushFront(BoundaryTag* node, const uint8_t& level) {
			node->prev = nullptr;
			node->next = mFree[level];

			if (mFree[level]) mFree[level]->prev = node;
			mFree[level] = node;
		}

		static size_t Level(size_t);
		static uint8_t SMinLevel; // Visual studio does not support constexpr
		static size_t SMaxSize;

		struct BoundaryTag {
			uint32_t used : 1;
			uint32_t size : 31;
			BoundaryTag* next;
			union {
				uint8_t data[1];
				BoundaryTag* prev;
			};
		};

		struct SharedData {
			SharedData() : mMemory(nullptr) {
				std::fill(mFree.begin(), mFree.end(), nullptr);
			}

			~SharedData() {
				HeapDestroy(mHeap);
			}

			HANDLE mHeap;
			uint8_t* mMemory = nullptr;
			std::array<BoundaryTag*, sizeClasses> mFree;
		};
	};

	template <uint8_t sizeClasses>
	uint8_t Allocator<sizeClasses>::SMinLevel = Level(std::max<uint8_t>(Alignment, sizeof(BoundaryTag)));

	template <uint8_t sizeClasses>
	size_t Allocator<sizeClasses>::SMaxSize = 1 << (sizeClasses - 1 + SMinLevel);

	template <uint8_t sizeClasses>
	Allocator<sizeClasses>::Allocator() :
		mSharedData(std::make_shared<SharedData>()),
		mHeap(mSharedData->mHeap),
		mMemory(mSharedData->mMemory),
		mFree(mSharedData->mFree) 
	{
		mHeap = HeapCreate(NULL, 0, 0);
	}

	template <uint8_t sizeClasses>
	Allocator<sizeClasses>::Allocator(const Allocator& other) :
		mSharedData(other.mSharedData),
		mHeap(mSharedData->mHeap),
		mMemory(mSharedData->mMemory),
		mFree(mSharedData->mFree) {}

	template <uint8_t sizeClasses>
	Allocator<sizeClasses>::Allocator(Allocator&& other) :
		mSharedData(std::move(other.mSharedData)),
		mHeap(mSharedData->mHeap),
		mMemory(mSharedData->mMemory),
		mFree(mSharedData->mFree) {}

	template <uint8_t sizeClasses>
	void* Allocator<sizeClasses>::MyMalloc(const size_t& size) {
		size_t total = size + Alignment;
		if (total > SMaxSize || !size) return nullptr;

		uint8_t level = Level(total);
		total = 1 << level;
		level = level < SMinLevel ? 0 : level - SMinLevel;

		if (BoundaryTag* freeMemory = mFree[level]) {
			mFree[level] = freeMemory->next;
			mFree[level] = freeMemory->prev;
			freeMemory->used = true;
			return freeMemory->data;
		}
		else {
			for (int freeLevel = level + 1; freeLevel < sizeClasses; ++freeLevel) {
				if (mFree[freeLevel]) {
					return SplitToSize(mFree[freeLevel], total)->data;
				}
			}

			if (GetMemory())
				return SplitToSize(mFree[sizeClasses - 1], total)->data;
		}

		return nullptr;
	}
	
	template <uint8_t sizeClasses>
	void Allocator<sizeClasses>::MyFree(void* ptr) {
		BoundaryTag* tag = reinterpret_cast<BoundaryTag*>((uint8_t*) ptr - Alignment);
		tag->used = false;

		while (BoundaryTag* buddy = FindBuddy(tag)) {
			tag = Coalesce(buddy, tag);
			if (tag->size == SMaxSize) break;
		}
		PushFront(tag, Level(tag->size) - SMinLevel);
	}

	template <uint8_t sizeClasses>
	void Allocator<sizeClasses>::Dump() {
		static auto printTag = [](BoundaryTag* tag) {
			std::cout << "{"
				<< "used: " << tag->used
				<< ", size: " << tag->size
				<< ", next: " << (LPVOID) tag->next
				<< ", prev: " << (LPVOID) tag->prev
				<< "} ";
		};

		std::cout << "Mininimum Allocation Size: " << (1 << SMinLevel) << endl;
		std::cout << "Maximum Allocation Size: " << SMaxSize << endl;
		std::cout << "Heap Handle: " << mHeap << endl;
		std::cout << "Memory Address: " << (LPVOID)mMemory << endl;

		std::for_each(
			mFree.begin(),
			mFree.end(),
			[](BoundaryTag* tag) {
				while (tag) {
					printTag(tag);
					tag = tag->next;
				}
				std::cout << endl;
			});
		std::cout << endl;
	}

	template <uint8_t sizeClasses>
	size_t Allocator<sizeClasses>::Level(size_t s)
	{
		int l;
		__asm
		{
			mov eax, [s]
			bsr eax, eax
			mov[l], eax
		}
		return l + ((1 << l) != s);
	}

	//// Trying a trick from Alexandrescu Modern C++ Design

	//namespace {
	//	// Symplified SingletonHolder from Alexandrescu Modern C++ Design
	//	template<class T>
	//	class SingletonHolder
	//	{
	//	public:
	//		static T& Instance();
	//		SingletonHolder() = delete;
	//	private:
	//		static std::unique_ptr<T> sInstance;
	//	};

	//	template<class T>
	//	T& SingletonHolder<T>::Instance() {
	//		if (!sInstance)
	//		{
	//			sInstance = std::make_unique<T>();
	//		}
	//		return *sInstance;
	//	}
	//}

	//const uint32_t DefaultSizeClasses = 9;
	//typedef SingletonHolder < Allocator<DefaultSizeClasses> > DefaultAllocator;
	//template <
	//	typename allocator = SingletonHolder<Allocator<DefaultSizeClasses>>
	//>
	//class CustomAllocated {
	//public:
	//	virtual ~CustomAllocated() {}
	//	void* operator new(std::size_t size)
	//	{
	//		std::cout << "create" << std::endl;
	//		return allocator::Instance().MyMalloc(size);
	//	}

	//	void operator delete(void* p)
	//	{
	//		std::cout << "delete" << std::endl;
	//		allocator::Instance().MyFree(p);
	//	}
	//};
}

#endif
