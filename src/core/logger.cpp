#include "core/logger.hpp"

#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>

namespace chrono = std::chrono;
using Clock      = chrono::system_clock;

namespace core
{
Logger::Logger(std::string_view module) noexcept : _kModule(module) {}

void Logger::set_section(const char *section) noexcept { _section = section; }

void Logger::info(const char *msg) noexcept { std::clog << _format_msg(msg); }

void Logger::info_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg), list);
}

void Logger::debug(const char *msg) noexcept
{
  std::clog << _format_msg(msg, _CYAN, true);
}

void Logger::debug_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg, _CYAN, true), list);
}

void Logger::warn(const char *msg) noexcept
{
  std::cerr << _format_msg(msg, _YELLOW, false);
}

void Logger::error(const char *msg) noexcept
{
  std::cerr << _format_msg(msg, _RED, false);
}

void Logger::error_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg, _RED, false), list);
}

void Logger::critical(const char *msg) noexcept
{
  std::cerr << _format_msg(msg, _RED, true);
}

void Logger::critical_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg, _RED, true), list);
}

void Logger::progress(const char *msg) noexcept
{
  std::cerr << _format_progress_base(msg);
}

void Logger::progress_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_progress_base(msg), list);
}

std::string Logger::_format_msg_base(const char *msg) const noexcept
{
  const auto kTime       = Clock::now();
  const time_t kRawCTime = Clock::to_time_t(kTime);
  const tm *kCTime       = localtime(&kRawCTime);

  char time[13]; // HH:MM:SS.mmm is always 13 characters (including NUL).
  strftime(time, sizeof(time), "%T.", kCTime);
  sprintf(
    &time[9], "%.3li",
    chrono::duration_cast<chrono::milliseconds>(kTime.time_since_epoch())
        .count() %
      1000
  );

  return std::string("[") + time + "] [" + _kModule +
         (_section.empty() ? "" : '/' + _section) + "] " + msg;
}

std::string Logger::_format_msg(const char *msg) const noexcept
{
  return _format_msg_base(msg) + '\n';
}

std::string
Logger::_format_msg(const char *msg, _LogColor color, bool bold) const noexcept
{
  return "\033[" + std::to_string(bold) + ";" + std::to_string(color) + 'm' +
         _format_msg_base(msg) + "\033[0;39m\n";
}

std::string Logger::_format_progress_base(const char *msg) const noexcept
{
  return "\r\033[2K" + _format_msg(msg, _GREEN, false) + "\033[A";
}

void Logger::_log_fmt(const std::string &msg, va_list &vaArgs) noexcept
{
  vfprintf(stderr, msg.data(), vaArgs);
  va_end(vaArgs);
}
}
