#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "ComplexPlane.h"
#include <iostream>

using namespace std;
using namespace sf;

int main() {
    // Get desktop resolution and divide for smaller initial size
    unsigned int screenWidth = VideoMode::getDesktopMode().width / 2;
    unsigned int screenHeight = VideoMode::getDesktopMode().height / 2;

    // Create the render window
    RenderWindow window(VideoMode(screenWidth, screenHeight), "Mandelbrot Set");
    window.setFramerateLimit(60);

    // Load font
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Failed to load font!" << endl;
        return 1;
    }

    // Text for UI
    Text displayText;
    displayText.setFont(font);
    displayText.setCharacterSize(16);
    displayText.setFillColor(Color::White);

    // ComplexPlane object
    ComplexPlane complexPlane(screenWidth, screenHeight);

    // Main loop
    while (window.isOpen()) {
        // Handle Input
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    complexPlane.zoomIn();
                    complexPlane.setCenter({event.mouseButton.x, event.mouseButton.y});
                } else if (event.mouseButton.button == Mouse::Right) {
                    complexPlane.zoomOut();
                    complexPlane.setCenter({event.mouseButton.x, event.mouseButton.y});
                }
            } else if (event.type == Event::MouseMoved) {
                complexPlane.setMouseLocation({event.mouseMove.x, event.mouseMove.y});
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        // Update Scene
        complexPlane.updateRender();
        complexPlane.loadText(displayText);

        // Draw Scene
        window.clear();
        window.draw(complexPlane);
        window.draw(displayText);
        window.display();
    }

    return 0;
}
