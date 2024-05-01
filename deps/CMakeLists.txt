add_library(sfml-imgui
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui-SFML.cpp
    imgui/imgui.cpp
    )
target_link_libraries(sfml-imgui PRIVATE sfml-graphics)
target_include_directories(sfml-imgui PUBLIC imgui)
target_compile_features(sfml-imgui PRIVATE cxx_std_17)
