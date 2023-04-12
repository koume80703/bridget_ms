#include "node.hpp"

Node::Node() {}

Node::Node(const State state, const Player target, const int expand_base) {
    this->state = state;

    this->w = 0;
    this->n = 0;
    this->expand_base = expand_base;
    this->children = vector<Node>();
    this->target = target;
}

Node::Node(const Node& rhs) {
    this->state = rhs.state;

    this->w = rhs.w;
    this->n = rhs.n;
    this->expand_base = rhs.expand_base;
    this->children = rhs.children;
    this->target = rhs.target;
}

Node& Node::operator=(const Node& rhs) {
    this->state = rhs.state;

    this->w = rhs.w;
    this->n = rhs.n;
    this->expand_base = rhs.expand_base;
    this->children = rhs.children;
    this->target = rhs.target;

    return *this;
}

double Node::evaluate() {
    if (state.is_done()) {
        if (DEBUG_NODE) {
            cout << "## in evaluate(), is_done() was true ##" << endl;
        }

        double value = state.get_result(target);

        update_weight(value);
        visited();

        return value;
    }

    if (children.empty()) {
        if (DEBUG_NODE) {
            cout << "## in evaluate(), children was emtpy ##" << endl;
        }

        double value = 0;

        if (TIME_MEASURING) {
            extern double playout_time;
            extern int playout_count;
            playout_time = get_time_msec();
            playout_count++;
        }

        if (MULTI_THREADS) {
            value += Node::playout_mt(state, target);
        } else {
            for (int i = 0; i < PLAYOUT_NUM; i++) {
                value += Node::playout(state, target);
            }
        }

        if (TIME_MEASURING) {
            extern double playout_time, total_playout_time;
            playout_time = get_time_msec() - playout_time;
            total_playout_time += playout_time;
        }

        update_weight(value);
        visited();

        if (n == expand_base) {
            if (DEBUG_NODE) {
                cout << "## in evaluate(), expand() was called ##" << endl;
            }
            expand();
        }

        return value;
    }

    else {
        if (DEBUG_NODE) {
            cout << "## in evaluate(), NOT children emtpy ##" << endl;
        }

        Node& child = next_child_based_ucb();

        double value = child.evaluate();

        update_weight(value);
        visited();

        return value;
    }
}

double Node::evaluate_mt() {
    if (state.is_done()) {
        double value = state.get_result(target);

        pthread_mutex_lock(&mutex);
        update_weight(value);
        visited();
        pthread_mutex_unlock(&mutex);

        return value;
    }

    if (children.empty()) {
        double value = 0;

        for (int i = 0; i < PLAYOUT_NUM; i++) {
            value += Node::playout(state, target);
        }

        pthread_mutex_lock(&mutex);
        update_weight(value);
        visited();
        pthread_mutex_unlock(&mutex);

        if (n == expand_base) {
            pthread_mutex_lock(&mutex);
            expand();
            pthread_mutex_unlock(&mutex);
        }

        return value;
    }

    else {
        Node& child = next_child_based_ucb();

        double value = child.evaluate();

        pthread_mutex_lock(&mutex);
        update_weight(value);
        visited();
        pthread_mutex_unlock(&mutex);

        return value;
    }
}

void Node::expand() {
    if (TIME_MEASURING) {
        extern double start;
        start = get_time_msec();
    }
    vector<Action> la = state.legal_actions();
    if (TIME_MEASURING) {
        extern double start, end;
        end = get_time_msec();
        print_time(end - start, "la time in expand()");
    }

    if (la.empty()) {
        return;
    }

    if (TIME_MEASURING) {
        extern double expand_time;
        extern int expand_count;
        expand_time = get_time_msec();
        expand_count++;
    }
    for (int i = 0; i < int(la.size()); i++) {
        children.push_back(Node(state.next(la[i])));
    }
    if (TIME_MEASURING) {
        extern double expand_time, total_expand_time;
        expand_time = get_time_msec() - expand_time;
        print_time(expand_time, "expand time");
        total_expand_time += expand_time;
    }
}

const State Node::get_state() const { return state; }

vector<Node> Node::get_children() const { return children; }

int Node::get_n() const { return n; }

double Node::get_w() const { return w; }

Node& Node::next_child_based_ucb() {
    // 探索されていないノードがあれば、そのノードを返す。
    for (int i = 0; i < int(children.size()); i++) {
        Node& child = children[i];
        if (child.get_n() == 0) {
            return child;
        }
    }

    if (TIME_MEASURING) {
        extern double ucb_time;
        extern int ucb_count;
        ucb_time = get_time_msec();
        ucb_count++;
    }

    // ucb1をもとに適切な子ノードを返す。
    int sum_n = 0;
    for (int i = 0; i < int(children.size()); i++) {
        sum_n += children[i].get_n();
    }

    vector<double> ucb1_values;
    for (int i = 0; i < int(children.size()); i++) {
        const Node child = children[i];
        ucb1_values.push_back(ucb1(sum_n, child.get_n(), child.get_w()));
    }

    if (TIME_MEASURING) {
        extern double ucb_time, total_ucb_time;
        ucb_time = get_time_msec() - ucb_time;
        print_time(ucb_time, "ucb calc time");
        total_ucb_time += ucb_time;
    }

    return children[argmax<double>(ucb1_values)];
}

void Node::visited() { this->n++; }
void Node::update_weight(const int value) { this->w += value; }

int Node::playout(const State state, const Player target) {
    if (state.is_done()) {
        return state.get_result(target);
    }

    if (TIME_MEASURING) {
        extern double la_time;
        extern int la_count;
        la_time = get_time_msec();
        la_count++;
    }

    vector<Action> la = state.legal_actions();

    if (TIME_MEASURING) {
        extern double la_time, total_la_time;
        la_time = get_time_msec() - la_time;
        total_la_time += la_time;
    }

    if (la.empty()) {
        return Node::playout(state.pass_moving(), target);
    } else {
        Action act = Game::random_action(la);
        return Node::playout(state.next(act), target);
    }
}

int Node::playout_mt(const State state, const Player target) {
    const int cores_num = thread::hardware_concurrency();

    future<int> futures[cores_num];
    for (int i = 0; i < cores_num; i++) {
        futures[i] = async(launch::async, [state, target]() {
            return playout(state, target);
        });
    }

    int sum = 0;
    for (int i = 0; i < cores_num; i++) {
        sum += futures[i].get();
    }

    return sum;
}
