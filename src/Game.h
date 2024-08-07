#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

struct PlayerConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
  float S;
};
struct EnemyConfig {
  int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
  float SMIN, SMAX;
};
struct BulletConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S;
};

class Game {
  sf::RenderWindow m_window; // the window be drawn to
  EntityManager m_entities;  // vector of entities to maintain
  sf::Font m_font;
  sf::Text m_text; // the score text
  PlayerConfig m_playerConfig;
  EnemyConfig m_enemyConfig;
  BulletConfig m_bulletConfig;
  sf::Clock m_deltaClock;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  int m_spawnInterval = 60;
  bool m_paused = false;
  bool m_running = true;

  std::shared_ptr<Entity> m_player;

  void init(const std::string &config);
  void setPaused(bool paused);

  void sMovement();
  void sUserInput();
  void sLifespan();
  void sRender();
  void sEnemySpawner();
  void sCollision();
  void sGUI();

  void spawnPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

  int rng(int min, int max);

public:
  Game(const std::string &config);
  void run();
};
