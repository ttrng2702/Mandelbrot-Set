#include "ComplexPlane.h"
#include <cmath>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixelWidth(pixelWidth), m_pixelHeight(pixelHeight),
      m_vArray(Points, pixelWidth * pixelHeight) {
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_plane_center = {0.0f, 0.0f};
    m_plane_size = {BASE_WIDTH, BASE_HEIGHT * m_aspectRatio};
    m_zoomCount = 0;
    m_State = State::CALCULATING;
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
    target.draw(m_vArray, states);
}

void ComplexPlane::updateRender() {
    if (m_State == State::CALCULATING) {
        for (int i = 0; i < m_pixelHeight; ++i) {
            for (int j = 0; j < m_pixelWidth; ++j) {
                size_t index = j + i * m_pixelWidth;
                m_vArray[index].position = {static_cast<float>(j), static_cast<float>(i)};
                Vector2f coord = mapPixelToCoords({j, i});
                size_t iterations = countIterations(coord);

                Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);
                m_vArray[index].color = Color(r, g, b);
            }
        }
        m_State = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    m_plane_size.x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    m_plane_size.x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
    m_plane_center = mapPixelToCoords(mousePixel);
    m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text) {
    stringstream ss;
    ss << "Mandelbrot Set\n"
       << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n"
       << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n"
       << "Left-click to Zoom in\n"
       << "Right-click to Zoom out";
    text.setString(ss.str());
    text.setPosition(10.f, 10.f);
}

size_t ComplexPlane::countIterations(Vector2f coord) {
    complex<double> c(coord.x, coord.y), z = c;
    size_t iter = 0;
    while (abs(z) <= 2.0 && iter < MAX_ITER) {
        z = z * z + c;
        iter++;
    }
    return iter;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
    if (count == MAX_ITER) {
        r = g = b = 0; // Black for Mandelbrot set
    } else {
        r = count * 5 % 255;
        g = count * 10 % 255;
        b = count * 15 % 255;
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {
    float real = ((mousePixel.x / static_cast<float>(m_pixelWidth)) * m_plane_size.x) +
                 (m_plane_center.x - m_plane_size.x / 2.0f);
    float imag = ((1.0f - mousePixel.y / static_cast<float>(m_pixelHeight)) * m_plane_size.y) +
                 (m_plane_center.y - m_plane_size.y / 2.0f);
    return {real, imag};
}
