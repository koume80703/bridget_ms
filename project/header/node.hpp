#pragma once

#include <future>
#include <iostream>
#include <thread>

#include "argmax.hpp"
#include "game.hpp"
#include "measuring.hpp"
#include "state.hpp"
#include "ucb1.hpp"

using namespace std;

class Node {
   public:
    static const bool MULTI_THREADS = node::MULTI_THREADS;
    static const bool DEBUG_NODE = node::DEBUG_NODE;
    static const int EXPAND_BASE = node::EXPAND_BASE;
    static const int PLAYOUT_NUM = node::PLAYOUT_NUM;

    Node();
    Node(const State state, const Player target = Game::TARGET_PLAYER,
         const int expand_base = EXPAND_BASE);

    Node(const Node& rhs);

    Node& operator=(const Node& rhs);

    double evaluate();
    void expand();

    const State get_state() const;
    vector<Node> get_children() const;
    int get_n() const;
    double get_w() const;

    static int playout(const State state, const Player target);

    static int playout_mt(const State state, const Player target);

   private:
    State state;
    double w;
    int n, expand_base;

    vector<Node> children;
    Player target;

    Node& next_child_based_ucb();

    void visited();
    void update_weight(const int value);
};