//
// Created by Leonardo Manfrotto on 2023-08-12.
//

#ifndef PONG_BALL_HPP
#define PONG_BALL_HPP

#include "constants.hpp"
#include "paddle.hpp"
#include <SFML/Graphics.hpp>

class Ball {
    float m_size{BALL_SIZE};
    float m_halfSize{m_size / 2.0f};
    sf::CircleShape m_shape{m_size};
    sf::Vector2f m_position{};
    sf::Vector2f m_velocity{};
public:
    Ball() {
        reset();
    };
    sf::CircleShape getShape() const;
    sf::Vector2f getPosition() const;
    void updatePosition();
    void reset();
    void update();
    void solveCollisions(const sf::Vector2f& overlap);
    sf::Vector2f checkCollisionAABB(const Paddle& paddle) const;
};


#endif //PONG_BALL_HPP
