#include <SFML/Graphics.hpp>
#include <cstdio>
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
    float timeScale = 1.0f;

    for (int i = 0; i < starCount; ++i) {
        stars[i].position = {dist(gen), dist(gen)};

        uint8_t brightness = static_cast<uint8_t>(colorDist(gen));
        stars[i].color = sf::Color(brightness, brightness, brightness);
    }


    sf::Font font;
    if (!font.openFromFile("assets/SpaceFont.ttf")) {
        printf("Failed to load font\n");
        return 1;
    }

    SpaceBody sun(font, "Sun",40.f, sf::Color::Yellow, 0.f, 0.f);
    sun.label.setPosition({420.0f, 440.0f});

    std::list<SpaceBody> bodies;
    bodies.emplace_back(SpaceBody(font, "Mercury", 7.f, sf::Color::Red, 50.0f, 2.5f, &sun));
    bodies.emplace_back(SpaceBody(font, "Venus", 9.f, sf::Color::Blue, 100.0f, 2.0f, &sun));
    bodies.emplace_back(SpaceBody(font, "Earth", 15.f, sf::Color::Green, 150.f, 1.5f, &sun));    // Earth
    bodies.back().orbitRadii = {200.f, 150.f};
    bodies.emplace_back(SpaceBody(font,"Moon", 5.f, sf::Color::White, 30.f, 2.5f, &bodies.back())); // Moon (orbits Earth)
    bodies.emplace_back(SpaceBody(font,"Mars", 20.f, sf::Color::Red, 280.f, 1.0f, &sun));     // Mars
    bodies.emplace_back(SpaceBody(font,"Comet", 4.f, sf::Color(200, 230, 255), 400.f, 0.4f, &sun));
    bodies.back().orbitRadii = {600.f, 150.f};

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
                    float zoomFactor = (mouseWheel->delta > 0) ? 0.9f : 1.1f;
                    camera.zoom(zoomFactor);
                    zoomLevel *= zoomFactor;
                }
            }
        }

        for (auto& b : bodies) b.update(dt* timeScale, zoomLevel);
        float cameraSpeed = 200.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  camera.move({-cameraSpeed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) camera.move({cameraSpeed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    camera.move({0.f, -cameraSpeed});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  camera.move({0.f, cameraSpeed});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal)) { // The '+' key (usually needs Shift, but '=' is the base)
            timeScale += 0.2f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen)) { // The '-' key
            timeScale -= 0.2f;
            if (timeScale < 0.0f) timeScale = 0.0f; // Prevent reverse time (unless you want it!)
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
            timeScale = (timeScale == 0.0f) ? 1.0f : 0.0f; // Toggle Pause
        }


        window.clear(sf::Color(5, 5, 15));
        window.setView(camera);
        window.draw(stars);
        sun.draw(window);

        for (auto& b : bodies) {
            b.draw(window);
        }
        window.setView(window.getDefaultView());

        sf::Text hud(font, "Time Scale: " + std::to_string(timeScale).substr(0, 4) + "x");
        hud.setCharacterSize(20);
        hud.setFillColor(sf::Color::Cyan);
        hud.setPosition({10.f, 10.f});
        window.draw(hud);
        window.display();
    }
    return 0;
}
