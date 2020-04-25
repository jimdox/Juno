#pragma once
#include <memory>
/* wrappers for stuff like smart pointers because I'm tired of writing them out all the time :') */
namespace Juno
{
template <typename T>
using SPtr = std::shared_ptr<T>;
template <typename T>
using UPtr = std::unique_ptr<T>;
template<typename T>
using WPtr = std::weak_ptr<T>;
}