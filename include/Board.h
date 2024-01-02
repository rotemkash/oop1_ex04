#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GameObject.h"

const int MAIN_WIDTH = 950;
const int MAIN_HEIGHT = 850;
const float CLICKED_BTN_SZ = 75;
const int BTN_SZ = 65;
const int PADDING = 20;
const float MENU_WIDTH = 150.f;
const float MENU_HEIGHT = 850.f;

const float OUTLINE_THICKNESS = 2.f;

const sf::Color OUTLINE_COLOR(128, 128, 128);
const sf::Color TILE_COLOR(190, 190, 190);
const sf::Color MENU_BACKGROUND_COLOR(0, 200, 200);
const sf::Color BACKGROUND_COLOR(100, 255, 255);


const int OBJECTS_NUM = 10;

enum Object {
  COOKIE,
  DEMON,
  DOOR,
  ERASER,
  GIFT,
  KEY,
  PACMAN,
  WALL,
  SAVE,
  CLEAR
};

class Board {
 public:
  Board();
  void createBoard();

  sf::RectangleShape getTile(int row, int col) const;

  void handleBoard(int currObject, sf::RenderWindow& window,
                   const sf::Event::MouseButtonEvent& event);

  void setOutline(int i, int j);
  void resetOutline(int i, int j);

  sf::Texture* charToTexture(char c);
  Object indexTexture(sf::RectangleShape tile);
  char textureToChar(Object obj);
  sf::Texture* getTexture(Object type);
  void setTexture(int i, int j, sf::Texture* texture);

  void clearTile(int row, int col);
  void clear();

  void fileToBoard(std::ifstream& board);
  void saveBoardToFile();

  void checkIfExist(int currObject, int i, int j);
  int getNumRow() const;
  int getNumCol() const;

 private:
  int m_rows;
  int m_cols;

  std::string m_pictures[OBJECTS_NUM] = {"cookie", "demon", "door",   "eraser",
                                       "gift",   "key",   "pacman", "wall",
                                       "save",   "trash"};
  sf::Texture m_textures[OBJECTS_NUM];
  std::vector<std::vector<sf::RectangleShape>> m_board;

  GameObject m_pacman;
};