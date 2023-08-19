//
// Created by Leonardo Manfrotto on 2023-08-12.
//

#ifndef PONG_BALL_HPP
#define PONG_BALL_HPP

#include "constants.hpp"
#include "paddle.hpp"
#include <SFML/Graphics.hpp>

class Ball {
    sf::CircleShape m_shape{};
    float m_size{};
    float m_halfSize{};
    sf::Vector2f m_position{};
    sf::Vector2f m_velocity{};
public:
    Ball() {
        m_size = BALL_SIZE;
        m_halfSize = m_size / 2.0f;
        m_shape = sf::CircleShape(m_size);
        reset();
    };
    sf::CircleShape getShape() const;
    sf::Vector2f getPosition() const;
    void setPosition();
    void reset();
    void update();
    void solveCollisions(const sf::Vector2f& overlap);
    sf::Vector2f checkCollisionAABB(const Paddle& paddle) const;
};


#endif //PONG_BALL_HPP
