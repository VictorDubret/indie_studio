/*
** EPITECH PROJECT, 2018
** Created by victor on 29/05/18
** File description:
** IndieStudtio.cpp
*/

#include <ItemLocker.hpp>
#include <iostream>
#include <vector>
#include <IEntity.hpp>
#include <ThreadPool.hpp>

namespace is
{
	typedef my::ItemLocker<std::vector<std::shared_ptr<IEntity>>> Entity_t;

	typedef my::ItemLocker<my::ThreadPool> ThreadPool_t;

}