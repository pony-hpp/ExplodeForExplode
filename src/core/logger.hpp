#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <cstdarg>
#include <string>

namespace core
{
class Logger final
{
public:
  explicit Logger(std::string_view module) noexcept;

  void set_section(const char *section) noexcept;

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
  enum _LogColor : char
  {
    _RED    = 31,
    _GREEN  = 32,
    _YELLOW = 33,
    _CYAN   = 36,
  };

  const std::string _kModule;
  std::string _section;

  std::string _format_msg_base(const char *msg) const noexcept;
  std::string _format_msg(const char *msg) const noexcept;
  std::string
  _format_msg(const char *msg, _LogColor color, bool bold) const noexcept;

  std::string _format_progress_base(const char *msg) const noexcept;

  void _log_fmt(const std::string &msg, va_list &vaArgs) noexcept;
};
}

#endif
