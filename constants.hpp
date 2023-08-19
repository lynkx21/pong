//
// Created by Leonardo Manfrotto on 2023-08-12.
//

#ifndef PONG_CONSTANTS_HPP
#define PONG_CONSTANTS_HPP

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
constexpr float BALL_SIZE = static_cast<float>(WINDOW_WIDTH) / 80;
const float BALL_VELOCITY = 0.12f; // 0.05f;
constexpr float PADDLE_HEIGHT = static_cast<float>(WINDOW_HEIGHT) / 4;
constexpr float PADDLE_WIDTH = PADDLE_HEIGHT / 10;
const float PADDLE_DISTANCE = 0.03f;
const float PADDLE_VELOCITY = 0.08f;
const int TEXT_SIZE = 64;
constexpr float TEXT_WIDTH = static_cast<float>(TEXT_SIZE) / 4;

#endif //PONG_CONSTANTS_HPP
