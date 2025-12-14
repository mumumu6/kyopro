#include <bits/stdc++.h>
using namespace std;

#define main complex_main
#include "a.cpp"
#undef main

#define main simple_main
#include "a_simple.cpp"
#undef main

struct InputData {
    int N = 0;
    int L = 0;
    int T = 0;
    long long K = 0;
    vector<long long> A;
    vector<vector<long long>> C;
    bool valid = false;
};

namespace {

string trim(const string &s) {
    size_t l = 0;
    while (l < s.size() && isspace(static_cast<unsigned char>(s[l]))) l++;
    size_t r = s.size();
    while (r > l && isspace(static_cast<unsigned char>(s[r - 1]))) r--;
    return s.substr(l, r - l);
}

int cmp_bigint(const BigInt &a, const BigInt &b) {
    if (a.d.size() != b.d.size()) {
        return (a.d.size() < b.d.size()) ? -1 : 1;
    }
    for (int i = static_cast<int>(a.d.size()) - 1; i >= 0; --i) {
        if (a.d[i] != b.d[i]) {
            return (a.d[i] < b.d[i]) ? -1 : 1;
        }
    }
    return 0;
}

InputData parse_input(const string &text) {
    InputData res;
    istringstream iss(text);
    if (!(iss >> res.N >> res.L >> res.T >> res.K)) {
        return res;
    }
    res.A.assign(res.N, 0);
    for (int i = 0; i < res.N; ++i) {
        if (!(iss >> res.A[i])) return res;
    }
    res.C.assign(res.L, vector<long long>(res.N, 0));
    for (int lv = 0; lv < res.L; ++lv) {
        for (int j = 0; j < res.N; ++j) {
            if (!(iss >> res.C[lv][j])) return res;
        }
    }
    res.valid = true;
    return res;
}

struct SimulationResult {
    bool ok = false;
    BigInt apples;
    string error;
};

SimulationResult simulate(const InputData &input,
                          const vector<pair<int, int>> &actions) {
    SimulationResult res;
    if (!input.valid) {
        res.error = "input parse failed";
        return res;
    }
    if (static_cast<int>(actions.size()) != input.T) {
        res.error = "action length mismatch";
        return res;
    }
    vector<vector<BigInt>> B(input.L, vector<BigInt>(input.N, BigInt(1)));
    vector<vector<int>> P(input.L, vector<int>(input.N, 0));
    BigInt apples(input.K);

    for (int turn = 0; turn < input.T; ++turn) {
        int i = actions[turn].first;
        int j = actions[turn].second;
        if (i >= 0) {
            if (!(0 <= i && i < input.L && 0 <= j && j < input.N)) {
                res.error = "invalid machine index";
                return res;
            }
            unsigned long long cost =
                static_cast<unsigned long long>(input.C[i][j]) *
                static_cast<unsigned long long>(P[i][j] + 1);
            if (!apples.ge_u64(cost)) {
                res.error = "not enough apples";
                return res;
            }
            apples.sub_u64(cost);
            P[i][j] += 1;
        } else {
            if (j != -1) {
                res.error = "malformed action";
                return res;
            }
        }

        for (int id = 0; id < input.N; ++id) {
            int p = P[0][id];
            if (p == 0) continue;
            unsigned long long mul =
                static_cast<unsigned long long>(input.A[id]) *
                static_cast<unsigned long long>(p);
            apples.add_mul_small(B[0][id], mul);
        }
        for (int lv = 1; lv < input.L; ++lv) {
            for (int id = 0; id < input.N; ++id) {
                int p = P[lv][id];
                if (p == 0) continue;
                B[lv - 1][id].add_mul_small(B[lv][id],
                                            static_cast<unsigned long long>(p));
            }
        }
    }

    res.ok = true;
    res.apples = apples;
    return res;
}

struct ParseResult {
    bool ok = false;
    vector<pair<int, int>> actions;
    string sanitized;
    string error;
};

ParseResult parse_actions(const string &raw_output, int T) {
    ParseResult res;
    vector<string> sanitized_lines;
    istringstream iss(raw_output);
    string line;
    bool too_many = false;
    while (getline(iss, line)) {
        string trimmed = trim(line);
        size_t hash_pos = trimmed.find('#');
        if (hash_pos != string::npos) {
            trimmed = trim(trimmed.substr(0, hash_pos));
        }
        if (trimmed.empty()) continue;
        if (res.actions.size() >= static_cast<size_t>(T)) {
            too_many = true;
            break;
        }
        stringstream ls(trimmed);
        int a = 0, b = 0;
        if (!(ls >> a)) {
            res.error = "failed to parse action";
            return res;
        }
        if (a == -1) {
            res.actions.emplace_back(-1, -1);
            sanitized_lines.emplace_back("-1");
            continue;
        }
        if (!(ls >> b)) {
            res.error = "failed to parse action";
            return res;
        }
        res.actions.emplace_back(a, b);
        sanitized_lines.emplace_back(to_string(a) + " " + to_string(b));
    }
    if (too_many) {
        res.error = "too many actions";
        return res;
    }
    if (static_cast<int>(res.actions.size()) != T) {
        res.error = "insufficient actions";
        return res;
    }
    res.ok = true;
    string out;
    out.reserve(T * 6);
    for (size_t idx = 0; idx < sanitized_lines.size(); ++idx) {
        out.append(sanitized_lines[idx]);
        out.push_back('\n');
    }
    res.sanitized = move(out);
    return res;
}

struct StrategyResult {
    bool ok = false;
    BigInt apples;
    string sanitized_output;
    string raw_output;
    string error;
};

StrategyResult run_strategy(int (*strategy_main)(), const string &input_text,
                            const InputData &input) {
    StrategyResult res;
    istringstream fake_in(input_text);
    ostringstream fake_out;
    auto *orig_in = cin.rdbuf(fake_in.rdbuf());
    auto *orig_out = cout.rdbuf(fake_out.rdbuf());
    strategy_main();
    cout.flush();
    res.raw_output = fake_out.str();
    cin.rdbuf(orig_in);
    cout.rdbuf(orig_out);

    ParseResult parsed = parse_actions(res.raw_output, input.T);
    if (!parsed.ok) {
        res.error = parsed.error;
        return res;
    }
    SimulationResult sim = simulate(input, parsed.actions);
    if (!sim.ok) {
        res.error = sim.error;
        return res;
    }
    res.ok = true;
    res.apples = sim.apples;
    res.sanitized_output = parsed.sanitized;
    return res;
}

} // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input_data((istreambuf_iterator<char>(cin)),
                      istreambuf_iterator<char>());
    if (input_data.empty()) {
        return 0;
    }
    InputData input = parse_input(input_data);
    if (!input.valid) {
        // fall back to complex_main output
        istringstream fake_in(input_data);
        ostringstream fake_out;
        auto *orig_in = cin.rdbuf(fake_in.rdbuf());
        auto *orig_out = cout.rdbuf(fake_out.rdbuf());
        complex_main();
        cout.flush();
        string raw = fake_out.str();
        cin.rdbuf(orig_in);
        cout.rdbuf(orig_out);
        cout << raw;
        if (!raw.empty() && raw.back() != '\n') cout << '\n';
        return 0;
    }

    StrategyResult complex_res = run_strategy(complex_main, input_data, input);
    StrategyResult simple_res = run_strategy(simple_main, input_data, input);

    int best_idx = -1;
    BigInt best_value;
    if (complex_res.ok) {
        best_idx = 0;
        best_value = complex_res.apples;
    }
    if (simple_res.ok) {
        if (best_idx == -1 || cmp_bigint(simple_res.apples, best_value) > 0) {
            best_idx = 1;
            best_value = simple_res.apples;
        }
    }

    if (best_idx == 0) {
        cout << complex_res.sanitized_output;
        return 0;
    }
    if (best_idx == 1) {
        cout << simple_res.sanitized_output;
        return 0;
    }

    string fallback = complex_res.raw_output.empty() ? simple_res.raw_output
                                                     : complex_res.raw_output;
    if (!fallback.empty()) {
        cout << fallback;
        if (fallback.back() != '\n') cout << '\n';
    } else {
        for (int t = 0; t < input.T; ++t) cout << "-1\n";
    }
    return 0;
}
