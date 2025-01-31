#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp> // For SFML 3.0 event handling
#include "solver.hpp"
#include "renderer.hpp"
#include "utils/number_generator.hpp"
#include "utils/math.hpp"

[[nodiscard]] static sf::Color GetRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * Math::PI);
    const float b = sin(t + 0.66f * 2.0f * Math::PI);
    return  {static_cast<uint8_t>(255.0f * r * r),
             static_cast<uint8_t>(255.0f * g * g),
             static_cast<uint8_t>(255.0f * b * b)};
}

void ProcessInput(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) 
            window.close();
    }
    
}

void SpawnObjects(Solver& solver, sf::Clock& clock, float object_spawn_delay,
                  float object_spawn_speed, const sf::Vector2f& object_spawn_position,
                  float object_min_radius, float object_max_radius, uint32_t max_objects_count, float max_angle) {
    if (solver.GetObjectsCount() < max_objects_count &&
        clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
            clock.restart();
            auto& object = solver.AddObject(object_spawn_position, RNGf::GetRange(object_min_radius, object_max_radius));
            const float t = solver.GetTime();
            const float angle = max_angle * std::sin(t) + Math::PI * 0.5f;
            solver.SetObjectVelocity(object, object_spawn_speed * sf::Vector2f{std::cos(angle), std::sin(angle)});
            object.color = GetRainbow(t);
        }
                  }

int32_t main(int32_t, char*[])
{
    constexpr int32_t kWindowWidth = 1000;
    constexpr int32_t kWindowHeight = 1000;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode({kWindowWidth, kWindowHeight}), "SFML window", sf::State::Windowed, settings);

    const uint32_t kFrameRate = 60;
    window.setFramerateLimit(kFrameRate);

    Solver solver;
    Renderer renderer(window);

    solver.SetConstraint(
        {static_cast<float>(kWindowWidth) * 0.5f, static_cast<float>(kWindowHeight) * 0.5f}, 
        450.0f);
    solver.SetSubStepsCount(8);
    solver.SetSimulationUpdateRule(kFrameRate);

    constexpr float kObjectSpawnDelay = 0.025f;
    constexpr float kObjectSpawnSpeed = 1200.0f;
    constexpr sf::Vector2f kObjectSpawnPosition = {500.0f, 200.0f};
    constexpr float kObjectMinRadius = 1.0f;
    constexpr float kObjectMaxRadius = 20.0f;
    constexpr uint32_t kMaxObjectsCount = 1000;
    constexpr float kMaxAngle = 1.0f;

    sf::Clock clock;

    while (window.isOpen()){
        ProcessInput(window);
        SpawnObjects(solver, clock, kObjectSpawnDelay, kObjectSpawnSpeed,
                    kObjectSpawnPosition, kObjectMinRadius, kObjectMaxRadius,
                    kMaxObjectsCount, kMaxAngle);
        solver.Update();
        window.clear(sf::Color::White);
        renderer.Render(solver);
        window.display();
    }

    return 0;
}