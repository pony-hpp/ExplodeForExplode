#include "core/rand.hpp"
#include "core/types.hpp"

namespace core
{
std::random_device Rand::_rd;
std::mt19937 Rand::_gen;
std::unique_ptr<core::Logger> Rand::_logger;

void Rand::init() noexcept
{
  const uint kSeed = _rd();
  _gen             = std::mt19937(kSeed);

  _logger = std::make_unique<core::Logger>("Random");
  _logger->info_fmt("Initialized with seed %u.", kSeed);
}

int Rand::next(int min, int max) noexcept
{
  return std::uniform_int_distribution<int>(min, max)(_gen);
}
}
