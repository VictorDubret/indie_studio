/*
** EPITECH PROJECT, 2018
** cpp_indie_studio
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef CPP_INDIE_STUDIO_ITEMLOCKER_HPP
#define CPP_INDIE_STUDIO_ITEMLOCKER_HPP

#include <mutex>

namespace my {
	template <class ItemClass>
	class ItemLocker {
	public:
		// Constructor
		explicit ItemLocker(ItemClass &item): _item(item) {

		}

		// Destructor
		~ItemLocker() = default;

		// Getter

		// Setter

		// Other
		ItemClass *operator->() {
			return &_item;
		}

		ItemClass &get() {
			return _item;
		}

		void lock() {
			_mutex.lock();
		}

		void unlock() {
			_mutex.unlock();
		}

		bool trylock() {
			return _mutex.try_lock();
		}

	protected:
		ItemClass &_item;
		std::mutex _mutex;
	private:
	};
}

#endif //CPP_INDIE_STUDIO_ITEMLOCKER_HPP
