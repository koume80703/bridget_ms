#pragma once

#include <iostream>
#include <vector>

#include "action.hpp"
#include "enum.hpp"
#include "game.hpp"

class State {
   public:
    State();
    State(const Game game);

    State(const State& rhs);
    State& operator=(const State& rhs);

    State next(Action act) const;
    State pass_moving() const;

    bool is_done() const;
    vector<Action> legal_actions() const;
    int get_result(Player target) const;

    Player get_winner() const;

    void show_all() const;

    const Game& get_game() const;

   private:
    Game game;
};