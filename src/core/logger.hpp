#ifndef _EFE_LOGGER_HPP_
#define _EFE_LOGGER_HPP_

#include "core/types.hpp"

#include <cstdarg>
#include <string>
#include <string_view>

namespace core
{
class Logger final
{
public:
  inline explicit Logger(std::string_view module) noexcept
    : _kModule(module) {};

  inline void set_section(const char *section) noexcept { _section = section; }

  void info(const char *msg) noexcept;
  void info_fmt(const char *fmt, ...) noexcept;

  void debug(const char *msg) noexcept;
  void debug_fmt(const char *msg, ...) noexcept;

  void warn(const char *msg) noexcept;

  void error(const char *msg) noexcept;
  void error_fmt(const char *msg, ...) noexcept;

  void critical(const char *msg) noexcept;
  void critical_fmt(const char *msg, ...) noexcept;

  void progress(const char *msg) noexcept;
  void progress_fmt(const char *msg, ...) noexcept;

private:
  enum _tLogColor : byte
  {
    RED    = 31,
    GREEN  = 32,
    YELLOW = 33,
    CYAN   = 36,
  };

  const std::string _kModule;
  std::string _section;

  std::string _basic_format_msg(const char *msg) const noexcept;
  std::string _format_msg(const char *msg) const noexcept;
  std::string
  _format_msg(const char *msg, _tLogColor color, bool bold) const noexcept;

  std::string _basic_format_progress(const char *msg) const noexcept;

  void _log_fmt(const std::string &msg, va_list &vaArgs) noexcept;
};
}

#endif
