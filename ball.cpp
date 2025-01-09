//
// Created by Leonardo Manfrotto on 2023-08-12.
//

#include "ball.hpp"
#include "constants.hpp"
#include <cstdlib>
#include <math.h>

sf::Vector2f getVelocity(float xDir, float yDir) {
    float angleModifier = std::acosf(std::fabs(xDir)) + M_PI / 4 / (M_PI / 2);
    return sf::Vector2f{BALL_VELOCITY * (1 + std::fabs(angleModifier - 0.5f)) * xDir, BALL_VELOCITY * (1 + std::fabs(angleModifier - 0.5f)) * yDir};
}

float getPositionRelativeToPaddle(float yBallPosition, float yPaddlePosition, float yPaddleHalfSize) {
    return (yBallPosition - (yPaddlePosition - yPaddleHalfSize)) / (2.0f * yPaddleHalfSize);
}

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
    float xDir = static_cast<float>(std::rand() / RAND_MAX) - 0.5f;
    float yDir = static_cast<float>(std::rand() / RAND_MAX) - 0.5f;
    sf::Vector2f vel{xDir, yDir};
    vel = vel.normalized();
    m_velocity = getVelocity(vel.x, vel.y);
}

void Ball::update() {
    m_position += m_velocity;
}

void Ball::solveCollisions(const sf::Vector2f& overlap, const float angleModifier) {
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
        float angle = -M_PI / 4 + M_PI / 2 * angleModifier;
        m_velocity = getVelocity(cos(angle), sin(angle));
    } else if (overlap.x > 0) {
        // Right collision
        // FIXME
        float paddleEdge = m_position.x;
        m_position.x = m_position.x - 2.0f * overlap.x;
        float angle = -M_PI / 4 + M_PI / 2 * angleModifier;
        m_velocity = getVelocity(cos(angle), sin(angle));
        m_velocity.x *= -1;
    }

    // New position
    updatePosition();
}

sf::Vector3f Ball::checkCollisionAABB(const Paddle& paddle) const {
    sf::Vector2f paddlePosition{paddle.getPosition()};
    sf::Vector2f paddleHalfSize{paddle.getHalfSize()};
    sf::Vector2f delta = paddlePosition - m_position;
    delta.x = abs(delta.x);
    delta.y = abs(delta.y);
    float overlapX = paddleHalfSize.x + m_halfSize - delta.x;
    float overlapY = paddleHalfSize.y + m_halfSize - delta.y;
    float yPositionToPaddle = getPositionRelativeToPaddle(m_position.y, paddlePosition.y, paddleHalfSize.y);
    return sf::Vector3f{overlapX, overlapY, yPositionToPaddle};
}
