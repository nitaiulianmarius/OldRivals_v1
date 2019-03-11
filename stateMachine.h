/*
Clasa stateMachine ma ajuta sa fac rapid si simplu schimbul intre ferestre (de la intro la meniul principal etc si invers)
Folosesc variabilele protected pentru a nu le redeclara in clasele derivate acesteia: introState, menuState etc
In firul principal de executie ma folosesc de functiile (pur virtuale) handleEvent, update si render ca functii principale
 a oricarei stari de joc
*/

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "dataManager.h"

#define SINGLEPLAYER 1
#define MULTIPLAYER 2

class stateMachine
{
protected:
    dataManager *data;
    sf::RenderWindow *window;
    sf::TcpSocket *socket;
    sf::Event event;
    sf::Text text;
    sf::Font font;
    sf::Sprite mouseSprite;
    int width, height;

public:
    stateMachine(){};

    virtual void handleEvent(std::vector<stateMachine*>&) = 0;
    virtual void update(std::vector<stateMachine*>&) = 0;
    virtual void render() = 0;

    void outtextxy(int x, int y, sf::String string, sf::Color color = { 255, 255, 255, 255 }, int size = 16)
    {
        text.setPosition(x, y);
        text.setString(string);
        text.setColor(color);
        text.setCharacterSize(size);
        window->draw(text);
    };
};
#endif // STATEMACHINE_H
