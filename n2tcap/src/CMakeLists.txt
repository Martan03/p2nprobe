file(GLOB src_ CONFIGURE_DEPENDS "**/*.cpp" "*.cpp")

target_sources(n2tcap PRIVATE ${src_})
