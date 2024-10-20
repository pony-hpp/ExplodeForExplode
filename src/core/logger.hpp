#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <string>

namespace core {
class Logger final {
public:
  Logger(const char *module) noexcept;

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
  const std::string _kModule;
  std::string _section;

  enum _LogColor : unsigned char {
    _RED    = 31,
    _GREEN  = 32,
    _YELLOW = 33,
    _CYAN   = 36,
  };

  std::string _format_msg_base(const char *msg) const noexcept;
  std::string _format_msg(const char *msg) const noexcept;
  std::string
  _format_msg(const char *msg, _LogColor color, bool bold) const noexcept;
  void _log_fmt(const char *msg, va_list &vaArgs) noexcept;
};
}

#endif
