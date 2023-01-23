#pragma once

namespace node {
const bool DEBUG_NODE = false;
const int EXPAND_BASE = 2;
const int PLAYOUT_NUM = 1;
}  // namespace node

namespace mcts {
const bool DEBUG_MCTS = false;
const int EVALUATE_NUM = 2000;
}  // namespace mcts

namespace measuring {
const bool TIME_MEASURING = false;
}

namespace game {
// TODO: enum.hppのインクルードによって相互参照が起きてしまう。
// 一旦、enum.hppのインクルードを中止し、game.hppにて直接定数を宣言することにする。
// const Player START_PLAYER = Player::BLACK;
// const Player TARGET_PLAYER = START_PLAYER;

const bool DEBUG_GAME = false;
}  // namespace game

namespace exe {
// main.cppにおけるパラメータ
// 名前の重複によりexeとした。
const int PLAY_NUM = 20;
}  // namespace exe

namespace board {
const int BOARD_SIZE = 8;
const int BOARD_HEIGHT = 3;
const int ABS_C_SIZE = 4;

const bool DEBUG_BOARD = false;
}  // namespace board