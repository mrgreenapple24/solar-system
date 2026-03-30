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
    sf::Text label;
    std::string name;

    SpaceBody(const sf::Font& font,std::string n,float size, sf::Color color, float dist, float speed, SpaceBody* p = nullptr)
        : orbitRadius(dist), orbitSpeed(speed), angle(0.f), parent(p), label(font, n), name(n) {

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

        if (color == sf::Color(200, 230, 255)) { // If it's our comet color
            shape.setOutlineThickness(2.f);
            shape.setOutlineColor(sf::Color(255, 255, 255, 150));
        }
    }

    void update(float dt, float zoomLevel) {
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

        label.setPosition({shape.getPosition().x, shape.getPosition().y + 20.0f});
        label.setScale({zoomLevel, zoomLevel});
    }

    void draw(sf::RenderWindow& window) {
        if (parent) {
            window.draw(outline);
        }
        window.draw(shape);
        window.draw(label);
    }
};
