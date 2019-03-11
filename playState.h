#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "windows.h"
#include "stdio.h"
#include "stateMachine.h"
#include "player.h"
#include "movement.h"
#include "algorithm"
#include <iostream>

class playState:public stateMachine
{
    sf::Sprite backgroundSprite;
    sf::Sprite shieldSprite;
    sf::Sprite viewSprite;
    sf::Sprite heartSprite;
    sf::Sprite cooldownSprite;
    sf::Sprite powerSprite;
    sf::Sprite fireSprite;
    sf::Sprite rivalSprite;
    sf::Sprite upArrowSprite;
    sf::RectangleShape lifeShape;
    sf::RectangleShape rivalLifeShape;
    sf::RectangleShape fireShape;
    sf::RectangleShape powerShape;
    sf::Thread *receiveMessageFromRivalThread;
    sf::Music backgroungMusic;
    sf::Music winMusic;
    sf::Music loseMusic;
    sf::String chatText;
    sf::String rivalMessage;
    Movement fireMovement;
    Movement rivalMovement;
    Player *player;
    DWORD fireTime, powerTime;
    DWORD rivalTime; //cand ataca CPU
    DWORD shieldTime, shieldCooldownTime;
    DWORD viewTime, viewCooldownTime;
    DWORD rivalViewTime;
    DWORD checkMessage;
    DWORD rivalMessageTime;
    float lifeShapeLenght;
    float fireShapeLenght;
    float powerShapeLenght;
    float rivalShapeLenght;
    float rivalHealth;
    float myDamage;
    float rivalDamage;
    bool isPressedOk;
    bool lostGame;
    bool winGame;
    bool shield;
    bool view;
    bool rendered;
    bool chatState;
    int modJoc;


public:
    playState(sf::RenderWindow *_window, dataManager *_data, int _modJoc, sf::TcpSocket *_socket = nullptr)
    {
        window = _window;
        data = _data;
        modJoc = _modJoc;
        socket = _socket;

        width = window->getSize().x;
        height = window->getSize().y;

        text.setFont(data->font);

        backgroundSprite = data->sprite[GAMEBACKGROUND];
        mouseSprite = data->sprite[MOUSE];
        shieldSprite = data->sprite[SHIELD];
        viewSprite = data->sprite[VIEW];
        heartSprite = data->sprite[HEART];
        cooldownSprite = data->sprite[COOLDOWN];
        powerSprite = data->sprite[POWER];
        fireSprite = data->sprite[FIRE];
        rivalSprite = data->sprite[RIVALFIRE];
        upArrowSprite = data->sprite[UPARROW];

        backgroungMusic.openFromFile("data/sound/playBackgroundMusic.wav");
        backgroungMusic.setLoop(true);
        winMusic.openFromFile("data/sound/winSound.wav");
        loseMusic.openFromFile("data/sound/loseSound.wav");

        player = new Player(window, data);

        lifeShape.setPosition(0.11*width, 0.03*height);
        lifeShape.setFillColor(sf::Color::Red);
        rivalLifeShape.setPosition(0.35*width, 0.03*height);
        rivalLifeShape.setFillColor(sf::Color::Red);
        fireShape.setPosition(0.11*width, 0.09*height);
        fireShape.setFillColor(sf::Color::Blue);
        powerShape.setPosition(0.11*width, 0.15*height);
        powerShape.setFillColor(sf::Color::Yellow);

        rivalHealth = 100;
        rivalViewTime = -10000;
        viewTime = -10000;
        rivalTime = clock();
        srand(time(NULL));

        if(modJoc == MULTIPLAYER)
        {
            receiveMessageFromRivalThread = new sf::Thread(&playState::receiveMessageFromRival, this);
            receiveMessageFromRivalThread->launch();
        }

        backgroungMusic.play();

    };

    void handleEvent(std::vector<stateMachine*>&);
    void update(std::vector<stateMachine*>&);
    void render();
    void receiveMessageFromRival();
};

#endif // INTROSTATE_H
