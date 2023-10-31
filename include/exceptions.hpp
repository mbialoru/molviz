#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

class NotImplementedException : public std::logic_error
{
public:
  NotImplementedException() : std::logic_error("Function not yet implemented") {}
  NotImplementedException(const std::string t_function_name) : NotImplementedException()
  {
    spdlog::error("function not yet implemented {}", t_function_name);
  }
  NotImplementedException(const char *tp_function_name) : NotImplementedException(std::string(tp_function_name)) {}
};
