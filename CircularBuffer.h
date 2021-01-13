#pragma once

#include <exception>

struct RetrieveFromEmptyBufferException : std::runtime_error
{
	using std::runtime_error::runtime_error;
};

template<typename ValType, size_t allocatedSize>
class CircularBuffer
{
	size_t begin = 0;
	size_t end = 0;
	bool isFull = false;
	std::array<ValType, allocatedSize> vals;

	void incrementModuloSize(size_t& i) const
	{
		i = (i + 1) % allocatedSize;
	}

public:
	constexpr size_t getAllocatedSize() const
	{
		return allocatedSize;
	}

	size_t getSize() const
	{
		if (isFull)
			return allocatedSize;
		else
			return end >= begin ? end - begin : allocatedSize - begin + end;
	}

	void add(ValType val)
	{
		vals[end] = val;
		
		if (isFull)
			incrementModuloSize(begin);

		incrementModuloSize(end);

		isFull = (begin == end);
	}

	ValType pop()
	{
		if (!isFull && begin == end)
			throw RetrieveFromEmptyBufferException("No element left in circular buffer");

		isFull = false;
		auto &toRet = vals[begin];
		incrementModuloSize(begin);
		return toRet;
	}
};