#ifndef EFE_STRUCTURES_GENERATOR_HPP
#define EFE_STRUCTURES_GENERATOR_HPP

#include "core/types.hpp"
#include "game/world/structures/structure.hpp"

#include <vector>

namespace game
{
struct WorldStructure
{
  ubyte id;
  int x;
  int y;
};

class StructuresGenerator final
{
public:
  using StructureFactory = std::unique_ptr<Structure> (*)(ubyte);

  StructureFactory factory = Structure::from_id;

  inline const std::vector<WorldStructure> &structures() const noexcept
  {
    return _structures;
  }

  inline const std::vector<Structure::Data> &structuresData() const noexcept
  {
    return _structuresData;
  }

  // The `_structures.size()` and `_structuresData.size()` are always the same.
  inline size_t size() const noexcept { return _structures.size(); }

  void push(const WorldStructure &structure) noexcept;
  void pop() noexcept;

private:
  std::vector<WorldStructure> _structures;
  std::vector<Structure::Data> _structuresData;
};
}

#endif
