#include "game/world/generators/structures_generator.hpp"

namespace game
{
void StructuresGenerator::push(const WorldStructure &structure) noexcept
{
  _structures.push_back(structure);

  auto data = factory(structure.id)->data();
  for (ullong i = 0; i < data.size(); i++)
  {
    BlockData *kBlock = data.at(i);
    if (kBlock)
    {
      // Convert the structure's block relative position to an absolute position
      kBlock->x += structure.x;
      kBlock->y += structure.y;

      for (auto &childPos : kBlock->childPoses)
      {
        childPos.first += structure.x;
        childPos.second += structure.y;
      }
    }
  }

  _structuresData.push_back(std::move(data));
}

void StructuresGenerator::pop() noexcept
{
  _structures.pop_back();
  _structuresData.pop_back();
}
}
