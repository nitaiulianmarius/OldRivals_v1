#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "introState.h"

using namespace std;

int main()
{
    sf::RenderWindow *window;
    std::vector<stateMachine*> myState;

    window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Old Rivals v1", sf::Style::Fullscreen);
    //window = new sf::RenderWindow(sf::VideoMode(800, 600), "Old Rivals v1");
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(true);

    myState.push_back(new introState(window));

    while(window->isOpen())
    {
        myState[myState.size()-1]->handleEvent(myState);
        myState[myState.size()-1]->update(myState);
        window->clear();
        myState[myState.size()-1]->render();
        window->display();
    }
}
