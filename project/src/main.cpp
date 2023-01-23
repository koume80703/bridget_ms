#include "main.hpp"

int play_bridget() {
    Game game;
    while (game.is_connected() == Color::NONE) {
        if (game.is_finished()) {
            game.show_all();
            return game.get_result(Game::TARGET_PLAYER);
        }

        vector<Action> la = game.legal_actions();

        if (la.empty()) {
            game.pass_moving();
            continue;
        }

        Action act = (game.get_cp() == Game::TARGET_PLAYER)
                         ? MCTS::mcts_action(State(game))
                         : Game::random_action(la);
        game.set_stone(act);

        cout << endl;
        game.show_action(act);
        game.show_all();
        cout << endl;
    }

    game.end_game(true);

    game.show_all();
    return game.get_result(Game::TARGET_PLAYER);
}

int main() {
    int black, white, draw;
    black = 0;
    white = 0;
    draw = 0;
    for (int i = 0; i < PLAY_NUM; i++) {
        int result = play_bridget();
        cout << i << ": ";
        if (result == 1) {
            black++;
            cout << "black" << endl;
        } else if (result == -1) {
            white++;
            cout << "white" << endl;
        } else if (result == 0) {
            draw++;
            cout << "draw" << endl;
        }
    }

    cout << "black: " << black << endl;
    cout << "white: " << white << endl;
    cout << "draw :" << draw << endl;

    return 0;
}