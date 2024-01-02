#include "Window.h"

Window::Window()
    : m_window(sf::VideoMode(MAIN_WIDTH, MAIN_HEIGHT), "Pacman Level Editor",
               sf::Style::Close | sf::Style::Titlebar),
      m_lastClick(SAVE),
      m_currObject(CLEAR) {
  // creat window menu
  for (int i = 0; i < OBJECTS_NUM; i++) {
    m_buttons[i].setSize(sf::Vector2f(BTN_SZ, BTN_SZ));
    m_buttons[i].setPosition(850.f, float(i * (BTN_SZ + PADDING)));
    m_buttons[i].setTexture(m_board.getTexture(Object(i)));
  }
}

/*
 * draw everything on the window
 */
void Window::drawNDisplay() {
  sf::RectangleShape menueBackground;
  menueBackground.setSize(sf::Vector2f(MENU_WIDTH, MENU_HEIGHT));
  menueBackground.setPosition(float(MAIN_WIDTH - MENU_WIDTH), 0.f);
  menueBackground.setFillColor(MENU_BACKGROUND_COLOR);

  m_window.clear(BACKGROUND_COLOR);

  m_window.draw(menueBackground);

  for (int i = 0; i < OBJECTS_NUM; i++) {
    m_window.draw(m_buttons[i]);
  }

  for (int i = 0; i < m_board.getNumRow(); i++) {
    for (int j = 0; j < m_board.getNumCol(); j++) {
      m_window.draw(m_board.getTile(i, j));
    }
  }

  m_window.display();
}

/*
 * run the program
 */
void Window::run() {
  drawNDisplay();

  while (m_window.isOpen()) {
    if (auto event = sf::Event{}; m_window.waitEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          m_window.close();
          break;

        case sf::Event::MouseButtonReleased:
          handleClick(event.mouseButton);
          break;

        case sf::Event::MouseMoved:
          for (int i = 0; i < m_board.getNumRow(); i++) {
            for (int j = 0; j < m_board.getNumCol(); j++) {
              if (m_board.getTile(i, j).getGlobalBounds().contains(
                      m_window.mapPixelToCoords(
                          {event.mouseMove.x, event.mouseMove.y}))) {
                m_board.setOutline(i, j);
              } else {
                m_board.resetOutline(i, j);
              }
            }
          }
      }
    }
    drawNDisplay();
  }
}

void Window::handleClick(const sf::Event::MouseButtonEvent& event) {
  for (int i = 0; i < OBJECTS_NUM; i++) {
    if (m_buttons[i].getGlobalBounds().contains(
            m_window.mapPixelToCoords({event.x, event.y}))) {
      if (i == CLEAR) {
        enlargeButton(i);
        m_window.clear();

        auto font = sf::Font();
        font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

        sf::Text inptMsg = sf::Text("PLEASE GO TO SHELL!", font);
        inptMsg.setPosition(
            m_window.mapPixelToCoords({MAIN_WIDTH / 2, MAIN_HEIGHT / 2}));
        m_window.draw(inptMsg);
        m_window.display();

        m_board.clear();
        return;
      }

      else if (i == SAVE) {
        enlargeButton(i);
        m_window.draw(m_buttons[i]);
        m_window.display();
        sf::sleep(sf::seconds(0.1f));
        reduceButton(i);
        m_board.saveBoardToFile();
        return;
      }

      // reduce last clicked button to orginal size
      reduceButton(m_lastClick);

      m_lastClick = i;
      m_currObject = i;

      // enlarge clicked button
      enlargeButton(i);
    }
  }
  m_board.handleBoard(m_currObject, m_window, event);
}

/*
* increase the button and immidiatly decrease it  
*/ 
void Window::enlargeButton(int i) {
  m_buttons[i].setSize(sf::Vector2f(CLICKED_BTN_SZ, CLICKED_BTN_SZ));
  // increase the button by pressing
  m_buttons[i].setOutlineThickness(OUTLINE_THICKNESS);
  m_buttons[i].setOutlineColor(OUTLINE_COLOR);
}

/*
* increase the button and immidiatly decrease it 
*/
void Window::reduceButton(int i) {
  m_buttons[i].setSize(sf::Vector2f(BTN_SZ, BTN_SZ));
  m_buttons[i].setOutlineThickness(0.f);
}