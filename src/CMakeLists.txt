add_executable(game main.cpp)
target_link_libraries(game PRIVATE sfml-graphics)
target_link_libraries(game PRIVATE sfml-imgui)
target_compile_features(game PRIVATE cxx_std_17)

install(TARGETS game)
