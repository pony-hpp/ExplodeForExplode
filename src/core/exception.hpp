#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <string>

namespace core {
struct FopenException {};

struct InvalidFileExtensionException {
  std::string provided;
};
}

#endif