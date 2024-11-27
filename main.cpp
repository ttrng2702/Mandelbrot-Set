#include <SFML/Graphics.hpp>
#include <sstream>
#include "ComplexPlane.h"

int main() {
    // Get desktop resolution and create a window
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int width = desktop.width / 2;
    unsigned int height = desktop.height / 2;
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");

    // Create the complex plane
    ComplexPlane complexPlane(width, height);

    // Load font for text display
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    complexPlane.zoomIn();
                    complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    complexPlane.zoomOut();
                    complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                complexPlane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
            }
        }

        // Update and render
        complexPlane.updateRender();
        complexPlane.loadText(text);

        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}

