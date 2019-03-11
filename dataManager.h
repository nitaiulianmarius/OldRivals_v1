/*
La inceputul executiei programului incarc toate datele necesare rularii acestuia
Prin clasa dataManager retin permanent un pointer catre adresa de memorie a fiecarei data, astfel nu mai sunt nevoit
 sa dealoc memoria la terminarea unei anumite stari de joc, iar mai apoi sa o realoc
 (de la startGameState->menuState->startGameState si tot asa)
Aceasta varianta de alocare a memoriei este eficienta pentru jocuri care ocupa, in general, putina memorie !!
*/

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#define FONT 0
#define MENUBACKGROUND 1
#define GAMEBACKGROUND 2
#define HOWTOPLAY 3
#define LOGO 4
#define PLAYER 5
#define MOUSE 6
#define SHIELD 7
#define VIEW 8
#define HEART 9
#define FIRE 10
#define POWER 11
#define COOLDOWN 12
#define RIVALFIRE 13
#define UPARROW 14

class dataManager
{
public:
    sf::Font font;
    sf::Sprite sprite[100];
    sf::Texture texture[100];
    int width, height;
    bool loaded = false;

    dataManager(sf::RenderWindow *window)
    {
        width = window->getSize().x;
        height = window->getSize().y;

        font.loadFromFile("data/font/madison.ttf");

        texture[MENUBACKGROUND].loadFromFile("data/image/menuBackground.jpg");
        sprite[MENUBACKGROUND].setTexture(texture[MENUBACKGROUND]);
        sprite[MENUBACKGROUND].setScale(width/sprite[MENUBACKGROUND].getGlobalBounds().width,
                                  height/sprite[MENUBACKGROUND].getGlobalBounds().height);

        texture[GAMEBACKGROUND].loadFromFile("data/image/gameBackground.png");
        sprite[GAMEBACKGROUND].setTexture(texture[GAMEBACKGROUND]);
        sprite[GAMEBACKGROUND].setScale(0.9*width/sprite[GAMEBACKGROUND].getGlobalBounds().width,
                                        0.8*height/sprite[GAMEBACKGROUND].getGlobalBounds().height);
        sprite[GAMEBACKGROUND].setPosition(0, 0.2*height);

        texture[HOWTOPLAY].loadFromFile("data/image/howToPlay.png");
        sprite[HOWTOPLAY].setTexture(texture[HOWTOPLAY]);
        sprite[HOWTOPLAY].setScale(0.45*width/sprite[HOWTOPLAY].getGlobalBounds().width,
                                   0.55*height/sprite[HOWTOPLAY].getGlobalBounds().height);

        texture[LOGO].loadFromFile("data/image/logo.png");
        sprite[LOGO].setTexture(texture[LOGO]);
        sprite[LOGO].setScale(0.25*width/sprite[LOGO].getGlobalBounds().width,
                            0.13*height/sprite[LOGO].getGlobalBounds().height);

        texture[PLAYER].loadFromFile("data/image/player.png");

        texture[MOUSE].loadFromFile("data/image/mouse.png");
        sprite[MOUSE].setTexture(texture[MOUSE]);
        sprite[MOUSE].setScale(35/sprite[MOUSE].getGlobalBounds().width,
                               35/sprite[MOUSE].getGlobalBounds().height);

        texture[SHIELD].loadFromFile("data/image/shield.png");
        sprite[SHIELD].setTexture(texture[SHIELD]);
        sprite[SHIELD].setScale(0.07*width/sprite[SHIELD].getGlobalBounds().width,
                               0.1*height/sprite[SHIELD].getGlobalBounds().height);
        sprite[SHIELD].setPosition(0.915*width, 0.4*height);

        texture[VIEW].loadFromFile("data/image/view.png");
        sprite[VIEW].setTexture(texture[VIEW]);
        sprite[VIEW].setScale(0.07*width/sprite[VIEW].getGlobalBounds().width,
                               0.1*height/sprite[VIEW].getGlobalBounds().height);
        sprite[VIEW].setPosition(0.915*width, 0.55*height);

        texture[HEART].loadFromFile("data/image/heart.png");
        sprite[HEART].setTexture(texture[HEART]);
        sprite[HEART].setScale(0.04*width/sprite[HEART].getGlobalBounds().width,
                               0.04*height/sprite[HEART].getGlobalBounds().height);
        sprite[HEART].setPosition(0.06*width, 0.02*height);

        texture[FIRE].loadFromFile("data/image/fire.png");
        sprite[FIRE].setTexture(texture[FIRE]);
        sprite[FIRE].setOrigin(0.5*sprite[FIRE].getGlobalBounds().width,
                               0.5*sprite[FIRE].getGlobalBounds().height);
        sprite[FIRE].setScale(0.17*width/sprite[FIRE].getGlobalBounds().width, 0.17*height/sprite[FIRE].getGlobalBounds().height);

        texture[RIVALFIRE].loadFromFile("data/image/rivalFire.png");
        sprite[RIVALFIRE].setTexture(texture[RIVALFIRE]);
        sprite[RIVALFIRE].setOrigin(0.5*sprite[RIVALFIRE].getGlobalBounds().width,
                               0.5*sprite[RIVALFIRE].getGlobalBounds().height);
        sprite[RIVALFIRE].setScale(0.17*width/sprite[RIVALFIRE].getGlobalBounds().width, 0.17*height/sprite[RIVALFIRE].getGlobalBounds().height);

        texture[COOLDOWN].loadFromFile("data/image/cooldown.png");
        sprite[COOLDOWN].setTexture(texture[COOLDOWN]);
        sprite[COOLDOWN].setScale(0.04*width/sprite[COOLDOWN].getGlobalBounds().width,
                               0.04*height/sprite[COOLDOWN].getGlobalBounds().height);
        sprite[COOLDOWN].setPosition(0.06*width, 0.08*height);

        texture[POWER].loadFromFile("data/image/power.png");
        sprite[POWER].setTexture(texture[POWER]);
        sprite[POWER].setScale(0.04*width/sprite[POWER].getGlobalBounds().width,
                               0.04*height/sprite[POWER].getGlobalBounds().height);
        sprite[POWER].setPosition(0.06*width, 0.14*height);

        texture[UPARROW].loadFromFile("data/image/upArrow.png");
        sprite[UPARROW].setTexture(texture[UPARROW]);
        sprite[UPARROW].setScale(0.1*width/sprite[UPARROW].getGlobalBounds().width,
                                0.1*height/sprite[UPARROW].getGlobalBounds().height);

        loaded = true;
    }

    bool isReady()
    {
        return loaded;
    }
};

#endif // DATAMANAGER_H
