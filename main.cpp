#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp> // For SFML 3.0 event handling
// #include "solver.hpp"
// #include "renderer.hpp"
// #include "utils/number_generator.hpp"
// #include "utils/math.hpp"

static sf::Color getRainbow(float t)
{
    const float r = sint(t);
    const float g = sint(t + 0.33f * 2.0f * Math::PI);
}