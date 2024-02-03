#pragma once
#include <functional>

#define mod(a, b) (a%b+b)%b

template <class T> class RingBuffer {
	T* data;
	std::size_t head;
	std::size_t max;
	std::size_t _size;

public:
	RingBuffer(std::size_t _max) {
		max = _max;
		data = (T*)malloc(_max * sizeof(T));
		head = 0;
		_size = 0;
	}

	~RingBuffer() {
		free(data);
	}

	void push(T entry) {
		data[head] = entry;
		head = (head + 1) % max;
		if (_size != max) ++_size;
	}

	// This function is only somewhat threadsafe.
	// It will not segfault, but there is a potential data race!
	// This can be accepted here however, given how the data will be processed
	T accumulate(T initial, std::function<T(T, std::size_t, T)> accumulator) {
		T result = initial;
		std::size_t fixed_head = head;
		for (int i = 0; i < _size; i++) {
			result = accumulator(result, i, data[mod((fixed_head - 1 - i), max)]);
		}
		return result;
	}

	std::size_t size() {
		return _size;
	}
};