#include "core/types.hpp"
#include "game/world/structures/structures.hpp"

static constexpr byte TRUNK_H = 8;

namespace game::structures
{
const char *Tree::name() const noexcept { return "Tree"; }

Structure::Data Tree::data() const noexcept
{
  Data data(7, TRUNK_H + 2);

  for (byte i = 0; i < TRUNK_H; i++)
  {
    data.push({blocks::OAK_BLOCK, 0, i});
  }

  data.push({blocks::OAK_LEAVES, 0, TRUNK_H});
  data.push({blocks::OAK_LEAVES, 0, TRUNK_H + 1});
  data.push({blocks::OAK_LEAVES, -3, TRUNK_H - 2});
  data.push({blocks::OAK_LEAVES, -2, TRUNK_H - 2});
  data.push({blocks::OAK_LEAVES, -1, TRUNK_H - 2});
  data.push({blocks::OAK_LEAVES, 1, TRUNK_H - 2});
  data.push({blocks::OAK_LEAVES, 2, TRUNK_H - 2});
  data.push({blocks::OAK_LEAVES, 3, TRUNK_H - 2});
  data.push({blocks::OAK_LEAVES, -2, TRUNK_H - 1});
  data.push({blocks::OAK_LEAVES, -1, TRUNK_H - 1});
  data.push({blocks::OAK_LEAVES, 1, TRUNK_H - 1});
  data.push({blocks::OAK_LEAVES, 2, TRUNK_H - 1});
  data.push({blocks::OAK_LEAVES, -1, TRUNK_H});
  data.push({blocks::OAK_LEAVES, 1, TRUNK_H});
  data.push({blocks::OAK_LEAVES, -1, TRUNK_H + 1});
  data.push({blocks::OAK_LEAVES, 1, TRUNK_H + 1});

  return data;
}
}
