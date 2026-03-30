#include <SFML/Graphics.hpp>
#include <list>
#include "spacebody.hpp"
#include <random>

int main() {
    const int starCount = 2000;
    sf::VertexArray stars(sf::PrimitiveType::Points, starCount);

    sf::RenderWindow window(sf::VideoMode({800, 800}), "Advanced Solar System");
    window.setFramerateLimit(60);
    sf::Clock clock;

    std::mt19937 gen(static_cast<unsigned>(time(nullptr)));
    std::uniform_real_distribution<float> dist(-5000.f, 5000.f);
    std::uniform_int_distribution<int> colorDist(150, 255);

    sf::View camera(sf::FloatRect({0.f, 0.f}, {800.f, 800.f}));
    float zoomLevel = 1.0f;

    for (int i = 0; i < starCount; ++i) {
        stars[i].position = {dist(gen), dist(gen)};

        uint8_t brightness = static_cast<uint8_t>(colorDist(gen));
        stars[i].color = sf::Color(brightness, brightness, brightness);
    }

    SpaceBody sun(40.f, sf::Color::Yellow, 0.f, 0.f);

    std::list<SpaceBody> bodies;
    bodies.emplace_back(SpaceBody(7.f, sf::Color::Red, 50.0f, 2.5f, &sun));
    bodies.emplace_back(SpaceBody(9.f, sf::Color::Blue, 100.0f, 2.0f, &sun));
    bodies.emplace_back(SpaceBody(15.f, sf::Color::Green, 150.f, 1.5f, &sun));    // Earth
    bodies.back().orbitRadii = {200.f, 150.f};
    bodies.emplace_back(SpaceBody(5.f, sf::Color::White, 30.f, 2.5f, &bodies.back())); // Moon (orbits Earth)
    bodies.emplace_back(SpaceBody(20.f, sf::Color::Red, 280.f, 1.0f, &sun));     // Mars

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Fix: Move pollEvent into the loop condition
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Move the zoom logic inside here so it's only checked when an event happens
            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
                    float zoomFactor = (mouseWheel->delta > 0) ? 0.9f : 1.1f;
                    camera.zoom(zoomFactor);
                    zoomLevel *= zoomFactor;
                }
            }
        }

        // Update all
        for (auto& b : bodies) b.update(dt);
        float cameraSpeed = 200.f * dt; // Adjust by delta time
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  camera.move({-cameraSpeed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) camera.move({cameraSpeed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    camera.move({0.f, -cameraSpeed});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  camera.move({0.f, cameraSpeed});


        window.clear(sf::Color(5, 5, 15));
        window.setView(camera);
        window.draw(stars);
        sun.draw(window);

        for (auto& b : bodies) {
            b.draw(window);
        }
        window.display();
    }
    return 0;
}
