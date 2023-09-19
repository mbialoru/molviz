# A very simple example of a configured file that might need to be
# converted to one that is publicly installed in the case that
# you are developing a library
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
configure_file("config.hpp.in" "${CMAKE_BINARY_DIR}/config/include/internal_use_only/config.hpp" ESCAPE_QUOTES)
