#ifndef EFE_DATA_2D_IMPL_HPP
#define EFE_DATA_2D_IMPL_HPP

#include "game/utils/data_2d.hpp"

#include <stdexcept>

namespace game::utils
{
template <typename T>
Data2D<T>::Data2D(uint w, uint h, uint extraDataSize) noexcept
  : _w(w), _h(h), _kExtraDataSize(extraDataSize)
{
  _size = w * h + extraDataSize;
  _data = std::make_unique<T[]>(_size);
}

template <typename T>
Data2D<T>::Data2D(const std::vector<T> &vec, uint w, uint h) noexcept
  : Data2D(w, h)
{
  for (const T &kElem : vec)
  {
    push(kElem);
  }
}

template <typename T>
ullong Data2D<T>::idx(int x, int y) const
{
  if ((x >= 0 && x < (int)w()) && (y >= 0 && y < (int)h()))
  {
    return y * _w + x;
  }
  else
  {
    try
    {
      return _extraElemIndices.at({x, y});
    }
    catch (const std::out_of_range &)
    {
      throw NonexistentExtraElementException {};
    }
  }
}

template <typename T>
const T *Data2D<T>::at(ullong idx) const noexcept
{
  return idx < _filled ? &_data[idx] : nullptr;
}

template <typename T>
T *Data2D<T>::at(ullong idx) noexcept
{
  // Use it to call `_at() const` and avoid recursion
  const auto kThis = (const Data2D<T> *)this;

  return (T *)kThis->at(idx);
}

template <typename T>
const T *Data2D<T>::at(int x, int y) const noexcept
{
  try
  {
    return at(idx(x, y));
  }
  catch (const NonexistentExtraElementException &)
  {
    return nullptr;
  }
}

template <typename T>
T *Data2D<T>::at(int x, int y) noexcept
{
  try
  {
    return at(idx(x, y));
  }
  catch (const NonexistentExtraElementException &)
  {
    return nullptr;
  }
}

template <typename T>
void Data2D<T>::push(const T &elem) noexcept
{
  _data[_filled++] = elem;
}

template <typename T>
void Data2D<T>::push(T &&elem) noexcept
{
  _data[_filled++] = std::move(elem);
}

template <typename T>
void Data2D<T>::push(const T &elem, const Pos &extraPos) noexcept
{
  _basic_extra_push(elem, extraPos);
}

template <typename T>
void Data2D<T>::push(T &&elem, const Pos &extraPos) noexcept
{
  _basic_extra_push(std::move(elem), extraPos);
}

template <typename T>
void Data2D<T>::_basic_extra_push(T elem, const Pos &extraPos) noexcept
{
  const auto kFoundIdx        = _extraElemIndices.find(extraPos);
  const bool kShouldOverwrite = kFoundIdx != _extraElemIndices.cend();

  if (_filled >= dimensions_size() && !kShouldOverwrite)
  {
    _extraElemIndices.insert_or_assign(extraPos, _filled);
  }

  _data[kShouldOverwrite ? kFoundIdx->second : _filled++] = std::move(elem);
}
}

#endif
