#include <SFML/Graphics.hpp>
#include <cmath>

class SpaceBody {
public:
    sf::CircleShape shape;
    float orbitRadius;
    float orbitSpeed;
    float angle;
    SpaceBody* parent;
    sf::CircleShape outline;

    SpaceBody(float size, sf::Color color, float dist, float speed, SpaceBody* p = nullptr)
        : orbitRadius(dist), orbitSpeed(speed), angle(0.f), parent(p) {
        shape.setRadius(size);
        shape.setFillColor(color);
        shape.setOrigin({size, size});

        outline.setRadius(this->orbitRadius);
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color(255, 255, 255, 50));
        outline.setOutlineThickness(1.f);

        outline.setOrigin({this->orbitRadius, this->orbitRadius});

        if (parent) {
            outline.setPosition(parent->shape.getPosition());
        } else {
            shape.setPosition({400.f, 400.f});
        }
    }

    void update(float dt) {
        angle += orbitSpeed * dt;

        if (parent) {
            // Calculate position relative to the parent
            float x = parent->shape.getPosition().x + orbitRadius * std::cos(angle);
            float y = parent->shape.getPosition().y + orbitRadius * std::sin(angle);
            shape.setPosition({x, y});

            outline.setPosition(parent->shape.getPosition());
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};
