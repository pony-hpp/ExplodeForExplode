#include "core/rand.hpp"

#include <random>

int core::rand(int min, int max) noexcept
{
  static std::random_device rd;
  static std::mt19937 mt(rd());
  return std::uniform_int_distribution<int>(min, max)(mt);
}
