#include "core/exception.hpp"
#include "core/utils.hpp"

#include <gtest/gtest.h>

TEST(CoreUtils, CheckFileExtensionTest) {
  ASSERT_NO_THROW(core::check_file_extension("shader.vs", "vs"));
  ASSERT_NO_THROW(core::check_file_extension("source.cxx", "cxx"));
  ASSERT_NO_THROW(core::check_file_extension(".config.conf", "conf"));
  ASSERT_NO_THROW(core::check_file_extension("../src/core/./utils.hpp", "hpp"));
  ASSERT_THROW(
    core::check_file_extension("main.cpp", "cxx"),
    core::InvalidFileExtensionException
  );
  ASSERT_THROW(
    core::check_file_extension("video.webm", "mp4"),
    core::InvalidFileExtensionException
  );
  ASSERT_THROW(
    core::check_file_extension(".install.ash", "sh"),
    core::InvalidFileExtensionException
  );
  ASSERT_THROW(
    core::check_file_extension("../../tests/main.cxxc", "cxx"),
    core::InvalidFileExtensionException
  );

  try {
    core::check_file_extension("os.iso", "img");
  } catch (const core::InvalidFileExtensionException &e) {
    ASSERT_EQ(e.provided, "iso");
  }
}
