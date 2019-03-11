#ifndef BUTTON_H
#define BUTTON_H

#include "SFML/Graphics.hpp"

#define nState 0
#define pState 1

class simpleButton
{
    sf::RenderWindow *window;
    sf::Font font;
    sf::Text text;
    sf::Color color;
    sf::String textString;
    int x, y;
    int xMouse, yMouse;
    int width, height;
    int state;
    int size;

public:

    simpleButton(sf::RenderWindow *_window, int _x, int _y, int _width, int _height, sf::String _text, sf::Font _font, int _size):
    window(_window),
    font(_font),
    x(_x),
    y(_y),
    width(_width),
    height(_height),
    textString(_text),
    size(_size)
    {
        text.setFont(font);
        text.setString(textString);
        text.setCharacterSize(size);
        text.setPosition(x+(width-text.getGlobalBounds().width)/2, y+(height-text.getGlobalBounds().height)/2.8);
    };

    void update();
    void render();
    bool wasClicked(sf::Vector2i);
    void setText(sf::String);
    sf::String getText();
};

#endif // BUTTON_H
