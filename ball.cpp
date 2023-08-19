//
// Created by Leonardo Manfrotto on 2023-08-12.
//

#include "ball.hpp"
#include "constants.hpp"
#include <cstdlib>

sf::CircleShape Ball::getShape() const {
    return m_shape;
}

sf::Vector2f Ball::getPosition() const {
    return m_position;
}

void Ball::updatePosition() {
    m_shape.setPosition(m_position - sf::Vector2f(m_halfSize, m_halfSize));
}

void Ball::reset() {
    m_position = sf::Vector2f(
            static_cast<float>(WINDOW_WIDTH) / 2,
            static_cast<float>(WINDOW_HEIGHT) / 2);
    updatePosition();

    // Generate random direction
    // TODO understand how C++ handles RNG
    float xDir = 2.0f * static_cast<float>(std::rand() % 2) - 1.0f;
    float yDir = 2.0f * static_cast<float>(std::rand() % 2) - 1.0f;
    m_velocity = sf::Vector2f(BALL_VELOCITY * xDir, BALL_VELOCITY * yDir);
}

void Ball::update() {
    m_position += m_velocity;
}

void Ball::solveCollisions(const sf::Vector2f& overlap) {
    // Vertical collision
    if (m_position.y - m_halfSize < 0) {
        // Position the ball by the quantity of overlap over the top boundary
        // New pos = (P - HS) * (-1) + HS
        m_position.y = m_halfSize - m_position.y + m_halfSize;
        m_velocity.y *= -1;
    } else if (m_position.y + m_halfSize > WINDOW_HEIGHT) {
        // Position the ball by the quantity of overlap over the bottom boundary
        // New pos = WH - (WH - (P + HS)) - HS
        m_position.y = WINDOW_HEIGHT - (WINDOW_HEIGHT - (m_position.y - m_halfSize)) - m_halfSize;
        m_velocity.y *= -1;
    }

    // Horizontal collision
    if (overlap.x < 0) {
        // Left collision
        float paddleEdge = m_position.x - m_halfSize - overlap.x;
        m_position.x = paddleEdge + m_halfSize - overlap.x;
        m_velocity.x *= -1;
    } else if (overlap.x > 0) {
        // Right collision
        // FIXME
        float paddleEdge = m_position.x;
        m_position.x = m_position.x - 2.0f * overlap.x;
        m_velocity.x *= -1;
    }

    // New position
    updatePosition();
}

sf::Vector2f Ball::checkCollisionAABB(const Paddle& paddle) const {
    sf::Vector2f paddlePosition{paddle.getPosition()};
    sf::Vector2f paddleHalfSize{paddle.getHalfSize()};
    sf::Vector2f delta = paddlePosition - m_position;
    delta.x = abs(delta.x);
    delta.y = abs(delta.y);
    float overlapX = paddleHalfSize.x + m_halfSize - delta.x;
    float overlapY = paddleHalfSize.y + m_halfSize - delta.y;
    return sf::Vector2f{overlapX, overlapY};
}
