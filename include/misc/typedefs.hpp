#pragma once

#include <memory>

typedef unsigned char byte;

template<typename T>
using ResourcePtr = std::shared_ptr<T>;
