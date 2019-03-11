#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "SFML/Graphics.hpp"
#include "math.h"

class Movement
{
    sf::Vector2f start, finish;
    sf::Vector2f vect, normalizedVector;
    float lenght;
    float speed;
    int lastUpdate;
    bool moving;
    bool hitLeftB;
    bool hitRightB;
    bool hitUpB;
    bool hitDownB;
    bool reverseHorizontalB;

public:
    Movement()
    {
        moving = false;
    };

    void init(sf::Vector2f _start, sf::Vector2f  _finish, float _speed)
    {
        start = _start;
        finish = _finish;
        speed = _speed;
        vect = finish - start;
        lenght = sqrt( pow(finish.x - start.x, 2) + pow(finish.y - start.y, 2) );
        if(lenght > 0.01*speed)
        {
            normalizedVector = (sf::Vector2f)vect / lenght;
            moving = true;
            lastUpdate = clock();
            hitLeftB = false;
            hitRightB = false;
            hitUpB = false;
            hitDownB = false;
            reverseHorizontalB = false;
        }
    };

    sf::Vector2f update(sf::Vector2f position)
    {
        if(moving == true)
        {
            sf::Vector2f tempVect = normalizedVector;
            int hitCount = 0;

            if(hitLeftB)
                tempVect.x *= -1, hitCount++;
            if(hitRightB)
                tempVect.x *= -1, hitCount++;

            if(hitUpB)
                tempVect.y *= -1, hitCount++;
            if(hitDownB)
                tempVect.y *= -1, hitCount++;

            if(hitCount >=2)
                moving = false;

            if(reverseHorizontalB)
                tempVect.x *= -1;

            tempVect.x *= speed*(clock()-lastUpdate)/1000.;
            tempVect.y *= speed*(clock()-lastUpdate)/1000.;

            lastUpdate = clock();

            return position+tempVect;
        }
        return sf::Vector2f(0, 0);
    };

    bool getExist()
    {
        return moving;
    }

    void setExist(bool exist)
    {
        moving = exist;
    }

    sf::Vector2f getStart()
    {
        return start;
    }

    sf::Vector2f getFinish()
    {
        return finish;
    }

    void hitLeft()
    {
        hitLeftB = true;
    }

    void hitRight()
    {
        hitRightB = true;
    }

    void hitUp()
    {
        hitUpB = true;
    }

    void hitDown()
    {
        hitDownB = true;
    }

    bool getHitLeft()
    {
        return hitLeftB;
    }

    bool getHitRight()
    {
        return hitRightB;
    }

    bool getHitUp()
    {
        return hitUpB;
    }

    bool getHitDown()
    {
        return hitDownB;
    }

    void reverseHorizontal()
    {
        reverseHorizontalB = true;
    }
};
#endif // MOVEMENT_H
