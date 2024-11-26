#include "core/logger.hpp"

#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>

namespace chrono = std::chrono;
using Clock      = chrono::system_clock;

namespace core
{
void Logger::info(const char *msg) noexcept { std::clog << _format_msg(msg); }

void Logger::info_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg), list);
}

void Logger::debug(const char *msg) noexcept
{
  std::clog << _format_msg(msg, CYAN, true);
}

void Logger::debug_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg, CYAN, true), list);
}

void Logger::warn(const char *msg) noexcept
{
  std::cerr << _format_msg(msg, YELLOW, false);
}

void Logger::error(const char *msg) noexcept
{
  std::cerr << _format_msg(msg, RED, false);
}

void Logger::error_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg, RED, false), list);
}

void Logger::critical(const char *msg) noexcept
{
  std::cerr << _format_msg(msg, RED, true);
}

void Logger::critical_fmt(const char *msg, ...) noexcept
{
  va_list list;
  va_start(list, msg);
  _log_fmt(_format_msg(msg, RED, true), list);
}

std::string Logger::_basic_format_msg(const char *msg) const noexcept
{
  const auto kTime       = Clock::now();
  const time_t kRawCTime = Clock::to_time_t(kTime);
  const tm *kCTime       = localtime(&kRawCTime);

  // As `tm` can't represent milliseconds, get them separately (rounded to
  // hundreds)
  const short kMilliseconds =
    chrono::duration_cast<chrono::milliseconds>(kTime.time_since_epoch())
      .count() %
    1000;

  char time[13]; // HH:MM:SS.mmm is always 13 characters (including NUL)
  strftime(time, sizeof(time), "%T.", kCTime);
  sprintf(&time[9], "%.3i", kMilliseconds);

  std::string res;
  res.append(std::string("[") + time + "] ");

  const std::string kSectionStr = _section.empty() ? "" : '/' + _section;
  res.append(std::string("[") + _kModule + kSectionStr + "] ");

  res.append(msg);

  return res;
}

std::string Logger::_format_msg(const char *msg) const noexcept
{
  return _basic_format_msg(msg) + '\n';
}

std::string
Logger::_format_msg(const char *msg, _tLogColor color, bool bold) const noexcept
{
  std::string colorEscCode = "\033[";

  // Will be inserted as "0;" (not bold) or "1;" (bold)
  colorEscCode.append(std::to_string(bold) + ";");

  colorEscCode.append(std::to_string(color) + "m");

  return colorEscCode + _basic_format_msg(msg) + "\033[0;39m\n";
}

void Logger::_log_fmt(const std::string &msg, va_list &vaArgs) noexcept
{
  vfprintf(stderr, msg.data(), vaArgs);
  va_end(vaArgs);
}
}
