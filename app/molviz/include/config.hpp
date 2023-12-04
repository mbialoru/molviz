#ifndef MOLVIZ_CONFIG_HPP
#define MOLVIZ_CONFIG_HPP

#include <cstdlib>
#include <string_view>

inline constexpr std::string_view project_name{ "molviz" };
inline constexpr std::string_view project_version{ "0.0.1" };
inline constexpr std::size_t project_version_major{ 0 };
inline constexpr std::size_t project_version_minor{ 0 };
inline constexpr std::size_t project_version_patch{ 1 };
inline constexpr std::size_t project_version_tweak{  };

inline constexpr std::string_view git_sha{ "628b1e1c106f72be3cc4fbed52463f069ff10e10" };
inline constexpr std::string_view GIT_BRANCH{ "main" };
inline constexpr std::string_view GIT_SHORT_SHA{ "628b1e1c" };
inline constexpr std::string_view BUILD_TYPE{ "Debug" };
inline constexpr std::string_view PLATFORM{ "Linux" };
inline constexpr std::string_view COMPILER{ "GNU" };
inline constexpr std::string_view COMPILER_VERSION{ "13.2.0" };
inline constexpr std::string_view BUILD_DATE{ "2023-12-04 11:13:20 UTC" };

#endif// MOLVIZ_CONFIG_HPP
