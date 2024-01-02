#include "Board.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>

Board::Board() : m_rows(10), m_cols(10) {
  // load bord from file if file exist
  std::ifstream board;
  for (int i = 0; i < OBJECTS_NUM; i++) {
    m_textures[i].loadFromFile(m_pictures[i] + ".png");
  }
  if (std::filesystem::exists("Board.txt")) {
    board.open("Board.txt");
    board >> m_rows >> m_cols;
    if (m_rows == 0 || m_cols == 0) m_rows = m_cols = 10;
  }
  // if file doesn't exsist ask user for num of cols and rows
  else {
    std::cout << "enter number of rows and cols:" << std::endl;
    std::cin >> m_rows >> m_cols;
    if (m_rows == 0 || m_cols == 0) m_rows = m_cols = 10;
  }

  createBoard();
  if (board.is_open()) fileToBoard(board);
}

void Board::createBoard() {
  m_board.clear();
  // find right size for tiles
  float tileSize = (MAIN_HEIGHT - 100 - (OUTLINE_THICKNESS * m_cols)) /
                   std::max(m_cols, m_rows);

  for (int i = 0; i < m_rows; ++i) {
    auto line = std::vector<sf::RectangleShape>();

    for (int j = 0; j < m_cols; ++j) {
      auto rec = sf::RectangleShape();

      // draw board
      rec.setSize(sf::Vector2f(tileSize, tileSize));
      rec.setPosition(float(PADDING + j * (tileSize + OUTLINE_THICKNESS)),
                      float(PADDING + i * (tileSize + OUTLINE_THICKNESS)));
      rec.setFillColor(TILE_COLOR);

      line.push_back(rec);
    }
    m_board.push_back(line);
  }
}

sf::RectangleShape Board::getTile(int row, int col) const {
  return m_board[row][col];
}

int Board::getNumRow() const { return m_rows; }

int Board::getNumCol() const { return m_cols; }

void Board::setOutline(int i, int j) {
  m_board[i][j].setOutlineColor(sf::Color::Red);
  m_board[i][j].setOutlineThickness(OUTLINE_THICKNESS);
}
void Board::resetOutline(int i, int j) {
  m_board[i][j].setOutlineThickness(0.f);
}

void Board::handleBoard(int currObject, sf::RenderWindow& window,
                        const sf::Event::MouseButtonEvent& event) {
  for (int i = 0; i < m_rows; ++i) {
    for (int j = 0; j < m_cols; ++j) {
      if (m_board[i][j].getGlobalBounds().contains(
              window.mapPixelToCoords({event.x, event.y}))) {
        if (currObject == ERASER) {
          clearTile(i, j);
          return;
        }

        else if (currObject == CLEAR || currObject == SAVE)
          return;

        checkIfExist(currObject, i, j);
        m_board[i][j].setFillColor(sf::Color::White);
        m_board[i][j].setTexture(
            &m_textures[currObject]);  // put object on board
      }
    }
  }
}

void Board::checkIfExist(int currObject, int i, int j) {
  switch (currObject) {
    case PACMAN:
      if (m_pacman.isOnBoard())
        clearTile(m_pacman.getPos().y, m_pacman.getPos().x);
      m_pacman.setPosition(sf::Vector2f(j, i));
      break;
  }
}
void Board::setTexture(int i, int j, sf::Texture* texture) {
  m_board[i][j].setTexture(texture);
}

sf::Texture* Board::getTexture(Object type) { return &m_textures[type]; }

void Board::clearTile(int row, int col) {
  m_board[row][col].setTexture(nullptr);
  m_board[row][col].setFillColor(TILE_COLOR);
}

void Board::clear() {
  std::cout << "enter number of rows and cols:" << std::endl;
  std::cin >> m_rows >> m_cols;

  createBoard();
}

void Board::fileToBoard(std::ifstream& board) {
  char c;

  for (int i = 0; i < m_rows; ++i) {
    c = char(board.get());  // \n
    for (int j = 0; j < m_cols; ++j) {
      c = char(board.get());
      if (c != ' ') {
        if (c == 'a') {
          m_pacman.setPosition(sf::Vector2f(i, j));
        }
        sf::Texture* objectTexture = charToTexture(c);
        m_board[i][j].setFillColor(sf::Color::White);
        m_board[i][j].setTexture(objectTexture);
      }
    }
  }
}


void Board::saveBoardToFile() {
  std::ofstream board;
  board.open("Board.txt");

  if (board.is_open()) {
    board << m_rows << ' ' << m_cols << std::endl;

    for (int i = 0; i < m_rows; ++i) {
      for (int j = 0; j < m_cols; ++j) {
        if (m_board[i][j].getTexture() == NULL)
          board << ' ';
        else
          board << textureToChar(indexTexture(m_board[i][j]));
      }
      board << '\n';
    }

    board.close();
  }
}

// get texture and find the index
Object Board::indexTexture(sf::RectangleShape tile) {
  for (int i = 0; i < OBJECTS_NUM; ++i) {
    if (tile.getTexture() == &m_textures[i]) return Object(i);
  }
  return Object(-1);
}

sf::Texture* Board::charToTexture(char c) {
  switch (c) {
    case '*':
      return &m_textures[COOKIE];
    case '&':
      return &m_textures[DEMON];
    case 'D':
      return &m_textures[DOOR];
    case '$':
      return &m_textures[GIFT];
    case '%':
      return &m_textures[KEY];
    case 'a':
      return &m_textures[PACMAN];
    case '#':
      return &m_textures[WALL];
    default:
      break;
  }
}

char Board::textureToChar(Object obj) {
  switch (obj) {
    case COOKIE:
      return '*';
    case DEMON:
      return '&';
    case DOOR:
      return 'D';
    case GIFT:
      return '$';
    case KEY:
      return '%';
    case PACMAN:
      return 'a';
    case WALL:
      return '#';
    default:
      break;
  }
}
