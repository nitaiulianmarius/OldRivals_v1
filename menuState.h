#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "windows.h"
#include "stateMachine.h"
#include "button.h"
#include "dataManager.h"
#include "playState.h"

class menuState:public stateMachine
{
    bool howToPlayState = false;
    bool hostState = false;
    bool connectState = false;
    std::vector<simpleButton*> button;
    sf::Sprite backgroundSprite;
    sf::Sprite howToPlaySprite;
    sf::Sprite logoSprite;
    sf::TcpListener listener;
    sf::String globalIP;
    sf::String localIP;
    sf::String keyPressedIP;
    sf::Thread *waitServerThread;
    sf::Thread *waitClientThread;

public:
    menuState(sf::RenderWindow *_window, dataManager *_data)
    {
        window = _window;
        data = _data;

        width = window->getSize().x;
        height = window->getSize().y;

        font = data->font;
        text.setFont(font);

        backgroundSprite = data->sprite[MENUBACKGROUND];
        howToPlaySprite = data->sprite[HOWTOPLAY];
        logoSprite = data->sprite[LOGO];
        mouseSprite = data->sprite[MOUSE];

        howToPlaySprite.setPosition(0.28*width, 0.25*height);
        logoSprite.setPosition(0.5*(width-logoSprite.getGlobalBounds().width), 0.01*height);

        globalIP = "Adresa IP globala: "; globalIP.insert(19, sf::IpAddress::getPublicAddress().toString());
        localIP = "Adresa IP locala: "; localIP.insert(18, sf::IpAddress::getLocalAddress().toString());

        socket = new sf::TcpSocket;
        listener.setBlocking(false);
        socket->setBlocking(false);

        button.push_back(new simpleButton(window, 0.01*width, 0.3*height, 0.25*width, 0.1*height, "Un jucator", font, width/45));
        button.push_back(new simpleButton(window, 0.01*width, 0.4*height, 0.25*width, 0.1*height, "Creaza camera", font, width/45));
        button.push_back(new simpleButton(window, 0.01*width, 0.5*height, 0.25*width, 0.1*height, "Intra in camera", font, width/45));
        button.push_back(new simpleButton(window, 0.01*width, 0.6*height, 0.25*width, 0.1*height, "Cum se joaca", font, width/45));
        button.push_back(new simpleButton(window, 0.01*width, 0.7*height, 0.25*width, 0.1*height, "Iesire", font, width/45));
    };

    void handleEvent(std::vector<stateMachine*>&);
    void update(std::vector<stateMachine*>&);
    void render();
    void waitServer();
    void waitClient();
};

#endif // MENUSTATE_H
