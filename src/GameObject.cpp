#include "GameObject.h"

GameObject::GameObject() : m_inBord(false), m_texture(NULL), m_position(-1, -1) {}

sf::Vector2f GameObject::getPos() { return m_position; }

void GameObject::setPosition(sf::Vector2f pos) {
  m_inBord = true;
  m_position = pos;
}

bool GameObject::isOnBoard() { return m_inBord; }
