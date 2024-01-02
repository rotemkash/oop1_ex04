#pragma once
#include <SFML/Graphics.hpp>

#include "Board.h"


class Window {
 public:
  Window();
  void run();
  void handleClick(const sf::Event::MouseButtonEvent& event);
  void enlargeButton(int i);
  void reduceButton(int i);

  void drawNDisplay();

 private:
  sf::RenderWindow m_window;
  sf::RectangleShape m_buttons[OBJECTS_NUM];
  Board m_board;
  int m_currObject;
  int m_lastClick;
};