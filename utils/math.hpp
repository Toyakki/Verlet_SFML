
#pragma once
#include <SFML/System/Vector2.hpp>

struct Math
{
    static constexpr float PI=3.1415936f;

    static sf::Vector2f dot(sf::Vector2f v1, sf::Vector2f v2)
    {
        return {v1.x * v2.x, v1.y * v2.y};
    }
};


// g++ -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib \
//     linear.cpp -o my_program \
//     -lsfml-graphics -lsfml-window -lsfml-system