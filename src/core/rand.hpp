#ifndef EFE_RAND_HPP
#define EFE_RAND_HPP

#include "core/logger.hpp"

#include <memory>
#include <random>

namespace core
{
class Rand final
{
public:
  static void init() noexcept;
  static int next(int min, int max) noexcept;

private:
  static std::random_device _rd;
  static std::mt19937 _gen;
  static std::unique_ptr<core::Logger> _logger;
};
}

#endif
