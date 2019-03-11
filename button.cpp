#include "button.h"

void simpleButton::update()
{
    sf::Vector2i mouseCoordinate = sf::Mouse::getPosition(*window);

    xMouse = mouseCoordinate.x;
    yMouse = mouseCoordinate.y;

    if (x < xMouse && xMouse < x + width && y < yMouse && yMouse < y + height)
    {
        color = sf::Color(255,128,0);
        state = pState;
    }

    else
    {
        color = sf::Color(255,255,255);
        state = nState;
    }
}

void simpleButton::render()
{
    if(state == pState)
    {
        sf::RectangleShape rectangle(sf::Vector2f(width,height));
        rectangle.setPosition(x,y);
        rectangle.setFillColor(sf::Color(255,255,255,20));
        window->draw(rectangle);
    }

    text.setColor(color);
    window->draw(text);
}

bool simpleButton::wasClicked(sf::Vector2i _mP)
{
    xMouse = _mP.x;
    yMouse = _mP.y;

    return x < xMouse && xMouse < x + width && y < yMouse && yMouse < y + height;
}

void simpleButton::setText(sf::String newText)
{
    textString = newText;
    text.setString(textString);
}

sf::String simpleButton::getText()
{
    return textString;
}
