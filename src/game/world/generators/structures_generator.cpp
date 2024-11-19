#include "game/world/generators/structures_generator.hpp"

namespace game
{
void StructuresGenerator::push(const WorldStructure &structure) noexcept
{
  _structures.push_back(structure);

  auto data = factory(structure.id)->data();
  for (ullong i = 0; i < data.size(); i++)
  {
    if (data.at(i))
    {
      // Convert the structure's block relative position to an absolute position
      data.at(i)->x += structure.x;
      data.at(i)->y += structure.y;
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
