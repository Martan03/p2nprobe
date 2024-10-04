# file(GLOB_RECURSE src_ CONFIGURE_DEPENDS *.cpp)
file(GLOB src_ CONFIGURE_DEPENDS "**/*.cpp" "*.cpp")

target_sources(p2nprobe PRIVATE ${src_})
