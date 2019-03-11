/*
Incarca si asteapta afisarea datelor pe ecran pentru intro (rendered = true) iar apoi se asteapta
 incarcarea tuturor datelor necesare pe toata durata de executie a jocului
Face legatura cu menuState
*/

#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "SFML/Graphics.hpp"
#include "stateMachine.h"
#include "dataManager.h"
#include "menuState.h"

class introState:public stateMachine
{
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::string loadingString;
    std::string loadingState;
    bool rendered;

public:
    introState(sf::RenderWindow *_window)
    {
        window = _window;
        window->setMouseCursorVisible(false);

        width = window->getSize().x;
        height = window->getSize().y;

        loadingString = "Incarc datele...";
        loadingState = "";

        rendered = false;

        font.loadFromFile("data/font/madison.ttf");
        text.setFont(font);

        backgroundTexture.loadFromFile("data/image/introBackground.jpg");
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(width/backgroundSprite.getGlobalBounds().width,
                                  height/backgroundSprite.getGlobalBounds().height);

        data = nullptr;
    };

    void handleEvent(std::vector<stateMachine*>&);
    void update(std::vector<stateMachine*>&);
    void render();
};

#endif // INTROSTATE_H
