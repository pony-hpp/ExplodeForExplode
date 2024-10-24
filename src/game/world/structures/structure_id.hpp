#ifndef _STRUCTURE_ID_HPP_
#define _STRUCTURE_ID_HPP_

namespace game::structures
{
enum StructureId : unsigned char
{
  TREE
};

extern const char *id_to_str(unsigned char id) noexcept;
}

#endif
