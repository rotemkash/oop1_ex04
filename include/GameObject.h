#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
 public:
  GameObject();

  sf::Vector2f getPos();

  void setPosition(sf::Vector2f pos);

  bool isOnBoard();

 private:
  sf::Vector2f m_position;
  sf::Texture* m_texture;
  bool m_inBord;
};