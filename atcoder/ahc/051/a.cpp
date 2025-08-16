#include <bits/stdc++.h>

using namespace std;

// --- Template ---
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define spa " "
#define el '\n'

// --- Timer & RNG ---
auto start_time = chrono::high_resolution_clock::now();
double get_time() {
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::duration<double>>(now - start_time).count();
}
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
double rand_double() { return uniform_real_distribution<double>(0.0, 1.0)(rng); }
ll rand_ll(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }

// --- Data Structures ---
struct Point {
    int x, y;
};
struct Edge {
    int u, v;
};

struct State {
    ll n, m, k;
    vector<int> d;
    vector<Edge> edges;
    vector<int> sorter_k;
    double score = 1e18;

    State(ll n, ll m, ll k) : n(n), m(m), k(k), d(n), sorter_k(m, -1) {}
};

// --- Geometry ---
long long cross_product(Point a, Point b, Point c) {
    return (long long)(b.x - a.x) * (c.y - a.y) - (long long)(b.y - a.y) * (c.x - a.x);
}
int sign(long long val) { return (val > 0) - (val < 0); }
bool segments_intersect(Point p1, Point p2, Point q1, Point q2) {
    // Endpoints shared check (allowed by problem)
    if ((p1.x == q1.x && p1.y == q1.y) || (p1.x == q2.x && p1.y == q2.y) ||
        (p2.x == q1.x && p2.y == q1.y) || (p2.x == q2.x && p2.y == q2.y))
        return false;

    if (max(p1.x, p2.x) < min(q1.x, q2.x) || max(q1.x, q2.x) < min(p1.x, p2.x) ||
        max(p1.y, p2.y) < min(q1.y, q2.y) || max(q1.y, q2.y) < min(p1.y, p2.y))
        return false;
    int o1 = sign(cross_product(p1, p2, q1));
    int o2 = sign(cross_product(p1, p2, q2));
    int o3 = sign(cross_product(q1, q2, p1));
    int o4 = sign(cross_product(q1, q2, p2));
    // Use <= to catch touching/collinear overlaps as forbidden (except shared endpoints handled above)
    return (o1 * o2 <= 0) && (o3 * o4 <= 0);
}

// --- Helper Functions ---
Point get_pos(int id, ll n, ll m, const vector<Point> &d_pos, const vector<Point> &s_pos) {
    if (id == n + m) return {0, 5000};
    if (id < n) return d_pos[id];
    return s_pos[id - n];
}
long long dist_sq(Point p1, Point p2) {
    return (long long)(p1.x - p2.x) * (p1.x - p2.x) + (long long)(p1.y - p2.y) * (p1.y - p2.y);
}
bool has_path(int start, int end, int num_nodes, const vector<vector<int>> &adj) {
    queue<int> q;
    q.push(start);
    vector<bool> visited(num_nodes, false);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == end) return true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return false;
}

// --- Core Logic ---
// Hungarian algorithm (max) to compute optimal assignment sum
double hungarian_max(const vector<vector<double>> &w) {
    int n = (int)w.size();
    vector<double> u(n + 1), v(n + 1);
    vector<int> p(n + 1), way(n + 1);
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        int j0 = 0;
        vector<double> minv(n + 1, 1e100);
        vector<char> used(n + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            double delta = 1e100;
            for (int j = 1; j <= n; j++) if (!used[j]) {
                double cur = -(w[i0 - 1][j - 1]) - u[i0] - v[j];
                if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                if (minv[j] < delta) { delta = minv[j]; j1 = j; }
            }
            for (int j = 0; j <= n; j++) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else { minv[j] -= delta; }
            }
            j0 = j1;
        } while (p[j0] != 0);
        do { int j1 = way[j0]; p[j0] = p[j1]; j0 = j1; } while (j0);
    }
    vector<int> match_row(n + 1);
    for (int j = 1; j <= n; j++) match_row[p[j]] = j;
    double value = 0.0;
    for (int i = 1; i <= n; i++) value += w[i - 1][match_row[i] - 1];
    return value;
}

double calculate_and_set_score(State &state, const vector<vector<double>> &p, const vector<Point> &d_pos,
                               const vector<Point> &s_pos) {
    ll n = state.n, m = state.m;
    int inlet_id = n + m, num_nodes = n + m + 1;

    vector<vector<int>> adj(num_nodes), rev_adj(num_nodes);
    vector<int> out_degree(num_nodes, 0);

    for (const auto &edge : state.edges) {
        adj[edge.u].push_back(edge.v);
        rev_adj[edge.v].push_back(edge.u);
        out_degree[edge.u]++;
    }

    vector<vector<double>> prob_at_node(num_nodes, vector<double>(n, 0.0));
    queue<int> q;
    rep(i, num_nodes) if (out_degree[i] == 0) q.push(i);
    vector<int> sorted_nodes;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        sorted_nodes.push_back(u);
        for (int v : rev_adj[u]) {
            out_degree[v]--;
            if (out_degree[v] == 0) q.push(v);
        }
    }
    reverse(sorted_nodes.begin(), sorted_nodes.end());
    rep(j, n) prob_at_node[inlet_id][j] = 1.0;

    for (int u : sorted_nodes) {
        rep(j, n) {
            if (prob_at_node[u][j] > 1e-9) {
                if (u >= n && u < n + m) { // Sorter
                    int sorter_idx = u - n;
                    if (state.sorter_k[sorter_idx] != -1 && adj[u].size() > 0) {
                        int k = state.sorter_k[sorter_idx];
                        int v1 = adj[u][0];
                        int v2 = (adj[u].size() > 1 ? adj[u][1] : adj[u][0]);
                        prob_at_node[v1][j] += prob_at_node[u][j] * p[k][j];
                        prob_at_node[v2][j] += prob_at_node[u][j] * (1.0 - p[k][j]);
                    }
                } else { // Inlet or device
                    for (int v : adj[u]) prob_at_node[v][j] += prob_at_node[u][j];
                }
            }
        }
    }

    vector<vector<double>> prob_at_dest(n, vector<double>(n, 0.0));
    rep(i, n) rep(j, n) prob_at_dest[i][j] = prob_at_node[i][j];

    // Hungarian assignment, record mapping as well
    int N = (int)n;
    // compute maximum sum
    double total_q = hungarian_max(prob_at_dest);
    // retrieve matching by running Hungarian again with tracking
    vector<double> u2(N + 1), v2(N + 1);
    vector<int> p2(N + 1), way2(N + 1);
    for (int i = 1; i <= N; i++) {
        p2[0] = i;
        int j0 = 0;
        vector<double> minv(N + 1, 1e100);
        vector<char> used(N + 1, false);
        do {
            used[j0] = true;
            int i0 = p2[j0], j1 = 0;
            double delta = 1e100;
            for (int j = 1; j <= N; j++) if (!used[j]) {
                double cur = -(prob_at_dest[i0 - 1][j - 1]) - u2[i0] - v2[j];
                if (cur < minv[j]) { minv[j] = cur; way2[j] = j0; }
                if (minv[j] < delta) { delta = minv[j]; j1 = j; }
            }
            for (int j = 0; j <= N; j++) {
                if (used[j]) { u2[p2[j]] += delta; v2[j] -= delta; }
                else { minv[j] -= delta; }
            }
            j0 = j1;
        } while (p2[j0] != 0);
        do { int j1 = way2[j0]; p2[j0] = p2[j1]; j0 = j1; } while (j0);
    }
    vector<int> match_row(N + 1);
    for (int j = 1; j <= N; j++) match_row[p2[j]] = j;
    fill(state.d.begin(), state.d.end(), -1);
    for (int i = 1; i <= N; i++) state.d[i - 1] = match_row[i] - 1;

    double base_score = round(1e9 * (1.0 - total_q / n));

    ll num_intersections = 0;
    rep(i, state.edges.size()) {
        for (size_t j = i + 1; j < state.edges.size(); ++j) {
            const auto &e1 = state.edges[i];
            const auto &e2 = state.edges[j];
            Point p1_u     = get_pos(e1.u, n, m, d_pos, s_pos);
            Point p1_v     = get_pos(e1.v, n, m, d_pos, s_pos);
            Point p2_u     = get_pos(e2.u, n, m, d_pos, s_pos);
            Point p2_v     = get_pos(e2.v, n, m, d_pos, s_pos);
            if (segments_intersect(p1_u, p1_v, p2_u, p2_v)) { num_intersections++; }
        }
    }

    state.score = base_score + 1e12 * num_intersections;
    return total_q;
}

void build_initial_topology(State &state, const vector<Point> &d_pos, const vector<Point> &s_pos) {
    ll n = state.n, m = state.m;
    int inlet_id = n + m;

    // status
    vector<bool> sorter_used(m, false);
    int num_nodes = n + m + 1;
    vector<vector<int>> adj(num_nodes);

    // helper lambda to test intersection with existing edges
    auto can_connect = [&](int u, int v) -> bool {
        Point pu = get_pos(u, n, m, d_pos, s_pos);
        Point pv = get_pos(v, n, m, d_pos, s_pos);
        for (const auto &e : state.edges) {
            if ((e.u == u && e.v == v) || (e.u == v && e.v == u)) continue;
            // share endpoint allowed
            if (e.u == u || e.u == v || e.v == u || e.v == v) continue;
            Point p1 = get_pos(e.u, n, m, d_pos, s_pos);
            Point p2 = get_pos(e.v, n, m, d_pos, s_pos);
            if (segments_intersect(pu, pv, p1, p2)) return false;
        }
        // DAG check: adding u->v must not create a cycle (no path v -> u)
        if (has_path(v, u, num_nodes, adj)) return false;
        return true;
    };

    // choose first sorter closest to inlet
    int first_sorter = -1;
    long long best_d = -1;
    rep(i, m) {
        long long d = dist_sq({0, 5000}, s_pos[i]);
        if (first_sorter == -1 || d < best_d) {
            best_d = d;
            first_sorter = i;
        }
    }
    int first_id = n + first_sorter;
    state.edges.push_back({inlet_id, first_id});
    adj[inlet_id].push_back(first_id);
    sorter_used[first_sorter] = true;
    state.sorter_k[first_sorter] = 0; // temporary

    struct Outlet {int node; int remaining;};
    deque<Outlet> outlets;
    outlets.push_back({first_id, 2});

    // greedy addition: for each sorter node, try to connect one dest and one new sorter
    int guard_iter = 0;
    while(!outlets.empty()) {
        if (++guard_iter > 200000) break; // safety guard
        auto cur = outlets.front();
        outlets.pop_front();
        if (cur.remaining==0) continue;
        int u = cur.node;
        Point pu = get_pos(u, n, m, d_pos, s_pos);
        int added = 0;
        int chosen_dest1 = -1;
        // first: nearest device
        int best_dest1=-1; long long bd1=LLONG_MAX;
        rep(i,n) {
            if(!can_connect(u,i)) continue;
            long long d=dist_sq(pu,d_pos[i]);
            if(d<bd1){bd1=d;best_dest1=i;}
        }
        if(best_dest1!=-1 && added < cur.remaining){
            state.edges.push_back({u,best_dest1});
            adj[u].push_back(best_dest1);
            chosen_dest1 = best_dest1;
            added++;
        }
        // second: nearest new sorter
        int best_sorter=-1; long long bs=LLONG_MAX;
        rep(i,m) if(!sorter_used[i]) {
            int vid=n+i;
            if(!can_connect(u,vid)) continue;
            long long d=dist_sq(pu,s_pos[i]);
            if(d<bs){bs=d;best_sorter=i;}
        }
        if(best_sorter!=-1 && added < cur.remaining){
            int vid=n+best_sorter;
            state.edges.push_back({u,vid});
            adj[u].push_back(vid);
            sorter_used[best_sorter]=true;
            state.sorter_k[best_sorter]=0;
            outlets.push_back({vid,2});
            added++;
        }
        // if room remains, try second device distinct from first
        if(added < cur.remaining){
            int best_dest2=-1; long long bd2=LLONG_MAX;
            rep(i,n){
                if(i==chosen_dest1) continue;
                if(!can_connect(u,i)) continue;
                long long d=dist_sq(pu,d_pos[i]);
                if(d<bd2){bd2=d;best_dest2=i;}
            }
            if(best_dest2!=-1){
                state.edges.push_back({u,best_dest2});
                adj[u].push_back(best_dest2);
                added++;
            }
        }
        // if could not attach anything, drop; else if remaining exits, requeue
        int rem = cur.remaining - added;
        if(added == 0) {
            // drop this outlet to avoid infinite loops
            continue;
        }
        if(rem > 0) outlets.push_front({u, rem});
    }

    // do not add extra edges from inlet (must be exactly one)
}


// --- Main ---
int main() {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, m, k;
    cin >> n >> m >> k;
    vector<Point> d_pos(n);
    rep(i, n) cin >> d_pos[i].x >> d_pos[i].y;
    vector<Point> s_pos(m);
    rep(i, m) cin >> s_pos[i].x >> s_pos[i].y;
    vector<vector<double>> p(k, vector<double>(n));
    rep(i, k) rep(j, n) cin >> p[i][j];

    State current_state(n, m, k);
    build_initial_topology(current_state, d_pos, s_pos);
    double best_sum = calculate_and_set_score(current_state, p, d_pos, s_pos);
    State best_state = current_state;

    double time_limit = 1.98;
    // Collect installed sorters
    vector<int> installed;
    rep(i, m) if (current_state.sorter_k[i] != -1) installed.push_back(i);

    // Initialize sorter types by a heuristic (maximize |p-0.5| sum)
    rep(idx, installed.size()) {
        int sidx = installed[idx];
        double best_sc = -1.0; int best_k = 0;
        rep(kk, k) {
            double val = 0.0;
            rep(j, n) val += fabs(p[kk][j] - 0.5);
            if (val > best_sc) { best_sc = val; best_k = (int)kk; }
        }
        current_state.sorter_k[sidx] = best_k;
    }
    best_sum   = calculate_and_set_score(current_state, p, d_pos, s_pos);
    best_state = current_state;

    bool improved = true;
    while (improved) {
        improved = false;
        for (int sidx : installed) {
            if (get_time() > time_limit) break;
            int orig = current_state.sorter_k[sidx];
            int bestk = orig;
            double local_best = best_sum;
            rep(kk, k) if (kk != orig) {
                current_state.sorter_k[sidx] = kk;
                double sum = calculate_and_set_score(current_state, p, d_pos, s_pos);
                if (sum > local_best + 1e-12) { local_best = sum; bestk = (int)kk; }
            }
            current_state.sorter_k[sidx] = bestk;
            if (bestk != orig) { best_sum = local_best; best_state = current_state; improved = true; }
        }
        if (get_time() > time_limit) break;
    }

    // Output
    vector<int> v1_out(m, -1), v2_out(m, -1);
    int inlet_s = -1;
    for (const auto &edge : best_state.edges) {
        if (edge.u == n + m) inlet_s = edge.v;
        else if (edge.u >= n) {
            int sorter_idx = edge.u - n;
            if (v1_out[sorter_idx] == -1) v1_out[sorter_idx] = edge.v;
            else v2_out[sorter_idx] = edge.v;
        }
    }

    rep(i, n) cout << best_state.d[i] << (i == n - 1 ? "" : " ");
    cout << el;
    cout << inlet_s << el;
    rep(i, m) {
        if (best_state.sorter_k[i] == -1) cout << -1 << el;
        else {
            if (v1_out[i] != -1 && v2_out[i] == -1) v2_out[i] = v1_out[i];
            else if (v1_out[i] == -1 && v2_out[i] != -1) v1_out[i] = v2_out[i];

            cout << best_state.sorter_k[i] << spa << v1_out[i] << spa << v2_out[i] << el;
        }
    }

    return 0;
}
