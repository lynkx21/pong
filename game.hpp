//
// Created by Leonardo Manfrotto on 2023-08-12.
//

#ifndef PONG_GAME_HPP
#define PONG_GAME_HPP

#include <SFML/Graphics.hpp>
#include "ball.hpp"
#include "paddle.hpp"

enum class GameState {
    START,
    PLAY,
    PAUSE,
    END,
};

enum class OutcomeState {
    WIN,
    LOSE,
    GAME_ON,
};

class Game {
    sf::RenderWindow m_window;
    sf::Font m_font;
    Ball m_ball{};
    Paddle m_paddleLeft{PaddleSide::LEFT, PaddleControl::USER};
    Paddle m_paddleRight{PaddleSide::RIGHT, PaddleControl::CPU};
    GameState m_gameState{GameState::START};
    OutcomeState m_outcomeState{OutcomeState::GAME_ON};
    int m_scoreLeft{};
    int m_scoreRight{};
    sf::Text m_textLeft{Game::m_font};
    sf::Text m_textRight{Game::m_font};

public:
    void setup();
    void reset();
    void processInput();
    void readInputPressed(const sf::Event::KeyPressed* keyPressed);
    void readInputReleased(const sf::Event::KeyReleased* keyReleased);
    void run();
    void update();
    void render();
    void drawScore(const int& score, sf::Text& text, float xPos);
    sf::Text drawText(const std::string& s);
};


#endif //PONG_GAME_HPP
