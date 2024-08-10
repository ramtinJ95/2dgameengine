add_executable(
    game main.cpp Action.cpp Animation.cpp Assets.cpp Entity.cpp 
    EntityManager.cpp GameEngine.cpp Physics.cpp Scene.cpp Scene_Menu.cpp 
    Scene_play.cpp Vec2.cpp)
target_link_libraries(game PRIVATE sfml-graphics)
target_link_libraries(game PRIVATE sfml-imgui)
target_compile_features(game PRIVATE cxx_std_17)

install(TARGETS game)
