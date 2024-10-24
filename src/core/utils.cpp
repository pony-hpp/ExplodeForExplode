#include "core/exception.hpp"
#include "core/utils.hpp"

#include <string>

void core::check_file_extension(const char *filename, const char *extension)
{
  const std::string kFilenameStr = filename;
  const std::string kFileExtension =
    kFilenameStr.substr(kFilenameStr.find_last_of('.') + 1);

  if (kFileExtension != extension)
  {
    throw InvalidFileExtensionException {kFileExtension};
  }
}
