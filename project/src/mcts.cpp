#include <mcts.hpp>

void MCTS::train(Node& root_node, const int evaluate_num) {
    if (TIME_MEASURING) {
        extern double total_evaluate_time, total_playout_time, total_la_time,
            total_expand_time, total_ucb_time;
        extern int playout_count, la_count, expand_count, ucb_count;
        total_evaluate_time = 0.0;
        total_playout_time = 0.0;
        total_la_time = 0.0;
        total_expand_time = 0.0;
        total_ucb_time = 0.0;
        playout_count = 0;
        la_count = 0;
        expand_count = 0;
        ucb_count = 0;
    }

    root_node.expand();

    for (int i = 0; i < evaluate_num; i++) {
        if (TIME_MEASURING) {
            extern double evaluate_time;
            evaluate_time = get_time_msec();
        }

        if (DEBUG_MCTS) {
            cout << "evaluate count: " << i << endl;
        }

        root_node.evaluate();

        if (TIME_MEASURING) {
            extern double evaluate_time, total_evaluate_time;
            evaluate_time = get_time_msec() - evaluate_time;
            total_evaluate_time += evaluate_time;
            print_time(evaluate_time, "evaluate time");
        }
    }

    if (TIME_MEASURING) {
        extern double total_evaluate_time, total_playout_time, total_la_time,
            total_expand_time, total_ucb_time;
        extern int playout_count, la_count, expand_count, ucb_count;

        cout << "**********************************************" << endl;
        cout << total_evaluate_time << ", " << evaluate_num << endl;
        print_time(total_evaluate_time / evaluate_num, "average evaluate time");
        cout << total_playout_time << ", " << playout_count << endl;
        print_time(total_playout_time / playout_count, "average playout time");
        cout << total_la_time << ", " << la_count << endl;
        print_time(total_la_time / la_count, "average la time");
        cout << total_expand_time << ", " << expand_count << endl;
        print_time(total_expand_time / expand_count, "average expand time");
        cout << total_ucb_time << ", " << ucb_count << endl;
        print_time(total_ucb_time / ucb_count, "average ucb calc time");
        print_percentage(total_playout_time / total_evaluate_time, "playout");
        print_percentage(total_la_time / total_evaluate_time, "legal_action()");
        cout << "**********************************************" << endl;
    }
}

Action MCTS::select_action(const Node& root_node) {
    // 訪問回数が多い子ノードの行動を選択する。

    vector<Action> la = root_node.get_state().legal_actions();
    vector<int> visit_list;
    vector<Node> children = root_node.get_children();
    for (int i = 0; i < int(children.size()); i++) {
        visit_list.push_back(children[i].get_n());
    }
    return la[argmax<int>(visit_list)];
}

Action MCTS::mcts_action(State state) {
    Node root_node = Node(state);
    MCTS::train(root_node, EVALUATE_NUM);
    return MCTS::select_action(root_node);
}