#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <malloc.h>
#include <type_traits>

template <typename T>
class Vector {

public:

	explicit Vector(size_t);
	Vector() : mSize(0), mCapacity(0), mData(nullptr) {}
	Vector(const Vector&);
	Vector(Vector&&);

	~Vector() { del(); }

	Vector& operator=(const Vector&);
	Vector& operator=(Vector&&);

	// as standard says: Returns a reference to the element at specified location pos. No bounds checking is performed.
	T& operator [](size_t);
	const T& operator [](size_t) const;

	void push_back(const T&);
	void push_back(T&&);

	void pop_back();

	void clear();
	void resize(size_t);
	void reserve(size_t);
	void shrink_to_fit();

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

private:

	T* mData;
	size_t mSize;
	size_t mCapacity;
	static_assert(std::is_copy_constructible<T>::value || 
		(std::is_default_constructible<T>::value && std::is_assignable<T, const T&>::value),
		"Vector requires copy constructor or default constructor + assignment operator");

	// Used to create objects depending on constructor-operator combination

	template <bool B>
	class Creator {
	public:
		void operator () (T* mem, const T& value) {
			::new (mem)T(value);
		}

		void operator () (T* mem, T&& value) {
			::new (mem)T(std::move(value));
		}
	};

	template <>
	class Creator <false> {
	public:
		void operator () (T* mem, const T& value) {
			::new (mem)T();
			*mem = value;
		}

		void operator () (T* mem, T&& value) {
			::new (mem)T();
			*mem = std::move(value);
		}
	};
	
	Creator <std::is_copy_constructible<T>::value> create;

	void copy(const Vector&);
	void del();
	size_t getNextSize() const;
};

template <typename T>
Vector<T>::Vector(size_t count) : mSize(0), mCapacity(count)
{
	mData = (T*)malloc(sizeof(T)*count);
}

template <typename T>
Vector<T>::Vector(Vector&& other) : mSize(other.mSize), mCapacity(other.mCapacity), mData(other.mData)
{
	other.mData = nullptr;
	other.mSize = 0;
	other.mCapacity = 0;
}

template <typename T>
Vector<T>::Vector(const Vector& other) : mSize(other.mSize), mCapacity(other.mSize)
{
	copy(other);
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this != &other) {
		del();
		copy(other);
		mSize = other.mSize;
		mCapacity = other.mSize;
	}
	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other)
{
	if (this != &other) {
		std::swap(mSize, other.mSize);
		std::swap(mCapacity, other.mCapacity);
		std::swap(mData, other.mData);
	}
	return *this;
}

template <typename T>
T& Vector<T>::operator [](size_t pos)
{
	return mData[pos];
}

template <typename T>
const T& Vector<T>::operator [](size_t pos) const
{
	return mData[pos];
}

template <typename T>
void Vector<T>::push_back(const T& value)
{
	if (mSize == mCapacity)
		reserve(getNextSize());
	create(mData + mSize++, value);
}

template <typename T>
void Vector<T>::push_back(T&& value)
{
	if (mSize == mCapacity)
		reserve(getNextSize());
	create(mData + mSize++, std::move(value));
}

template <typename T>
void Vector<T>::pop_back()
{
	if (empty())
		return;
	(mData + --mSize)->~T();
}

template <typename T>
void Vector<T>::resize(size_t count)
{
	if (count < mSize)
		for (size_t i = count; i < mSize; i++)
			(mData + i)->~T();
	else if (count > mSize) {
		reserve(count);
		for (size_t i = mSize; i < count; i++)
			::new (mData + i) T();
	}
	mSize = count;
}

template <typename T>
void Vector<T>::reserve(size_t new_cap)
{
	if (new_cap > mCapacity) {
		T* temp = (T*)malloc(sizeof(T)*new_cap);
		for (size_t i = 0; i < mSize; i++)
		{
			create(temp + i, mData[i]);
			(mData + i)->~T();
		}
		free(mData);
		mCapacity = new_cap;
		mData = temp;
	}
}

template <typename T>
void Vector<T>::shrink_to_fit()
{
	T* temp = (T*)malloc(sizeof(T)*mSize);
	for (size_t i = 0; i < mSize; i++)
	{
		create(temp + i, mData[i]);
		(mData + i)->~T();
	}
	free(mData);
	mCapacity = mSize;
	mData = temp;
}

template <typename T>
size_t Vector<T>::size() const
{
	return mSize;
}

template <typename T>
size_t Vector<T>::capacity() const
{
	return mCapacity;
}

template <typename T>
bool Vector<T>::empty() const
{
	return !static_cast<bool>(mSize);
}

template <typename T>
void Vector<T>::copy(const Vector& other)
{
	mData = (T*)malloc(sizeof(T)*other.mSize);
	for (size_t i = 0; i < other.mSize; i++)
		create(mData + i, other.mData[i]);
}

template <typename T>
void Vector<T>::del()
{
	for (size_t i = 0; i < mSize; ++i)
		(mData + i)->~T();
	free(mData);
}

template <typename T>
size_t Vector<T>::getNextSize() const
{
	return (mCapacity * 3) / 2 + 1;
}

#endif
