#include "menuState.h"

void menuState::handleEvent(std::vector<stateMachine*>& myState)
{
    while(window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window->close();

        if(event.type == sf::Event::MouseButtonPressed &&
           event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mouseCoordinate(sf::Mouse::getPosition(*window));

            for(int i=0;i<button.size();i++)
                if(button[i]->wasClicked(mouseCoordinate))
                {
                    if(button[i]->getText() == "Un jucator")
                    {
                        myState.push_back(new playState(window, data, SINGLEPLAYER));
                        if(hostState)
                        {
                            hostState = false;
                        }

                        if(connectState)
                        {
                            connectState = false;
                            delete button[button.size()-1];
                            button.pop_back();
                        }

                        if(howToPlayState)
                                howToPlayState = false;
                    }

                    if(button[i]->getText() == "Creaza camera")
                        if(hostState == false)
                        {
                            if(connectState)
                            {
                                connectState = false;
                                delete button[button.size()-1];
                                button.pop_back();
                            }
                            if(howToPlayState)
                                howToPlayState = false;

                            hostState = true;
                            waitClientThread = new sf::Thread(&menuState::waitClient, this);
                            waitClientThread->launch();
                        }
                        else
                            hostState = false;

                    if(button[i]->getText() == "Intra in camera")
                        if(connectState == false)
                        {
                            if(hostState)
                                hostState = false;

                            if(howToPlayState)
                                howToPlayState = false;

                            connectState = true;
                            button.push_back(new simpleButton(window, 0.26*width, 0.3*height, 0.25*width, 0.1*height, "Intra", font, width/50));
                        }
                        else
                        {
                            connectState = false;
                            delete button[button.size()-1];
                            button.pop_back();
                        }

                    if(button[i]->getText() == "Cum se joaca")
                        if(howToPlayState == false)
                        {
                            if(hostState)
                            {
                                hostState = false;
                            }

                            if(connectState)
                            {
                                connectState = false;
                                delete button[button.size()-1];
                                button.pop_back();
                            }

                            howToPlayState = true;
                        }
                        else
                        {
                            howToPlayState = false;
                        }

                    if(button[i]->getText() == "Iesire")
                        window->close();

                    if(button[i]->getText() == "Intra")
                    {
                        waitServerThread = new sf::Thread(&menuState::waitServer, this);
                        waitServerThread->launch();
                    }
                }
        } // mouseButtonPressed

        if(event.type == sf::Event::TextEntered && connectState)
        {
            if(event.text.unicode == 8 && keyPressedIP.getSize())
                keyPressedIP.erase(keyPressedIP.getSize()-1);
            else
                keyPressedIP.insert(keyPressedIP.getSize(), event.text.unicode);
        }
    } // pollEvent


}

void menuState::update(std::vector<stateMachine*>& myState)
{
    for(int i=0; i<button.size(); i++)
        button[i]->update();

    if(hostState || connectState)
        if(socket->send("test", 4) == sf::Socket::Done)
        {
            myState.push_back(new playState(window, data, MULTIPLAYER, socket));
            if(connectState)
            {
                connectState = false;
                delete button[button.size()-1];
                button.pop_back();
            }
            if(hostState)
            {
                hostState = false;
            }
            listener.close();
        }

    mouseSprite.setPosition((sf::Vector2f)sf::Mouse::getPosition(*window));
}

void menuState::render()
{
    window->draw(backgroundSprite);
    window->draw(logoSprite);

    for(int i=0; i<button.size(); i++)
        button[i]->render();

    if(howToPlayState == true)
    {
        sf::RectangleShape rectangle(sf::Vector2f(0.5*width,0.6*height));
        rectangle.setPosition(0.26*width, 0.2*height);
        rectangle.setFillColor(sf::Color(255,255,255,180));
        window->draw(rectangle);
        window->draw(howToPlaySprite);
    }

    if(hostState == true)
    {
        outtextxy(0.26*width, 0.2*height, globalIP, sf::Color(255,255,255), width/50);
        outtextxy(0.26*width, 0.25*height, localIP, sf::Color(255,255,255), width/50);
        outtextxy(0.26*width, 0.3*height, "Astept rival !", sf::Color(255,0,0), width/50);
    }

    if(connectState == true)
    {
        sf::String temp = "IP Camera: ";
        temp.insert(temp.getSize(), keyPressedIP);
        outtextxy(0.26*width, 0.2*height, temp, sf::Color(255, 255, 255), width/50);
    }

    window->draw(mouseSprite);

}

void menuState::waitServer()
{
    int status;
    while(status = socket->connect(keyPressedIP.toAnsiString(), 27015) != sf::Socket::Done &&  status != sf::Socket::Error && connectState) Sleep(10);
}

void menuState::waitClient()
{
    listener.listen(27015);
    while(listener.accept(*socket) != sf::Socket::Done && hostState) Sleep(10);
}
