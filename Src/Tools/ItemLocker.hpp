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
		~ItemLocker();

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

		std::recursive_mutex &getMutex() {
			return _mutex;
		}

	protected:
		ItemClass &_item;
		std::recursive_mutex _mutex;
	private:
	};
	template<class ItemClass>
	ItemLocker<ItemClass>::~ItemLocker()
	{
	}
}

#endif //CPP_INDIE_STUDIO_ITEMLOCKER_HPP
