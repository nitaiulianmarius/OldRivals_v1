#include "playState.h"

void playState::handleEvent(std::vector<stateMachine*>& myState)
{
    while(window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window->close();

        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            delete myState[myState.size()-1];
            delete player;
            myState.pop_back();
            if(modJoc == MULTIPLAYER)
            {
                socket->disconnect();
                socket->setBlocking(false);
            }
            backgroungMusic.stop();
        }

        if(event.type == sf::Event::MouseButtonPressed &&
           event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mouseCoordinate(sf::Mouse::getPosition(*window));

            // daca apas click stanga in arena, pornesc timer-ul pentru puterea atacului
            // folosesc isPressedOk pentru a putea stii cand se calculeaza corect puterea atacului
            if(mouseCoordinate.x < 0.9*width && clock()-fireTime >= 3000)
            {
                powerTime = clock();
                isPressedOk = true;
            }

            if(0.915*width <= mouseCoordinate.x && mouseCoordinate.x <= 0.985*width &&
               0.4*height <= mouseCoordinate.y && mouseCoordinate.y <= 0.5*height &&
               clock() - shieldCooldownTime > 8000 &&
               !shield)
            {
                shield = true;
                shieldTime = clock();
            }

            if(0.915*width <= mouseCoordinate.x && mouseCoordinate.x <= 0.985*width &&
               0.55*height <= mouseCoordinate.y && mouseCoordinate.y <= 0.65*height &&
               clock() - viewCooldownTime > 8000 &&
               modJoc == MULTIPLAYER &&
               !view)
            {
                view = true;
                viewTime = clock();
                socket->send("C", 1);
            }
        }

        if(event.type == sf::Event::MouseButtonReleased &&
           event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouseCoordinate(sf::Mouse::getPosition(*window));

            // daca am lasat click stanga in arena si asta s-a intamplat dupa ce am apasat click stanga in arena (isPressedOk = true)
            // creez imaginea de atac prin fireMovement (guverneaza miscarea in totalitate a atacului)
            // fireSprite reprezinta doar imaginea bilei
            if(mouseCoordinate.x < 0.9*width && clock()-fireTime >= 3000 && isPressedOk)
            {
                sf::Vector2f tempPosition;
                fireTime = clock();
                isPressedOk = false;
                if(player->getPosition().x <= 0.1*width)
                    tempPosition = sf::Vector2f(0.11*width, player->getPosition().y);
                else
                    tempPosition = player->getPosition();

                fireSprite.setPosition(tempPosition);
                fireMovement.init(tempPosition, mouseCoordinate, 600);
                myDamage = powerShapeLenght/8;
            }
        }

        // daca apas enter, folosesc chatul
        if(event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Return &&
           modJoc == MULTIPLAYER)
            if(chatState == true)
            {
                chatState = false;
                char s[128] = "Rival: ";
                for(int i=0; i<chatText.getSize(); i++)
                    s[i+7] = chatText[i];
                socket->send(s, sizeof(s));
                chatText.clear();
            }
            else
                chatState = true;

        if(event.type == sf::Event::TextEntered && chatState)
        {
            if(event.text.unicode == 8 && chatText.getSize())
                chatText.erase(chatText.getSize()-1);
            else
                if(chatText.getSize() < 64)
                    chatText.insert(chatText.getSize(), event.text.unicode);
        }
    }

    if(player->getHealth() <= 0 && !lostGame)
        lostGame = true, rendered = false;

    if(rivalHealth <= 0 && !winGame)
    {
        winGame = true;
        rendered = false;
        player->winImage();
    }
}

void playState::update(std::vector<stateMachine*>& myState)
{
    // lungimile barilor din stanga sus pentru viata, timpul intre atacuri si puterea atacului
    lifeShapeLenght = std::max(0, (int)(0.1*width*player->getHealth()/100));
    fireShapeLenght = std::min(0.1*width, 0.1*width*(clock()-fireTime)/3000);
    powerShapeLenght = sf::Mouse::isButtonPressed(sf::Mouse::Left) && isPressedOk ? std::min(0.1*width, 0.1*width*(clock()-powerTime)/1000) : 0;
    rivalShapeLenght = std::max(0, (int)(0.1*width*rivalHealth/100));


    lifeShape.setSize(sf::Vector2f(lifeShapeLenght, 0.01*height));
    fireShape.setSize(sf::Vector2f(fireShapeLenght, 0.01*height));
    powerShape.setSize(sf::Vector2f(powerShapeLenght, 0.01*height));
    rivalLifeShape.setSize(sf::Vector2f(rivalShapeLenght, 0.01*height));

    if(!chatState)
        player->update();

    // daca am generat un atac
    if(fireMovement.getExist())
    {
        // fac update la bila de atac
        fireSprite.setPosition(fireMovement.update(fireSprite.getPosition()));

        // verific coliziunea cu marginile arenei
        if(fireSprite.getPosition().x < 0.1*width)
            fireMovement.hitLeft(), myDamage/=2;

        if(fireSprite.getPosition().x > 0.8*width)
            fireMovement.hitRight(), myDamage/=2;

        if(fireSprite.getPosition().y > 0.78*height)
            fireMovement.hitDown(), myDamage/=2;

        if(fireSprite.getPosition().y < 0.3*height)
        {
            // daca bila ajunge in partea de sus, ajunge la rival
            fireMovement.setExist(false);

            if(modJoc == MULTIPLAYER)
            {
                char s[128];
                sprintf(s, "A %f %f %f %f %f %d %d %d", fireMovement.getFinish().x, fireMovement.getFinish().y, fireMovement.getStart().x, fireMovement.getStart().y, myDamage, fireMovement.getHitRight(), fireMovement.getHitDown(), fireMovement.getHitLeft());
                socket->send(s, sizeof(s));
            }

            // daca joc singleplayer, lovesc un rival fictiv cu o sansa de 75%
            if(modJoc == SINGLEPLAYER && rand()%100+1 <= 75)
                rivalHealth -= myDamage;
        }
    }

    // daca joc singleplayer, fac cpu sa "atace" la fiecare 3,5 secunde
    if(modJoc == SINGLEPLAYER && clock()-rivalTime >= 3500)
    {
        sf::Vector2f _start;
        sf::Vector2f _finish;

        _start.x = rand()%((int)(0.7*width)) + 0.1*width;
        _start.y = 0.31*height;

        _finish.x = rand()%((int)(0.7*width)) + 0.1*width;
        _finish.y = 0.8*height;

        rivalTime = clock();

        rivalSprite.setPosition(_start);
        rivalMovement.init(_start, _finish, 600);
        rivalDamage = rand()%29+1;
    }

    // daca am primit un atac din partea rivalului, fac update la fel ca si atacului generat de mine
    if(rivalMovement.getExist())
    {
        rivalSprite.setPosition(rivalMovement.update(rivalSprite.getPosition()));

        if(rivalSprite.getPosition().x < 0.1*width)
            rivalMovement.hitLeft(), rivalDamage/=2;

        if(rivalSprite.getPosition().x > 0.8*width)
            rivalMovement.hitRight(), rivalDamage/=2;

        if(rivalSprite.getPosition().y > 0.77*height)
            rivalMovement.hitDown(), rivalDamage/=2;

        if(rivalSprite.getPosition().y < 0.3*height)
            rivalMovement.setExist(false);

        // coliziunea dintre atacul rival si mine
        if(sqrt(pow(player->getPosition().x - rivalSprite.getPosition().x + 0.04*width, 2) + pow(player->getPosition().y - rivalSprite.getPosition().y + 0.04*width, 2)) <= 0.08*width)
        {
            if(!shield)
                player->setHealth(player->getHealth() - rivalDamage);
            rivalMovement.setExist(false);

            // trimit la oponent cata viata am
            if(modJoc == MULTIPLAYER && !shield)
            {
                char s[128];
                sprintf(s, "B %f", player->getHealth());
                socket->send(s, sizeof(s));
            }
        }
    }

    if(modJoc == MULTIPLAYER)
    {
        // s-a terminat timpul de efect pentru powerup
        if(view && clock() - viewTime > 3000)
        {
            view = false;
            viewCooldownTime = clock();
        }

        if(socket->send("test", 4) == sf::Socket::Disconnected && clock()-checkMessage > 3000)
        {
            checkMessage = clock();
            delete myState[myState.size()-1];
            delete player;
            myState.pop_back();
            socket->disconnect();
            socket->setBlocking(false);
            backgroungMusic.stop();
        }
    }

    // s-a terminat timpul de efect pentru powerup
    if(shield && clock() - shieldTime > 3000)
    {
        shield = false;
        shieldCooldownTime = clock();
    }

    // daca trebuie sa trimit la rival pozitia mea (are activat view)
    if(clock() - rivalViewTime < 3000 && modJoc == MULTIPLAYER)
    {
        char s[128];
        sprintf(s, "D %f", player->getPosition().x);
        socket->send(s, sizeof(s));
    }

    if((lostGame || winGame) && rendered)
    {
        backgroungMusic.setVolume(0);
        if(lostGame)
            loseMusic.play();
        else
            winMusic.play();
        Sleep(3000);
        backgroungMusic.setVolume(100);
        player->setHealth(100);
        rivalHealth = 100;
        lostGame = false;
        winGame = false;
    }

    mouseSprite.setPosition((sf::Vector2f)sf::Mouse::getPosition(*window));
}

void playState::render()
{
    window->draw(backgroundSprite);
    player->render();
    outtextxy(0.01*width, 0.01*height, "Tu:", sf::Color(255,255,255), width/40);
    outtextxy(0.23*width, 0.01*height, "Rival:", sf::Color(255,255,255), width/40);
    outtextxy(0.92*width, 0.95*height, "Esc - iesi", sf::Color(255,255,255,180), width/50);
    window->draw(shieldSprite);
    window->draw(viewSprite);
    heartSprite.setPosition(0.06*width, 0.02*height);
    window->draw(heartSprite);
    heartSprite.setPosition(0.3*width, 0.02*height);
    window->draw(heartSprite);
    window->draw(cooldownSprite);
    window->draw(powerSprite);
    window->draw(lifeShape);
    window->draw(rivalLifeShape);
    window->draw(fireShape);
    window->draw(powerShape);
    if(fireMovement.getExist())
        window->draw(fireSprite);
    if(rivalMovement.getExist())
        window->draw(rivalSprite);

    if(shield)
    {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color(0, 0, 0, 150));
        shape.setPosition(0.915*width, 0.5*height);
        shape.setSize(sf::Vector2f(0.07*width, -0.1*height*(clock()-shieldTime)/3000));
        window->draw(shape);

        sf::CircleShape circle;
        circle.setFillColor(sf::Color(0, 0, 255, 150));
        circle.setPosition(player->getPosition() - sf::Vector2f(0.01*width, 0.03*height));
        circle.setRadius(0.05*width);
        circle.setPointCount(5);
        window->draw(circle);
    }

    if(!shield && clock()-shieldCooldownTime < 8000)
    {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color(0, 0, 0, 150));
        shape.setPosition(0.915*width, 0.5*height);
        shape.setSize(sf::Vector2f(0.07*width, -0.1*height*(8000-clock()+shieldCooldownTime)/8000));
        window->draw(shape);
    }

    // daca folosesc view
    if(view)
    {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color(0, 0, 0, 150));
        shape.setPosition(0.915*width, 0.65*height);
        shape.setSize(sf::Vector2f(0.07*width, -0.1*height*(clock()-viewTime)/3000));
        window->draw(shape);
    }

    // timpul de incarcare pt view
    if(!view && clock()-viewCooldownTime < 8000)
    {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color(0, 0, 0, 150));
        shape.setPosition(0.915*width, 0.65*height);
        shape.setSize(sf::Vector2f(0.07*width, -0.1*height*(8000-clock()+viewCooldownTime)/8000));
        window->draw(shape);
    }

    if(clock() - viewTime < 3000)
        window->draw(upArrowSprite);

    if(clock() - rivalMessageTime < 8000)
        outtextxy(0.4*width, 0.05*height, rivalMessage.toAnsiString(), sf::Color(255, 255, 255), width/50);

    if(modJoc == MULTIPLAYER)
    {
        if(chatState)
        {
            sf::RectangleShape rectangle;
            rectangle.setPosition(0.4*width, 0.12*height);
            rectangle.setSize(sf::Vector2f(0.5*width, 0.07*height));
            rectangle.setFillColor(sf::Color(255, 255, 255, 20));
            window->draw(rectangle);

            outtextxy(0.4*width, 0.12*height, chatText.toAnsiString(), sf::Color(255, 255, 255), width/50);
        }
        outtextxy(0.7*width, 3, "Apasa 'enter' pentru a folosi chatul", sf::Color(255, 255, 255, 180), width/50);
    }

    window->draw(mouseSprite);
    rendered = true;
}

void playState::receiveMessageFromRival()
{
    socket->setBlocking(true);
    while(1)
    {
        char s[128];
        size_t received = 0;

        while(socket->receive(s, sizeof(s), received) != sf::Socket::Done) Sleep(10);


            // am primit proiectil rival
            if(s[0] == 'A')
            {
                char *nrChar;
                sf::Vector2f start, finish;

                nrChar = strtok(s, " ");
                nrChar = strtok(NULL, " ");
                start.x = atof(nrChar);
                nrChar = strtok(NULL, " ");
                start.y = atof(nrChar);
                nrChar = strtok(NULL, " ");
                finish.x = atof(nrChar);
                nrChar = strtok(NULL, " ");
                finish.y = atof(nrChar);
                nrChar = strtok(NULL, " ");
                rivalDamage = atoi(nrChar);

                rivalSprite.setPosition(start.x, 0.31*height);
                rivalMovement.init(start, finish, 600);
                rivalMovement.reverseHorizontal();

                if(atoi(strtok(NULL, " ")))
                    rivalMovement.hitRight();

                if(atoi(strtok(NULL, " ")))
                    rivalMovement.hitDown();

                if(atoi(strtok(NULL, " ")))
                    rivalMovement.hitLeft();
            }

            // am primit viata rivalului
            else if(s[0] == 'B')
            {
                char *nrChar;
                nrChar = strtok(s, " ");
                nrChar = strtok(NULL, " ");
                rivalHealth = atof(nrChar);
            }

            // am primit mesaj ca rivalul a activat view
            else if(s[0] == 'C')
            {
                rivalViewTime = clock();
            }

            // am primit pozitia rivalului
            else if(s[0] == 'D')
            {
                char *nrChar;
                nrChar = strtok(s, " ");
                nrChar = strtok(NULL, " ");
                upArrowSprite.setPosition(atof(nrChar), 0.2*height);
            }

            // am primit mesaj in chat
            else if(s[0] == 'R')
            {
                rivalMessage = s;
                rivalMessageTime = clock();
            }

        Sleep(10);
    }
}
