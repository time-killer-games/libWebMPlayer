#include <vector>
#include <cstddef>

#include "widen_narrow.h"

#include <windows.h>

std::wstring widen(std::string str) {
  std::size_t wchar_count = str.size() + 1;
  std::vector<wchar_t> buf(wchar_count);
  return std::wstring { buf.data(), 
    (std::size_t)MultiByteToWideChar(
    CP_UTF8, 0, str.c_str(), -1, 
    buf.data(), (int)wchar_count) };
}

std::string narrow(std::wstring wstr) {
  int nbytes = WideCharToMultiByte(CP_UTF8,
    0, wstr.c_str(), (int)wstr.length(), 
    nullptr, 0, nullptr, nullptr);
  std::vector<char> buf(nbytes);
  return std::string { buf.data(), 
    (std::size_t)WideCharToMultiByte(
    CP_UTF8, 0, wstr.c_str(), 
    (int)wstr.length(), buf.data(), 
    nbytes, nullptr, nullptr) };
}
