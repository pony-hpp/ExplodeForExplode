#include "game/world/blocks/block_id.hpp"
#include "game/world/structures/structures.hpp"

namespace game::structures {
unsigned short Tree::w() const noexcept {
  return 7;
}

unsigned short Tree::h() const noexcept {
  return 10;
}

std::unique_ptr<BlockData[]> Tree::data() const noexcept {
  auto data = std::make_unique<BlockData[]>(w() * h());

  // clang-format off
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 0});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 1});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 2});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 3});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 4});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 5});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 6});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 7});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 8});
  _push_block(data, {blocks::DEFAULT_BLOCK,  0, 9});
  _push_block(data, {blocks::DEFAULT_BLOCK, -3, 6});
  _push_block(data, {blocks::DEFAULT_BLOCK, -2, 6});
  _push_block(data, {blocks::DEFAULT_BLOCK, -1, 6});
  _push_block(data, {blocks::DEFAULT_BLOCK,  1, 6});
  _push_block(data, {blocks::DEFAULT_BLOCK,  2, 6});
  _push_block(data, {blocks::DEFAULT_BLOCK,  3, 6});
  _push_block(data, {blocks::DEFAULT_BLOCK, -2, 7});
  _push_block(data, {blocks::DEFAULT_BLOCK, -1, 7});
  _push_block(data, {blocks::DEFAULT_BLOCK,  1, 7});
  _push_block(data, {blocks::DEFAULT_BLOCK,  2, 7});
  _push_block(data, {blocks::DEFAULT_BLOCK, -1, 8});
  _push_block(data, {blocks::DEFAULT_BLOCK,  1, 8});
  _push_block(data, {blocks::DEFAULT_BLOCK, -1, 9});
  _push_block(data, {blocks::DEFAULT_BLOCK,  1, 9});
  // clang-format on

  return data;
}
}
