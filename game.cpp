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
    m_inputDirection = sf::Vector2f(0.0f, 0.0f);
}

void Game::readInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_window.close();
        return;
    }

    switch (m_gameState) {
        case GameState::START:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    m_gameState = GameState::PLAY;
                }
            }
            break;
        case GameState::PLAY:
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Enter:
                        m_gameState = GameState::PAUSE;
                        break;
                    case sf::Keyboard::R:
                        m_gameState = GameState::START;
                        break;
                    default:
                        break;
                    case sf::Keyboard::W:
                        m_inputDirection.y = -1.0f;
                        break;
                    case sf::Keyboard::S:
                        m_inputDirection.y = 1.0f;
                        break;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        m_inputDirection.y = 0.0f;
                        break;
                    case sf::Keyboard::S:
                        m_inputDirection.y = 0.0f;
                        break;
                    default:
                        break;
                }
            }
            break;
        case GameState::PAUSE:
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Enter:
                        m_gameState = GameState::PLAY;
                        break;
                    case sf::Keyboard::R:
                        m_gameState = GameState::START;
                        break;
                    default:
                        break;
                }
            }
            break;
        case GameState::END:
            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Enter) {
                m_gameState = GameState::START;
                m_outcomeState = OutcomeState::GAME_ON;
            }
            break;
    }
}

void Game::run() {
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong", sf::Style::Default);
    if (!m_font.loadFromFile("../assets/VCR_OSD_MONO_1.001.ttf")) {
        // Throw error
    }

    while (m_window.isOpen()) {
        // Check events
        sf::Event event{};
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }

            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                readInput(event);
            }

        }

        switch (m_gameState) {
            case GameState::START:
                setup();
                break;
            case GameState::PAUSE:
                break;
            case GameState::PLAY: {
                // Update logic
                m_paddleLeft.update(m_inputDirection, m_ball.getPosition());
                m_paddleRight.update(m_inputDirection, m_ball.getPosition());
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
    text.setPosition(xPosition, WINDOW_HEIGHT * 0.03f);
    text.setFillColor(sf::Color::White);
}

sf::Text Game::drawText(const std::string& s) {
    sf::Text text;
    text.setFont(m_font);
    text.setString(s);
    text.setCharacterSize(TEXT_SIZE * 1.5f);
    text.setFillColor(sf::Color::White);

    float width = text.getGlobalBounds().width;
    float height = text.getGlobalBounds().height;
    text.setPosition(WINDOW_WIDTH / 2.0f - width / 2.0f, WINDOW_HEIGHT / 2.0f - height / 2.0f);

    return text;
}
