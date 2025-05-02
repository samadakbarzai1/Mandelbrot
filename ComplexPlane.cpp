// ComplexPlane.cpp

// created by samad and maryam
#include "ComplexPlane.h"
#include <cmath>
#include <thread>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixel_size(pixelWidth, pixelHeight), m_vArray(sf::Points, pixelWidth * pixelHeight), m_zoomCount(0), m_state(State::CALCULATING) {
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_plane_center = {0.f, 0.f};
    m_plane_size = {BASE_WIDTH, BASE_HEIGHT * m_aspectRatio};
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray, states);
}
// i used multi threading to generating the Mandelbrot set pixels, i hope you like it professor
void ComplexPlane::updateRender() {
    if (m_state != State::CALCULATING) return;

    const int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    int rows_per_thread = m_pixel_size.y / num_threads;

    for (int t = 0; t < num_threads; ++t) {
        int start = t * rows_per_thread;
        int end = (t == num_threads - 1) ? m_pixel_size.y : start + rows_per_thread;

        threads.emplace_back([=]() {
            for (int i = start; i < end; ++i) {
                for (int j = 0; j < m_pixel_size.x; ++j) {
                    int idx = j + i * m_pixel_size.x;
                    sf::Vector2f coord = mapPixelToCoords({j, i});
                    size_t iter = countIterations(coord);
                    sf::Uint8 r, g, b;
                    iterationsToRGB(iter, r, g, b);
                    m_vArray[idx].position = {(float)j, (float)i};
                    m_vArray[idx].color = sf::Color(r, g, b);
                }
            }
        });
    }

    for (auto& t : threads) t.join();
    m_state = State::DISPLAYING;
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float scale = pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {BASE_WIDTH * scale, BASE_HEIGHT * m_aspectRatio * scale};
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float scale = pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {BASE_WIDTH * scale, BASE_HEIGHT * m_aspectRatio * scale};
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i pixel) {
    m_plane_center = mapPixelToCoords(pixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i pixel) {
    m_mouseLocation = mapPixelToCoords(pixel);
}

void ComplexPlane::loadText(sf::Text& text) {
    std::stringstream ss;
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")";
    text.setString(ss.str());
}

size_t ComplexPlane::countIterations(sf::Vector2f coord) {
    std::complex<double> c(coord.x, coord.y);
    std::complex<double> z = c;
    size_t i = 0;
    while (abs(z) < 2.0 && i < MAX_ITER) {
        z = z * z + c;
        i++;
    }
    return i;
}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
    if (count == MAX_ITER) {
        r = g = b = 0;
    } else {
        r = static_cast<sf::Uint8>((count * 5) % 255);
        g = static_cast<sf::Uint8>((count * 10) % 255);
        b = static_cast<sf::Uint8>((count * 15) % 255);
    }
}

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i pixel) const {
    float real = ((float)pixel.x / m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2);
    float imag = ((float)(m_pixel_size.y - pixel.y) / m_pixel_size.y) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2);
    return {real, imag};
}

