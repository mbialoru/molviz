file(GLOB SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(lib${PROJECT_NAME} PRIVATE ${SRCS})
