#include "core/exception.hpp"
#include "core/utils.hpp"

#include <gtest/gtest.h>

using namespace core;

TEST(CoreUtils, CheckFileExtensionTest) {
  ASSERT_NO_THROW(check_file_extension("shader.glsl", "glsl"));
  ASSERT_NO_THROW(check_file_extension("source.cxx", "cxx"));
  ASSERT_NO_THROW(check_file_extension(".config.ini", "ini"));
  ASSERT_NO_THROW(check_file_extension("../src/core/./utils.hpp", "hpp"));
  ASSERT_THROW(
    check_file_extension("main.cpp", "cxx"), InvalidFileExtensionException
  );
  ASSERT_THROW(
    check_file_extension("video.webm", "mp4"), InvalidFileExtensionException
  );
  ASSERT_THROW(
    check_file_extension(".install.zsh", "sh"), InvalidFileExtensionException
  );
  ASSERT_THROW(
    check_file_extension("../../tests/main.cxxc", "cxx"),
    InvalidFileExtensionException
  );

  try {
    check_file_extension("os.iso", "img");
  } catch (const InvalidFileExtensionException &e) {
    ASSERT_EQ(e.provided, "iso");
  }
}
