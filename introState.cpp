#include "introState.h"

void introState::handleEvent(std::vector<stateMachine*>& myState)
{
    while(window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window->close();

        if(event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Space &&
           data->isReady() == true)
            myState.push_back(new menuState(window, data));
    }
}

void introState::update(std::vector<stateMachine*>& myState)
{
    if(rendered == true && data == nullptr)
        data = new dataManager(window);

    if(data != nullptr)
        if(data->isReady())
        {
            loadingString = "Incarc datele...GATA!";
            loadingState = "Apasa 'space' pentru a continua";
        }
}

void introState::render()
{
    window->draw(backgroundSprite);
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(0.25*width, height));
    rectangle.setFillColor(sf::Color(0,0,0,100));
    rectangle.setPosition(0.15*width,0);
    window->draw(rectangle);
    outtextxy(0.2*width, 0.4*height, "Creator / Programator:", sf::Color::Cyan, width/50);
    outtextxy(0.2*width, 0.4*height + 20, "Nita Iulian Marius", sf::Color::White, width/50);
    outtextxy(0.2*width, 0.8*height, loadingString, sf::Color::White, width/50);
    outtextxy(0.16*width, 0.8*height + 20, loadingState, sf::Color::White, width/50);
    rendered = true;
}
