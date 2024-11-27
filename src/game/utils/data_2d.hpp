#ifndef EFE_DATA_2D_HPP
#define EFE_DATA_2D_HPP

#include "core/types.hpp"

#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace game::utils
{
// This class represents data of any type in a 2D storage with access by linear
// index or position. Adds data from the bottom left corner to the top right
// corner.
// But this class can store more data than its dimensions - this data is extra.
// If your `push` function call can push an extra element, you should set its
// position in the 2nd parameter to get it via `at` in the future.
template <typename T>
class Data2D
{
public:
  using Pos = std::pair<int, int>;

  explicit Data2D(uint w, uint h, uint extraDataSize = 0) noexcept;

  // Extra blocks aren't supported when Data2D is created from vector.
  explicit Data2D(const std::vector<T> &vec, uint w, uint h) noexcept;

  inline uint w() const noexcept { return _w; }
  inline uint h() const noexcept { return _h; }
  inline uint extra_data_size() const noexcept { return _kExtraDataSize; }
  inline ullong dimensions_size() const noexcept { return _w * _h; }
  inline ullong size() const noexcept { return _size; }
  ullong idx(int x, int y) const;

  const T *at(ullong idx) const noexcept;
  T *at(ullong idx) noexcept;
  const T *at(int x, int y) const noexcept;
  T *at(int x, int y) noexcept;

  void push(const T &elem) noexcept;
  void push(T &&elem) noexcept;
  void push(const T &elem, const Pos &extraPos) noexcept;
  void push(T &&elem, const Pos &extraPos) noexcept;

private:
  const uint _w, _h, _kExtraDataSize;
  std::unique_ptr<T[]> _data;
  std::map<Pos, ullong> _extraElemIndices;
  ullong _size, _filled = 0;

  void _basic_extra_push(T elem, const Pos &extraPos) noexcept;
};

struct NonexistentExtraElementException
{};
}

#ifndef EFE_DATA_2D_IMPL_HPP
#include "game/utils/data_2d_impl.hpp" // IWYU pragma: keep
#endif

#endif
