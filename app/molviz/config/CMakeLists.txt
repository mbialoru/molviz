# create git variables
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S UTC" UTC)

configure_file("config.hpp.in" "${CMAKE_SOURCE_DIR}/app/${CMAKE_PROJECT_NAME}/include/config.hpp" ESCAPE_QUOTES)