// main.cpp
// created by samad and maryam
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main() {
    int width = sf::VideoMode::getDesktopMode().width / 2;
    int height = sf::VideoMode::getDesktopMode().height / 2;
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Explorer");

    ComplexPlane complexPlane(width, height);
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text text("", font, 14);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.f, 10.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved)
                complexPlane.setMouseLocation({event.mouseMove.x, event.mouseMove.y});

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    complexPlane.zoomIn();
                    complexPlane.setCenter({event.mouseButton.x, event.mouseButton.y});
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    complexPlane.zoomOut();
                    complexPlane.setCenter({event.mouseButton.x, event.mouseButton.y});
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        complexPlane.updateRender();
        complexPlane.loadText(text);

        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}

