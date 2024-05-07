#include <fstream>
#include <iostream>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

class Rectangle {
public:
  std::string m_name = "name";
  float m_posX, m_posY, m_speedX, m_speedY, m_width, m_height = 0;
  int m_R, m_G, m_B = 0;
  bool visable = true;
  sf::RectangleShape rectShape;

  Rectangle() {}
  Rectangle(const std::string &name, float posX, float posY, float speedX,
            float speedY, float width, float height, int R, int G, int B)
      : m_name(name), m_posX(posX), m_posY(posY), m_speedX(speedX),
        m_speedY(speedY), m_width(width), m_height(height), m_R(R), m_B(B),
        m_G(G) {}

  void print() {
    std::cout << m_name << " " << m_posY << " " << m_posX << " " << m_R << " "
              << m_G << " " << m_B << " " << m_width << " " << m_height
              << " end of rectangle object. \n";
  }

  void checkBoundery(int xBoundry, int yBoundry) { 
    if(rectShape.getPosition().x < 0 || rectShape.getPosition().x + rectShape.getSize().x > xBoundry){
      m_speedX *= -1;
    }
    if(rectShape.getPosition().y < 0 || rectShape.getPosition().y + rectShape.getSize().y > yBoundry){
      m_speedY *= -1;
    }
  }

  void update(int xBoundry, int yBoundry) {
    checkBoundery(xBoundry, yBoundry);
    rectShape.setPosition(rectShape.getPosition().x + m_speedX,
                          rectShape.getPosition().y + m_speedY);
    m_posX = rectShape.getPosition().x;
    m_posY = rectShape.getPosition().y;
  }

  void toggleVisable() { visable = !visable; }
};

class Circle {
public:
  std::string m_name = "name";
  float m_posX, m_posY, m_speedX, m_speedY, m_radius = 0;
  int m_R, m_G, m_B = 0;

  Circle() {}
  Circle(const std::string &name, float posX, float posY, float speedX,
         float speedY, float radius, int R, int G, int B)
      : m_name(name), m_posX(posX), m_posY(posY), m_speedX(speedX),
        m_speedY(speedY), m_radius(radius), m_R(R), m_G(G), m_B(B) {}

  void print() {
    std::cout << m_name << " " << m_posY << " " << m_posX << " " << m_R << " "
              << m_G << " " << m_B << " " << m_radius
              << " end of circle object. \n";
  }
};

int main(int argc, char *argv[]) {
  std::vector<Rectangle> rectangles;
  std::vector<Circle> circles;
  int wWidth = 0;
  int wHeight = 0;
  const std::string configFile = "src/config.txt";
  std::ifstream fin(configFile);
  std::string line;

  sf::Font myFont;
  std::string fontPath;
  int fontSize, fontR, fontG, fontB = 0;

  while (std::getline(fin, line)) {
    std::istringstream iss(line);
    std::string type;
    if (!(iss >> type)) {
      std::cerr << "Error reading type.\n";
      break;
    }
    if (type == "Window") {
      iss >> wWidth >> wHeight;

    } else if (type == "Font") {
      iss >> fontPath >> fontSize >> fontR >> fontG >> fontB;

    } else if (type == "Rectangle") {
      Rectangle rectangle;
      iss >> rectangle.m_name >> rectangle.m_posX >> rectangle.m_posY >>
          rectangle.m_speedX >> rectangle.m_speedY >> rectangle.m_R >>
          rectangle.m_G >> rectangle.m_B >> rectangle.m_width >>
          rectangle.m_height;

      rectangle.rectShape.setPosition(rectangle.m_posX, rectangle.m_posY);
      rectangle.rectShape.setSize(
          sf::Vector2f{rectangle.m_width, rectangle.m_height});
      rectangle.rectShape.setFillColor(
          sf::Color(rectangle.m_R, rectangle.m_G, rectangle.m_B));
      rectangles.push_back(rectangle);

    } else if (type == "Circle") {
      Circle circle;
      iss >> circle.m_name >> circle.m_posX >> circle.m_posY >>
          circle.m_speedX >> circle.m_speedY >> circle.m_R >> circle.m_G >>
          circle.m_B >> circle.m_radius;
      circles.push_back(circle);
    }
  }
  fin.close();

  // create a new window of size w*h pixels
  // top-left of the window is (0,0) and bottom-right is (w,h)
  // you will have to read these from the config file
  sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
  window.setFramerateLimit(60); // limit frame rate to 60 fps

  // initialize IMGUI and create a cloc-n__n-k used for its internal timing
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  // scale the imgui ui by a given factor, does not affect text size
  ImGui::GetStyle().ScaleAllSizes(1.0f);

  // the imgui color {r, g, b} wheel requires floats from 0-1 instead of ints
  // from 0-255
  float c[3] = {0.0f, 1.0f, 1.0f};

  // let's make a shape that we will draw to the screen
  float circleRadius = 50;   // radius to draw the circle
  int circleSegments = 32;   // number of segments to draw the circle with
  float circleSpeedX = 1.0f; // we will use this to move the circle later
  float circleSpeedY = 0.5f; // you will read these values from the file
  bool drawCircle = true;    // wheter or not to draw the circle
  bool drawText = true;      // wheter or not to draw the text

  // create the sfml circle shape based on our parameters
  sf::CircleShape circle(
      circleRadius, circleSegments); // create a circle shape with radius 50
  circle.setPosition(10.0f, 10.0f);  // set the top-left position of the circle

  // attempt to load the font from a file
  if (!myFont.loadFromFile("fonts/Arial.ttf")) {
    // if we can't load the font, print an error to the error console and exit
    std::cerr << "Could not load font!\n";
    exit(-1);
  }

  // set up the text object that will be drawn to the screen
  sf::Text text("Sample Text", myFont, 24);

  // position the top-left corner of the text so that the text aligns on the
  // bottom text character size is in pixels, so move the text up from the
  // bottom by its height

  // set up a character array to set the text
  char displayString[255] = "Sample Text";

  // main loop - continues for each frame while window is open
  while (window.isOpen()) {
    // for (auto event = sf::Event{}; window.pollEvent(event);)
    // {
    //     if (event.type == sf::Event::Closed)
    //     {
    //         window.close();
    //     }
    // }

    // event handling
    sf::Event event;
    while (window.pollEvent(event)) {
      // pass the event to imgui to be parsed
      ImGui::SFML::ProcessEvent(window, event);

      // this event triggers whe the window is closed
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // this event is triggered when a key is pressed
      if (event.type == sf::Event::KeyPressed) {
        // print the key that was pressed to the console
        std::cout << "Key pressed with code = " << event.key.code << "\n";

        // example, what happens when x is pressed
        if (event.key.code == sf::Keyboard::X) {
          // reverse the x direction of the circle on the screen
          circleSpeedX *= -1.0f;
        }
        if (event.key.code == sf::Keyboard::S) {
          window.close();
        }
      }
    }

    // update imgui for this frame with the time that the last frame took
    ImGui::SFML::Update(window, deltaClock.restart());

    // draw the UI
    ImGui::Begin("Window title");
    ImGui::Text("Window text!");
    ImGui::Checkbox("Draw Circle", &drawCircle);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Text", &drawText);
    ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
    ImGui::SliderInt("Sides", &circleSegments, 3, 64);
    ImGui::ColorEdit3("Color Circle", c);
    ImGui::InputText("Text", displayString, 255);
    if (ImGui::Button("Set Text")) {
      text.setString(displayString);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Circle")) {
      circle.setPosition(0, 0);
    }
    ImGui::End();

    // set the circle properties, because they may have been updated with the ui
    circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));
    circle.setPointCount(circleSegments);
    circle.setRadius(circleRadius);

    // basic animation - move each shape if it's still in frame
    circle.setPosition(circle.getPosition().x + circleSpeedX,
                       circle.getPosition().y + circleSpeedY);
    // basic rendering function calls
    window.clear(); // clear the window of anything previously drawn
    if (drawCircle) // draw the circle if the boolean is true
    {
      window.draw(circle);
      for (auto &rect : rectangles) {
        sf::Text rectName(rect.m_name, myFont, 20);
        rectName.setFillColor(sf::Color::White);
        rect.update(wWidth, wHeight);
        auto rectBB = rect.rectShape.getGlobalBounds(); 
        if(rectBB.height < 20){
          rectName.setCharacterSize(rectBB.height * 0.9);
        }
        auto rectNameBB= rectName.getGlobalBounds();
        rectName.setPosition( rectBB.left + rectBB.width / 2 - rectNameBB.width / 2,
            rectBB.top + rectBB.height / 2 - rectNameBB.height / 2 - 6);
        window.draw(rect.rectShape);
        window.draw(rectName);
      }

    }
    if (drawText) // draw the text if the boolean is true
    {
      window.draw(text);
    }
    ImGui::SFML::Render(window); // draw the ui last so it's on top
    window.display();            // call the window display function
  }

  return 0;
}
