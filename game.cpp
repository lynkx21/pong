//
// Created by Leonardo Manfrotto on 2023-08-12.
//

#include "game.hpp"
#include "constants.hpp"
#include "ball.hpp"
#include <string>

void Game::setup() {
    m_scoreLeft = 0;
    m_scoreRight = 0;
    reset();
}

void Game::reset() {
    m_paddleLeft.reset();
    m_paddleRight.reset();
    m_ball.reset();
}

void Game::processInput() {
    // sf::Event event{};
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        if (event->is<sf::Event::KeyPressed>()) {
            const auto* keyPressed{event->getIf<sf::Event::KeyPressed>()};
            readInputPressed(keyPressed);
        }

        if (event->is<sf::Event::KeyReleased>()) {
            const auto* keyReleased{event->getIf<sf::Event::KeyReleased>()};
            readInputReleased(keyReleased);
        }

    }
}

void Game::readInputPressed(const sf::Event::KeyPressed* keyPressed) {
    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
        m_window.close();
        return;
    }

    switch (m_gameState) {
        case GameState::START:
            if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
                m_gameState = GameState::PLAY;
            }
            break;
        case GameState::PLAY:
            switch (keyPressed->scancode) {
                case sf::Keyboard::Scancode::Enter:
                    m_gameState = GameState::PAUSE;
                    break;
                case sf::Keyboard::Scancode::R:
                    m_gameState = GameState::START;
                    break;
                default:
                    m_paddleLeft.handleInputPressed(keyPressed);
                    m_paddleRight.handleInputPressed(keyPressed);
                    break;
            }
        case GameState::PAUSE:
            switch (keyPressed->scancode) {
                case sf::Keyboard::Scancode::Enter:
                    m_gameState = GameState::PLAY;
                    break;
                case sf::Keyboard::Scancode::R:
                    m_gameState = GameState::START;
                    break;
                default:
                    break;
            }
            break;
        case GameState::END:
            if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
                m_gameState = GameState::START;
                m_outcomeState = OutcomeState::GAME_ON;
            }
            break;
    }
}

void Game::readInputReleased(const sf::Event::KeyReleased* keyReleased) {
    if (m_gameState == GameState::PLAY) {
        // when releasing
        m_paddleLeft.handleInputReleased(keyReleased);
        m_paddleRight.handleInputReleased(keyReleased);
    }
}

void Game::update() {
    // Update logic
    m_paddleLeft.update(m_ball.getPosition());
    m_paddleRight.update(m_ball.getPosition());
    m_ball.update();

    sf::Vector2 overlapLeft{m_ball.checkCollisionAABB(m_paddleLeft)};
    sf::Vector2 overlapRight{m_ball.checkCollisionAABB(m_paddleRight)};
    if (overlapLeft.x > 0 && overlapLeft.y > 0) {
        m_ball.solveCollisions(-overlapLeft);
    } else if (overlapRight.x > 0 && overlapRight.y > 0) {
        m_ball.solveCollisions(overlapRight);
    } else {
        m_ball.solveCollisions(sf::Vector2{0.0f, 0.0f});
    }

    // Check for scores
    if (m_ball.getPosition().x < 0) {
        m_scoreRight += 1;
        reset();
        m_gameState = GameState::PAUSE;
    }
    if (m_ball.getPosition().x > WINDOW_WIDTH) {
        m_scoreLeft += 1;
        reset();
        m_gameState = GameState::PAUSE;
    }

    // Check if
    if (m_scoreLeft == 5) {
        m_outcomeState = OutcomeState::WIN;
        m_gameState = GameState::END;
    }
    if (m_scoreRight == 5) {
        m_outcomeState = OutcomeState::LOSE;
        m_gameState = GameState::END;
    }
}

void Game::run() {
    m_window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pong", sf::Style::Default);
    if (!m_font.openFromFile("../assets/VCR_OSD_MONO_1.001.ttf")) {
        // Throw error
    }

    while (m_window.isOpen()) {
        // Process events
        processInput();

        // Update
        switch (m_gameState) {
            case GameState::START:
                setup();
                break;
            case GameState::PAUSE:
                break;
            case GameState::PLAY:
                update();
            case GameState::END:
                break;
        }

        // Render
        render();
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);
    m_window.draw(m_paddleLeft.getShape());
    m_window.draw(m_paddleRight.getShape());
    m_window.draw(m_ball.getShape());
    drawScore(m_scoreLeft, m_textLeft, WINDOW_WIDTH * 0.4f);
    drawScore(m_scoreRight, m_textRight, WINDOW_WIDTH * 0.6f - TEXT_WIDTH);
    m_window.draw(m_textLeft);
    m_window.draw(m_textRight);

    if (m_outcomeState == OutcomeState::WIN) {
        m_window.draw(drawText("YOU WIN! :)"));
    }
    if (m_outcomeState == OutcomeState::LOSE) {
        m_window.draw(drawText("YOU LOSE! :("));
    }

    m_window.display();
}

void Game::drawScore(const int &score, sf::Text &text, float xPosition) {
    text.setFont(m_font);
    text.setString(std::to_string(score));
    text.setCharacterSize(TEXT_SIZE);
    text.setPosition({xPosition, WINDOW_HEIGHT * 0.03f});
    text.setFillColor(sf::Color::White);
}

sf::Text Game::drawText(const std::string& s) {
    sf::Text text(m_font);
    text.setString(s);
    text.setCharacterSize(TEXT_SIZE * 1.5f);
    text.setFillColor(sf::Color::White);

    float width = text.getGlobalBounds().size.x;
    float height = text.getGlobalBounds().size.y;
    text.setPosition({WINDOW_WIDTH / 2.0f - width / 2.0f, WINDOW_HEIGHT / 2.0f - height / 2.0f});

    return text;
}
