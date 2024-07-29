#include "Game.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <numbers>
#include <string>

#include "SFML/Window/Event.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

Game::Game(const std::string &config)
{
  init(config);
}

void Game::init(const std::string &path)
{
  // todo: read in config file here
  //       use the premade PlayerConfig, EnemyConfig, BulletConfig variables
  std::ifstream fin(path);
  if (!fin)
  {
    std::cerr << "Could not find/load config.txt file!\n";
    std::cerr << path;
    exit(-1);
  }

  std::string head;
  int window_width, window_height;
  int frameLimit, fullscreen;
  while (fin >> head)
  {
    if (head == "Window")
    {
      fin >> window_width >> window_height >> frameLimit >> fullscreen;
    }
    else if (head == "Font")
    {
      int r, g, b;
      std::string font_file;
      int font_size;
      fin >> font_file >> font_size >> r >> g >> b;
      m_font.loadFromFile(font_file);
      m_text.setFillColor(sf::Color(r, g, b));
      m_text = sf::Text("SCORE: ", m_font, font_size);
      m_text.setPosition(10, 10);
    }
    else if (head == "Player")
    {
      fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >>
          m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >>
          m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
    }
    else if (head == "Enemy")
    {
      fin >> m_enemyConfig.SR
                >> m_enemyConfig.CR
                >> m_enemyConfig.SMIN
                >> m_enemyConfig.SMAX
                >> m_enemyConfig.OR
                >> m_enemyConfig.OG
                >> m_enemyConfig.OB
                >> m_enemyConfig.OT
                >> m_enemyConfig.VMIN
                >> m_enemyConfig.VMAX
                >> m_enemyConfig.L
                >> m_enemyConfig.SI;
    }
    else if (head == "Bullet")
    {
      fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >>
          m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >>
          m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
    }
    else
    {
      std::cerr << "head is: " << head << "\n";
      std::cerr << "The config file is not formatted correctly or contains "
                   "weird wrong things!\n";
      exit(-1);
    }
  }

  // set up default window parameters
  m_window.create(sf::VideoMode(window_width, window_height), "SFML");
  m_window.setFramerateLimit(frameLimit);

  ImGui::SFML::Init(m_window);
  spawnPlayer();
}

void Game::run()
{
  // todo: add pause functionality in here
  //       some systems should function while paused (rendering)
  //       some systems shouldn't (movement / input)
  while (m_running)
  {
    // update the entity manager
    m_entities.update();

    // required update call to ImGui
    ImGui::SFML::Update(m_window, m_deltaClock.restart());

    sEnemySpawner();
    sMovement();
    sCollision();
    sUserInput();
    sGUI();
    sRender();

    // increment the current frame
    // may need to be moved when pause implemented
    m_currentFrame++;
  }
}

void Game::setPaused(bool paused)
{
  // todo
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
  // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
  auto entity = m_entities.addEntity("player");

  // Give this entity a Transform so it spawns at (200, 200) with velocity (1,
  // 1) and angle 0
  entity->cTransform = std::make_shared<CTransform>(Vec2(200.f, 200.f),
                                                    Vec2(m_playerConfig.S, m_playerConfig.S), 0.f);

  // The entity's shape will have radius 32, 8 sides, dark grey fill,
  // and red outline of thickness 4
  entity->cShape = std::make_shared<CShape>(
      m_playerConfig.SR, m_playerConfig.V,
      sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
      sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

  entity->cInput = std::make_shared<CInput>();
  entity->cCollission = std::make_shared<CCollision>(m_playerConfig.CR);

  m_player = entity;
  spawnEnemy();
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
  // todo: make sure the enemy is spawned properly with the m_enemyConfig
  // variables
  //       the enemy must be spawned completely within the bounds of the window

  // record when the most recent enemy was spawned
  auto enemy_entity = m_entities.addEntity("enemy");
  enemy_entity->cTransform = std::make_shared<CTransform>(Vec2(400.f, 200.f), Vec2(0, 0), 0.f);

  enemy_entity->cShape =
      std::make_shared<CShape>(m_enemyConfig.SR, 3, sf::Color(10, 10, 10), sf::Color::Blue, 4.f);

  enemy_entity->cCollission = std::make_shared<CCollision>(16);
  m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
  // todo: spawn small enemies at the location of the input enemy e

  // when we create the smaller enemy, we have to read the values of the
  // original enemy
  // - spawn a number of small enemies equal to the vertices of the original
  // enemy
  // - set each small enemy to the same color as the original, half the size
  // - small enemies are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &target)
{
  // todo: implement the spawning of a bullet which travels toward target
  //       - bullet speed is given as a scalar speed
  //       - you must set the velocity by using formula in notes
  auto bullet = m_entities.addEntity("bullet");
  float origin_x = entity->cTransform->pos.x;
  float origin_y = entity->cTransform->pos.y;
  Vec2 distance = entity->cTransform->pos.dist(target);
  Vec2 normal_D = distance.normalized(distance);
  int speed_scalar = m_bulletConfig.S;
  Vec2 velocity = Vec2(normal_D.x * speed_scalar, normal_D.y * speed_scalar);
  bullet->cTransform = std::make_shared<CTransform>(Vec2(origin_x, origin_y), velocity, 0.f);

  bullet->cShape = std::make_shared<CShape>(
      m_bulletConfig.SR, m_bulletConfig.V,
      sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
      sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);

  bullet->cCollission = std::make_shared<CCollision>(m_bulletConfig.CR);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
  // todo: implement your own special weapon
}

void Game::sMovement()
{
  // todo: implement all entity movement in this function
  //       you should read the m_player->cInput component to determine if the
  //       player is moving

  // Sample movement speed update
  // m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
  // m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
  m_player->cTransform->velocity = Vec2(0, 0);
  auto player_input = m_player->cInput;
  auto &player_velocity = m_player->cTransform->velocity;

  if (player_input->up && !player_input->right && !player_input->left)
  {
    player_velocity.y -= m_playerConfig.S;
  }
  else if (player_input->down && !player_input->left && !player_input->right)
  {
    player_velocity.y += m_playerConfig.S;
  }
  else if (player_input->left && !player_input->up && !player_input->down)
  {
    player_velocity.x -= m_playerConfig.S;
  }
  else if (player_input->right && !player_input->up && !player_input->down)
  {
    player_velocity.x += m_playerConfig.S;
  }
  else if (player_input->right && player_input->up)
  {
    float dx = m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    float dy = -m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    player_velocity = Vec2(dx, dy);
  }
  else if (player_input->right && player_input->down)
  {
    float dx = m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    float dy = m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    player_velocity = Vec2(dx, dy);
  }
  else if (player_input->left && player_input->up)
  {
    float dx = -m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    float dy = -m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    player_velocity = Vec2(dx, dy);
  }
  else if (player_input->left && player_input->down)
  {
    float dx = -m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    float dy = m_playerConfig.S * std::cosf(std::numbers::pi / 4);
    player_velocity = Vec2(dx, dy);
  }

  // player against wall check
  for (auto e : m_entities.getEntities())
  {
    e->cTransform->pos.x += e->cTransform->velocity.x;
    e->cTransform->pos.y += e->cTransform->velocity.y;
  }
  if (0 > m_player->cTransform->pos.x - m_player->cShape->circle.getRadius() ||
      m_window.getSize().x < m_player->cTransform->pos.x + m_player->cShape->circle.getRadius())
  {
    m_player->cTransform->pos.x -= player_velocity.x;
  }
  if (0 > m_player->cTransform->pos.y - m_player->cShape->circle.getRadius() ||
      m_window.getSize().y < m_player->cTransform->pos.y + m_player->cShape->circle.getRadius())
  {
    m_player->cTransform->pos.y -= player_velocity.y;
  }
}

void Game::sLifespan()
{
  // todo: implement all lifespan functionality
  // for all entities
  //     if entity has no lifespan component, skip it
  //     if entity has > 0 remaining lifespan, subtract 1
  //     if it has lifespan and is_alive, scale its alpha channel properly
  //     if it has lifespan and its time is up, destroy the entity
  for (auto e : m_entities.getEntities())
  {
    if (e->cLifespan)
    {
      if (e->cLifespan->remaining > 0)
      {
        e->cLifespan->remaining -= 1;
      }
    }
  }
}

void Game::sCollision()
{
  // enemy-player collision check
  for (auto e : m_entities.getEntities("enemy"))
  {
    Vec2 dv = m_player->cTransform->pos.dist(e->cTransform->pos);
    float r1 = m_player->cCollission->radius;
    float r2 = e->cCollission->radius;
    if ((dv.x * dv.x + dv.y * dv.y) < (r1 * r2 + r1 * r2))
    {
      std::cout << "We have a collison ";
      // m_player->destroy();
    }
  }
  // bullet enemy collision
  for (auto bullet : m_entities.getEntities("bullet"))
  {
    for (auto enemy : m_entities.getEntities("enemy"))
    {
      Vec2 dv = bullet->cTransform->pos.dist(enemy->cTransform->pos);
      float bullet_radius = bullet->cCollission->radius;
      float enemy_radius = enemy->cCollission->radius;
      if ((dv.x * dv.x + dv.y * dv.y) <
          (bullet_radius * enemy_radius + bullet_radius * enemy_radius))
      {
        bullet->destroy();
        enemy->destroy();
      }
    }
  }
}

void Game::sEnemySpawner()
{
  // todo: code which implements enemy spawning should go here
  // if (60 < m_currentFrame - m_lastEnemySpawnTime) {
  // spawnEnemy();
  // }
}

void Game::sGUI()
{
  ImGui::Begin("Geometry Wars");
  ImGui::Text("Stuff Goes Here");
  ImGui::End();
}

void Game::sRender()
{
  // todo: change the code below to draw ALL of the entities
  //       sample drawing of the player entity that we have created
  m_window.clear();

  // set the position of the shape based on the entity's transform->pos
  for (auto e : m_entities.getEntities())
  {
    e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

    // set the rotation of the shape based on the entity's transform->angle
    // e->cTransform->angle += 1.0f;
    // e->cShape->circle.setRotation(e->cTransform->angle);

    // draw the entity's sf::CircleShape
    m_window.draw(e->cShape->circle);

    // draw the ui last
    ImGui::SFML::Render(m_window);
  }

  m_window.draw(m_text);
  m_window.display();
}

void Game::sUserInput()
{
  // todo: handle user input here
  //     note that you should only be setting the player's input component
  //     variables here, you should not implement the player's movemnet
  //     logic here, the movement system will read the variables you set in this
  //     function

  sf::Event event;
  while (m_window.pollEvent(event))
  {
    ImGui::SFML::ProcessEvent(m_window, event);

    if (event.type == sf::Event::Closed)
    {
      m_running = false;
    }

    if (event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::W:
        m_player->cInput->up = true;
        break;
      case sf::Keyboard::S:
        m_player->cInput->down = true;
        break;
      case sf::Keyboard::D:
        m_player->cInput->right = true;
        break;
      case sf::Keyboard::A:
        m_player->cInput->left = true;
        break;
      case sf::Keyboard::Escape:
        m_running = false;
        m_window.close();
        break;
      default:
        break;
      }
    }

    if (event.type == sf::Event::KeyReleased)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::W:
        m_player->cInput->up = false;
        break;
      case sf::Keyboard::S:
        m_player->cInput->down = false;
        break;
      case sf::Keyboard::D:
        m_player->cInput->right = false;
        break;
      case sf::Keyboard::A:
        m_player->cInput->left = false;
        break;

      default:
        break;
      }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
      // this line ignores mouse events if ImGui is the thing clicked
      if (ImGui::GetIO().WantCaptureMouse)
      {
        continue;
      }

      if (event.mouseButton.button == sf::Mouse::Left)
      {
        int x = event.mouseButton.x;
        int y = event.mouseButton.y;
        std::cout << "Left Mouse Button Clicked at (" << x << "," << y << ")\n";
        spawnBullet(m_player, Vec2(x, y));
      }

      if (event.mouseButton.button == sf::Mouse::Right)
      {
        std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ","
                  << event.mouseButton.y << ")\n";
        // call spawnSpecialWeapon here
      }
    }
  }
}
