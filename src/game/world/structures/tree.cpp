#include "core/rand.hpp"
#include "core/types.hpp"
#include "game/world/structures/structures.hpp"

namespace game::structures
{
const char *Tree::name() const noexcept { return "Tree"; }

Structure::Data Tree::data() const noexcept
{
  const byte kTrunkH = core::Rand::next(8, 10);

  Data data(7, kTrunkH + 2);

  for (byte i = 0; i < kTrunkH; i++)
  {
    data.push({blocks::OAK_BLOCK, 0, i});
  }

  data.push({blocks::OAK_LEAVES, 0, kTrunkH});
  data.push({blocks::OAK_LEAVES, 0, kTrunkH + 1});
  data.push({blocks::OAK_LEAVES, -3, kTrunkH - 2});
  data.push({blocks::OAK_LEAVES, -2, kTrunkH - 2});
  data.push({blocks::OAK_LEAVES, -1, kTrunkH - 2});
  data.push({blocks::OAK_LEAVES, 1, kTrunkH - 2});
  data.push({blocks::OAK_LEAVES, 2, kTrunkH - 2});
  data.push({blocks::OAK_LEAVES, 3, kTrunkH - 2});
  data.push({blocks::OAK_LEAVES, -2, kTrunkH - 1});
  data.push({blocks::OAK_LEAVES, -1, kTrunkH - 1});
  data.push({blocks::OAK_LEAVES, 1, kTrunkH - 1});
  data.push({blocks::OAK_LEAVES, 2, kTrunkH - 1});
  data.push({blocks::OAK_LEAVES, -1, kTrunkH});
  data.push({blocks::OAK_LEAVES, 1, kTrunkH});
  data.push({blocks::OAK_LEAVES, -1, kTrunkH + 1});
  data.push({blocks::OAK_LEAVES, 1, kTrunkH + 1});

  return data;
}
}
