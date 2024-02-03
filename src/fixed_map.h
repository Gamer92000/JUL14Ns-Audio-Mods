#pragma once

#include <vector>
#include <exception>
#include <mutex>

template <class K, class V> class FixedSizeMap {
private:
	std::tuple<K, V>* _data;
	int _size;
	bool* _occupied;
	std::mutex _insertion_mutex;

public:
	FixedSizeMap(int size) {
		_size = size;
		_occupied = (bool *) malloc(size * sizeof(bool));
		for (int i = 0; i < size; i++) _occupied[i] = false;
		_data = (std::tuple<K, V>*) malloc(size * sizeof(V));
	}

	V get_or_init(K key, std::function<V()> init) {
		for (int i = 0; i < _size; i++) {
			if (_occupied[i] && std::get<0>(_data[i]) == key) {
				return std::get<1>(_data[i]);
			}
		}
		std::lock_guard<std::mutex> guard(_insertion_mutex);
		// ensure the requested element was created while awaiting the lock
		for (int i = 0; i < _size; i++) {
			if (_occupied[i] && std::get<0>(_data[i]) == key) {
				return std::get<1>(_data[i]);
			}
		}
		// find open slot in map
		int slot = 0;
		for (; slot < _size && _occupied[slot]; slot++);
		if (slot == _size) {
			throw std::runtime_error("Map is full!");
		}
		V entry = init();
		_occupied[slot] = true;
		_data[slot] = std::tuple<K, V>{ key, entry };
		return entry;
	}

// WARNING!!!
// The remove methods here are highly evil!
// As the get method does not acquire the lock, it might read from illegal memory
// Assume thread A reading and thread B removing at the same time:
// 
//             A                           B
// 
// _occupied[i] => true
//                             _occupied[i] = false
//                             _data[slot] = nullptr
// _data[slot] => nullptr
// 
// Thus thread A tries to access a tuple at an unexpected pointer!

	void remove(std::function<void(V)> deinit) {
		std::lock_guard<std::mutex> guard(_insertion_mutex);
		for (int i = 0; i < _size; i++) {
			if (_occupied[i]) {
				_occupied[i] = false;
				deinit(std::get<1>(_data[i]));
			}
		}
	}

	void remove(K key, std::function<void(V)> deinit) {
		std::lock_guard<std::mutex> guard(_insertion_mutex);
		for (int i = 0; i < _size; i++) {
			if (_occupied[i] && std::get<0>(_data[i]) == key) {
				_occupied[i] = false;
				deinit(std::get<1>(_data[i]));
				return;
			}
		}
		throw std::invalid_argument("The provided key does not exist in the map!");
	}
};