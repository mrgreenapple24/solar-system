#include <SFML/Graphics.hpp>
#include <list>
#include "spacebody.hpp"
#include <random>

int main() {
    const int starCount = 500;
    sf::VertexArray stars(sf::PrimitiveType::Points, starCount);

    sf::RenderWindow window(sf::VideoMode({800, 800}), "Advanced Solar System");
    window.setFramerateLimit(60);
    sf::Clock clock;

    std::mt19937 gen(static_cast<unsigned>(time(nullptr)));
    std::uniform_real_distribution<float> dist(0.f, 800.f);
    std::uniform_int_distribution<int> colorDist(150, 255);

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
        while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

        // Update all
        for (auto& b : bodies) b.update(dt);


        window.clear(sf::Color(5, 5, 15));
        window.draw(stars);
        sun.draw(window);

        for (auto& b : bodies) {
            b.draw(window);
        }
        window.display();
    }
    return 0;
}
