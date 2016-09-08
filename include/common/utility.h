#pragma once

#include <limits>

template <typename T>
bool isMaxValue(const T& val)
{
    return val == std::numeric_limits<T>::max();
}

template <typename T>
bool isMinValue(const T& val)
{
    return val == std::numeric_limits<T>::min();
}