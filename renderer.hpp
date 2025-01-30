#pragma once
#include "solver.hpp"
#include <SFML/Graphics.hpp> // Explicit include for SFML 3.0

class Renderer
{
public:
    explicit Renderer(sf::RenderTarget& target)
        : m_target{target}
    {
    }

    void render(const Solver& solver) const
    {
        // Render constraint
        const sf::Vector3f constraint = solver.getConstraint();
        sf::CircleShape constraint_background(constraint.z); // Radius as constructor argument
        constraint_background.setOrigin({constraint.z, constraint.z});
        constraint_background.setFillColor(sf::Color::Black);
        constraint_background.setPosition({constraint.x, constraint.y});
        constraint_background.setPointCount(128); // Smooth circle with many points
        m_target.draw(constraint_background);

        // Render objects
        sf::CircleShape circle(1.0f); // Set radius during construction
        circle.setPointCount(32);    // Smooth circle with 32 points
        circle.setOrigin({1.0f, 1.0f}); // Origin at the center
        const auto& objects = solver.getObjects();

        for (const auto& obj : objects) {
            circle.setPosition(obj.position);                // Set circle position
            circle.setScale({obj.radius, obj.radius});       // Scale to match object radius
            circle.setFillColor(obj.color);                  // Set object-specific color
            m_target.draw(circle);                           // Render the circle
        }
    }

private:
    sf::RenderTarget& m_target;
    
};
