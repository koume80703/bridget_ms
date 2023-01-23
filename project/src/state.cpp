#include <state.hpp>

State::State() {}

State::State(Game game) { this->game = game; }

State::State(const State& rhs) { this->game = rhs.game; }

State& State::operator=(const State& rhs) {
    this->game = rhs.game;
    return *this;
}

State State::next(Action act) const {
    State n_state = State(game);
    n_state.game.set_stone(act);

    return n_state;
}

State State::pass_moving() const {
    State n_state = State(game);
    n_state.game.pass_moving();

    return n_state;
}

bool State::is_done() const { return game.is_finished(); }

vector<Action> State::legal_actions() const { return game.legal_actions(); }

int State::get_result(Player target) const { return game.get_result(target); }

Player State::get_winner() const { return game.get_winner(); }

const Game& State::get_game() const { return game; }

void State::show_all() const { game.show_all(); }
