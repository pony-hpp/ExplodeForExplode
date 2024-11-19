#include "core/types.hpp"
#include "game/utils/data_2d.hpp"

#include <gtest/gtest.h>

using namespace game::utils;

TEST(Data2D, GettingEmptyElementReturnsNullptrTest)
{
  Data2D<bool> data(3, 3, 3);

  for (byte i = 0; i < 12; i++)
  {
    ASSERT_EQ(data.at(i), nullptr);
    ASSERT_EQ(data.at(i, i), nullptr);
  }
}

TEST(Data2D, GettingIndexOfEmptyExtraElementThrowsExceptionTest)
{
  Data2D<bool> data(3, 3, 3);

  for (byte i = 9; i < 12; i++)
  {
    ASSERT_THROW(data.idx(i, i), NonexistentExtraElementException);
  }
}

TEST(Data2D, GetTest)
{
  Data2D<char> data(2, 4);

  const char *kStr = "abcdefgh";
  for (size_t i = 0; i < strlen(kStr); i++)
  {
    char c = kStr[i];
    data.push(std::move(c));
  }

  for (byte y = 0; y < 4; y++)
  {
    for (byte x = 0; x < 2; x++)
    {
      ASSERT_EQ(*data.at(x, y), *data.at(y * 2 + x));
      ASSERT_EQ(*data.at(x, y), kStr[y * 2 + x]);
    }
  }
}

TEST(Data2D, GetExtraDataTest)
{
  Data2D<bool> data(2, 2, 3);

  for (byte i = 0; i < 4; i++)
  {
    // The specified position should be ignored, because we aren't pushing out
    // of bounds
    data.push(true, {314, 159});
  }

  data.push(false, {4, 2});
  data.push(true, {2, 4});
  data.push(false, {2, 4});
  data.push(true, {-1, -1});

  ASSERT_EQ(data.extra_data_size(), 3);

  ASSERT_EQ(data.at(314, 159), nullptr);
  ASSERT_EQ(*data.at(0, 0), true);
  ASSERT_EQ(*data.at(0, 1), true);
  ASSERT_EQ(*data.at(1, 0), true);
  ASSERT_EQ(*data.at(0, 1), true);
  ASSERT_EQ(*data.at(4, 2), false);
  ASSERT_EQ(*data.at(2, 4), false);
  ASSERT_EQ(*data.at(-1, -1), true);
}
