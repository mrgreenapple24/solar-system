#include <SFML/Graphics.hpp>
#include <cmath>

class SpaceBody {
public:
    sf::Vector2f orbitRadii;
    sf::CircleShape shape;
    float orbitRadius;
    float orbitSpeed;
    float angle;
    SpaceBody* parent;
    sf::CircleShape outline;

    SpaceBody(float size, sf::Color color, float dist, float speed, SpaceBody* p = nullptr)
        : orbitRadius(dist), orbitSpeed(speed), angle(0.f), parent(p) {

        orbitRadii = {dist, dist}; // Default to circular

        shape.setRadius(size);
        shape.setFillColor(color);
        shape.setOrigin({size, size});

        outline.setRadius(this->orbitRadius);
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color(255, 255, 255, 50));
        outline.setOutlineThickness(1.f);
        outline.setOrigin({this->orbitRadius, this->orbitRadius});

        if (!parent) {
            shape.setPosition({400.f, 400.f});
        }
    }

    void update(float dt) {
        angle += orbitSpeed * dt;

        if (parent) {
            float x = parent->shape.getPosition().x + orbitRadii.x * std::cos(angle);
            float y = parent->shape.getPosition().y + orbitRadii.y * std::sin(angle);
            shape.setPosition({x, y});

            outline.setPosition(parent->shape.getPosition());
            outline.setRadius(orbitRadii.x);
            outline.setOrigin({orbitRadii.x, orbitRadii.x});
            outline.setScale({1.0f, orbitRadii.y / orbitRadii.x});
        }
    }

    void draw(sf::RenderWindow& window) {
        if (parent) {
                window.draw(outline);
            }
            window.draw(shape);
    }
};
