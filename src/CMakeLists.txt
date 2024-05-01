add_executable(comp4300a1 main.cpp)
target_link_libraries(comp4300a1 PRIVATE sfml-graphics)
target_link_libraries(comp4300a1 PRIVATE sfml-imgui)
target_compile_features(comp4300a1 PRIVATE cxx_std_17)

install(TARGETS comp4300a1)
