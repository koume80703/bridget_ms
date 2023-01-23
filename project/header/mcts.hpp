#pragma once

#include <iostream>
#include <vector>

#include "action.hpp"
#include "argmax.hpp"
#include "measuring.hpp"
#include "node.hpp"
#include "parameter.hpp"
#include "state.hpp"

using namespace std;

class MCTS {
   public:
    static const bool DEBUG_MCTS = mcts::DEBUG_MCTS;
    static const int EVALUATE_NUM = mcts::EVALUATE_NUM;

    static void train(Node& root_node, const int evaluate_num = EVALUATE_NUM);
    static Action select_action(const Node& root_node);
    static Action mcts_action(State state);

   private:
};