#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp>
#include <complex>
#include <sstream>

using namespace std;
using namespace sf;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State { CALCULATING, DISPLAYING };

class ComplexPlane : public Drawable {
public:
    ComplexPlane(int pixelWidth, int pixelHeight);
    void updateRender();
    void zoomIn();
    void zoomOut();
    void setCenter(Vector2i mousePixel);
    void setMouseLocation(Vector2i mousePixel);
    void loadText(Text& text);

private:
    virtual void draw(RenderTarget& target, RenderStates states) const override;
    size_t countIterations(Vector2f coord);
    void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
    Vector2f mapPixelToCoords(Vector2i mousePixel);

    int m_pixelWidth;
    int m_pixelHeight;
    float m_aspectRatio;
    Vector2f m_plane_center;
    Vector2f m_plane_size;
    int m_zoomCount;
    State m_State;
    VertexArray m_vArray;
    Vector2f m_mouseLocation;
};

#endif // COMPLEXPLANE_H
