#include "ComplexPlane.h"
#include <cmath>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixel_size(pixelWidth, pixelHeight),
      m_aspectRatio(static_cast<float>(pixelHeight) / pixelWidth),
      m_plane_center(0, 0),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio),
      m_zoomCount(0),
      m_State(State::CALCULATING),
      m_vArray(sf::Points, pixelWidth * pixelHeight) {}

void ComplexPlane::updateRender() {
    if (m_State == State::CALCULATING) {
        for (unsigned int i = 0; i < m_pixel_size.y; ++i) {
            for (unsigned int j = 0; j < m_pixel_size.x; ++j) {
                size_t idx = j + i * m_pixel_size.x;
                m_vArray[idx].position = { static_cast<float>(j), static_cast<float>(i) };

                sf::Vector2f coord = mapPixelToCoords({ static_cast<int>(j), static_cast<int>(i) });
                size_t iterations = countIterations(coord);

                sf::Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);
                m_vArray[idx].color = { r, g, b };
            }
        }
        m_State = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    m_plane_size.x = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    m_plane_size.x = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
    m_plane_center = mapPixelToCoords(mousePixel);
    m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text) {
    std::ostringstream ss;
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Size: (" << m_plane_size.x << ", " << m_plane_size.y << ")\n";
    ss << "Mouse: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")";
    text.setString(ss.str());
}

size_t ComplexPlane::countIterations(sf::Vector2f coord) {
    std::complex<float> c(coord.x, coord.y);
    std::complex<float> z = c;
    size_t iterations = 0;
    while (std::abs(z) < 2.0f && iterations < MAX_ITER) {
        z = z * z + c;
        ++iterations;
    }
    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
    if (count == MAX_ITER) {
        r = g = b = 0;
    } else {
        float t = static_cast<float>(count) / MAX_ITER;
        r = static_cast<sf::Uint8>(9 * (1 - t) * t * t * t * 255);
        g = static_cast<sf::Uint8>(15 * (1 - t) * (1 - t) * t * t * 255);
        b = static_cast<sf::Uint8>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    }
}

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i pixel) const {
    sf::Vector2f topLeft(
        m_plane_center.x - m_plane_size.x / 2.0f,
        m_plane_center.y - m_plane_size.y / 2.0f
    );
    return {
        topLeft.x + static_cast<float>(pixel.x) / m_pixel_size.x * m_plane_size.x,
        topLeft.y + static_cast<float>(pixel.y) / m_pixel_size.y * m_plane_size.y
    };
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray, states);
}
