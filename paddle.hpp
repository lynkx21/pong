//
// Created by Leonardo Manfrotto on 2023-08-13.
//

#ifndef PONG_PADDLE_HPP
#define PONG_PADDLE_HPP

#include "constants.hpp"
#include <SFML/Graphics.hpp>

enum class PaddleSide {
    LEFT,
    RIGHT,
};

enum class PaddleControl {
    USER,
    CPU,
};

class Paddle {
    sf::RectangleShape m_shape{};
    PaddleSide m_side{};
    PaddleControl m_control{};
    sf::Vector2f m_position{};
    sf::Vector2f m_size{};
    sf::Vector2f m_halfSize{};
    sf::Vector2f m_velocity{0.0f, PADDLE_VELOCITY};
public:
    Paddle(PaddleSide side, PaddleControl control) {
        m_size = sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT);
        m_halfSize = m_size / 2.0f;
        m_shape = sf::RectangleShape(m_size);
        m_side = side;
        m_control = control;
        reset();
    }
    sf::RectangleShape getShape() const;
    sf::Vector2f getPosition() const;
    void setPosition();
    sf::Vector2f getHalfSize() const;
    void reset();
    void update(const sf::Vector2f& inputDirection, const sf::Vector2f& ballPosition);
};


#endif //PONG_PADDLE_HPP
