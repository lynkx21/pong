//
// Created by Leonardo Manfrotto on 2023-08-13.
//

#include "paddle.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>

sf::RectangleShape Paddle::getShape() const {
    return m_shape;
}

sf::Vector2f Paddle::getPosition() const {
    return m_position;
}

void Paddle::updatePosition() {
    m_shape.setPosition(m_position - m_halfSize);
}

sf::Vector2f Paddle::getHalfSize() const {
    return m_halfSize;
}

void Paddle::reset() {
    if (m_side == PaddleSide::LEFT) {
        m_position = sf::Vector2f(
                WINDOW_WIDTH * PADDLE_DISTANCE + m_halfSize.x,
                WINDOW_HEIGHT / 2.0f);
    } else {
        m_position = sf::Vector2f(
                WINDOW_WIDTH * (1 - PADDLE_DISTANCE) - m_halfSize.x,
                WINDOW_HEIGHT / 2.0f);
    }
    updatePosition();
}


void Paddle::handleInputPressed(const sf::Event::KeyPressed* keyPressed) {
    m_inputDirection.y = 0.0f;

    if (m_control == PaddleControl::CPU) {
        return;
    }

    sf::Keyboard::Scan paddle_up = m_side == PaddleSide::LEFT ? sf::Keyboard::Scancode::W : sf::Keyboard::Scancode::Up;
    sf::Keyboard::Scan paddle_down = m_side == PaddleSide::LEFT ? sf::Keyboard::Scancode::S : sf::Keyboard::Scancode::Down;

    // const auto* keyPressed{event->getIf<sf::Event::KeyPressed>()};
    // const auto* keyReleased{event->getIf<sf::Event::KeyReleased>()};
    if (keyPressed->scancode == paddle_up) {
        m_inputDirection.y = -1.0f;
    }
    if (keyPressed->scancode == paddle_down) {
        m_inputDirection.y = 1.0f;
    }
}

void Paddle::handleInputReleased(const sf::Event::KeyReleased* keyReleased) {
    m_inputDirection.y = 0.0f;

    if (m_control == PaddleControl::CPU) {
        return;
    }

    sf::Keyboard::Scan paddle_up = m_side == PaddleSide::LEFT ? sf::Keyboard::Scancode::W : sf::Keyboard::Scancode::Up;
    sf::Keyboard::Scan paddle_down = m_side == PaddleSide::LEFT ? sf::Keyboard::Scancode::S : sf::Keyboard::Scancode::Down;

    if (keyReleased->scancode == paddle_up || keyReleased->scancode == paddle_down) {
        m_inputDirection.y = 0.0f;
    }
}


void Paddle::update(const sf::Vector2f& ballPosition) {
    float new_pos_y{m_position.y};
    if (m_control == PaddleControl::USER) {
            new_pos_y += m_velocity.y * m_inputDirection.y;
    } else {
        // CPU
        if (ballPosition.y < m_position.y) {
            new_pos_y -= m_velocity.y;
        } else if (ballPosition.y > m_position.y) {
            new_pos_y += m_velocity.y;
        }
    }

    // Check top-bottom boundaries
    if (new_pos_y - m_halfSize.y < 0) {
        new_pos_y = m_halfSize.y;
    } else if (new_pos_y + m_halfSize.y > WINDOW_HEIGHT) {
        new_pos_y = WINDOW_HEIGHT - m_halfSize.y;
    }

    m_position.y = new_pos_y;
    updatePosition();
}