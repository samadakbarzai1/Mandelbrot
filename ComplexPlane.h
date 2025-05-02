// ComplexPlane.h
#pragma once
#include <SFML/Graphics.hpp>
#include <complex>
#include <vector>
#include <sstream>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State { CALCULATING, DISPLAYING };

class ComplexPlane : public sf::Drawable {
public:
    ComplexPlane(int pixelWidth, int pixelHeight);
    void updateRender();
    void zoomIn();
    void zoomOut();
    void setCenter(sf::Vector2i mousePixel);
    void setMouseLocation(sf::Vector2i mousePixel);
    void loadText(sf::Text& text);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::Vector2f mapPixelToCoords(sf::Vector2i pixel) const;
    size_t countIterations(sf::Vector2f coord);
    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);

    sf::VertexArray m_vArray;
    sf::Vector2u m_pixel_size;
    float m_aspectRatio;
    sf::Vector2f m_plane_center;
    sf::Vector2f m_plane_size;
    int m_zoomCount;
    sf::Vector2f m_mouseLocation;
    State m_state;
};

