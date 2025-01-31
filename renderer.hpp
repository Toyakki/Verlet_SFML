#pragma once
#include "solver.hpp"
#include <SFML/Graphics.hpp> // Explicit include for SFML 3.0

class Renderer
{
public:
    explicit Renderer(sf::RenderTarget& target) : render_target_(target) {}

    void Render(const Solver& solver) const
    {
        DrawConstraint(solver);
        DrawObjects(solver);
    }

private:
    sf::RenderTarget& render_target_;

    void DrawConstraint(const Solver& solver) const{
        const sf::Vector3f constraint = solver.GetConstraint();
        sf::CircleShape constraint_background(constraint.z); // Radius as constructor argument
        constraint_background.setOrigin({constraint.z, constraint.z});
        constraint_background.setFillColor(sf::Color::Black);
        constraint_background.setPosition({constraint.x, constraint.y});
        constraint_background.setPointCount(128); // Smooth circle with many points
        render_target_.draw(constraint_background);
    }

    void DrawObjects(const Solver& solver) const {
        // Render objects
        sf::CircleShape circle(1.0f); // Set radius during construction
        circle.setPointCount(32);    // Smooth circle with 32 points
        circle.setOrigin({1.0f, 1.0f}); // Origin at the center
        const auto& objects = solver.GetObjects();

        for (const auto& obj : objects) {
            circle.setPosition(obj.position);                // Set circle position
            circle.setScale({obj.radius, obj.radius});       // Scale to match object radius
            circle.setFillColor(obj.color);                  // Set object-specific color
            render_target_.draw(circle);                           // Render the circle
        }
    }
};
