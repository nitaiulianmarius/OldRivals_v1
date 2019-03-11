#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp"
#include "AnimatedSprite.h"
#include "dataManager.h"
#include "iostream"

class Player
{
    sf::Clock frameClock;
    sf::Time frameTime;
    sf::RenderWindow *window;
    dataManager *data;
    Animation walkingAnimationDown;
    Animation walkingAnimationLeft;
    Animation walkingAnimationRight;
    Animation walkingAnimationUp;
    Animation loseSprite;
    Animation winSprite;
    Animation* currentAnimation;
    AnimatedSprite animatedSprite;
    float speed;
    float health;
    bool noKeyWasPressed;
    int width, height;

public:
    Player(){};

    Player(sf::RenderWindow *_window, dataManager *_data)
    {
        window = _window;
        data = _data;

        width = window->getSize().x;
        height = window->getSize().y;

        walkingAnimationDown.setSpriteSheet(data->texture[PLAYER]);
        walkingAnimationDown.addFrame(sf::IntRect(1, 1, 27, 34));
        walkingAnimationDown.addFrame(sf::IntRect(33, 1, 27, 34));
        walkingAnimationDown.addFrame(sf::IntRect(65, 1, 27, 34));
        walkingAnimationDown.addFrame(sf::IntRect(97, 1, 27, 36));
        walkingAnimationDown.addFrame(sf::IntRect(129, 1, 27, 34));
        walkingAnimationDown.addFrame(sf::IntRect(161, 1, 27, 34));
        walkingAnimationDown.addFrame(sf::IntRect(193, 1, 27, 34));
        walkingAnimationDown.addFrame(sf::IntRect(224, 1, 27, 34));
        walkingAnimationDown.addFrame(sf::IntRect(257, 1, 27, 36));
        walkingAnimationDown.addFrame(sf::IntRect(289, 1, 27, 35));

        walkingAnimationRight.setSpriteSheet(data->texture[PLAYER]);
        walkingAnimationRight.addFrame(sf::IntRect(1, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(33, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(65, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(97, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(129, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(161, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(193, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(224, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(257, 41, 27, 34));
        walkingAnimationRight.addFrame(sf::IntRect(289, 41, 27, 34));

        walkingAnimationLeft.setSpriteSheet(data->texture[PLAYER]);
        walkingAnimationLeft.addFrame(sf::IntRect(1, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(33, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(65, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(97, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(129, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(161, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(193, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(224, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(257, 79, 27, 34));
        walkingAnimationLeft.addFrame(sf::IntRect(289, 79, 27, 34));

        walkingAnimationUp = walkingAnimationDown;

        winSprite.setSpriteSheet(data->texture[PLAYER]);
        winSprite.addFrame(sf::IntRect(1, 119, 27, 28));

        loseSprite.setSpriteSheet(data->texture[PLAYER]);
        loseSprite.addFrame(sf::IntRect(39, 119, 33, 28));

        currentAnimation = &walkingAnimationDown;

        animatedSprite = AnimatedSprite(sf::seconds(0.2), true, false);
        animatedSprite.setPosition(sf::Vector2f(0.4*width, 0.5*height));
        animatedSprite.setScale(0.003*width, 0.003*height);
        speed = 260;
        noKeyWasPressed = true;

        health = 100;
    };

    void update();
    void render();
    void winImage();
    void setHealth(float h);
    float getHealth();
    sf::Vector2f getPosition();

};
#endif // PLAYER_H
