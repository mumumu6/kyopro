#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'

// ----------------- 評価関数 -----------------
double eval(const vector<int> &P,
            const vector<char> &base1d,
            int N, int trials,
            double bestSoFar = -1e100) // 早期打切り
{
    static mt19937 rng(123456);
    uniform_int_distribution<int> dir4(0, 3);

    /* 空きマス一覧 */
    static vector<int> empties;
    empties.clear();
    empties.reserve(N * N);
    for (int i = 0; i < N * N; i++)
        if (base1d[i] == '.') empties.push_back(i);
    uniform_int_distribution<int> randStart(0, (int)empties.size() - 1);

    double sum = 0;
    vector<char> g = base1d;            // 作業盤面
    for (int t = 0; t < trials; t++) {
        g.assign(base1d.begin(), base1d.end());
        int robot = empties[randStart(rng)];

        int gain = 0;
        for (int step = 0; step < (int)P.size(); ++step) {
            int y = robot / N, x = robot % N;
            int d = dir4(rng);
            while (true) {
                int ny = y + (d == 0 ? -1 : d == 1 ? 1 : 0);
                int nx = x + (d == 2 ? -1 : d == 3 ? 1 : 0);
                if (ny < 0 || ny >= N || nx < 0 || nx >= N) break;
                int ni = ny * N + nx;
                if (g[ni] == '#') break;
                y = ny;  x = nx;
            }
            robot = y * N + x;

            if (robot == P[step]) break;    // 潰された
            ++gain;
            g[P[step]] = '#';               // 岩を置く
        }
        sum += gain;

        /* 早期打ち切り */
        if (bestSoFar > -1e90) {
            double maxPos = sum + (trials - t - 1) * (int)P.size();
            if (maxPos < bestSoFar * trials) return -1e100;
        }
    }
    return sum / trials;
}

/* 近傍 2 種 */
inline void two_swap(vector<int>& P, mt19937& rng){
    int n = P.size();
    uniform_int_distribution<int> R(0,n-1);
    int i = R(rng), j = R(rng);
    swap(P[i], P[j]);
}
inline void short_rev(vector<int>& P, mt19937& rng){
    int n = P.size();
    uniform_int_distribution<int> L(0,n-4);
    int l = L(rng);
    int r = min(n-1, l + (int)uniform_int_distribution<int>(3,8)(rng));
    reverse(P.begin()+l, P.begin()+r+1);
}

/* ==================== main ==================== */
int main(){
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);

    int N, M;  cin >> N >> M;
    vector<vector<char>> s(N, vector<char>(N));
    rep(i,N) rep(j,N) cin >> s[i][j];

    /* 1 次元 flatten */
    vector<char> base1d(N*N,'.');
    rep(i,N) rep(j,N) base1d[i*N+j] = s[i][j];

    /* --- Greedy で初期列 P を生成 (同じロジックをそのまま転用) --- */
    vector<int> P;
    vector<vector<char>> tmp = s;
    int remain = N*N - M;
    rep(step, remain){
        int bestIdx = -1, bestCost = 10;
        rep(i,N) rep(j,N){
            if(tmp[i][j]=='#') continue;
            int cnt = 0;
            if(i==0 || tmp[i-1][j]=='#') cnt++;
            if(i==N-1 || tmp[i+1][j]=='#') cnt++;
            if(j==0 || tmp[i][j-1]=='#') cnt++;
            if(j==N-1 || tmp[i][j+1]=='#') cnt++;
            if(cnt < bestCost){
                bestCost = cnt;
                bestIdx  = i*N + j;
            }
        }
        P.push_back(bestIdx);
        tmp[bestIdx / N][bestIdx % N] = '#';
    }

    /* --- 焼きなまし (2 秒) --- */
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    double cur = eval(P, base1d, N, 200);
    double best = cur;
    vector<int> bestP = P;

    const double TL = 1.95;
    auto t0 = chrono::steady_clock::now();
    int iter = 0;

    while(true){
        double t = chrono::duration<double>(chrono::steady_clock::now()-t0).count();
        if(t > TL) break;
        double ratio = t / TL;
        double T = 0.01 + (1.0-0.01)*(1.0-ratio);

        vector<int> nxt = P;
        if(rng() & 1) two_swap(nxt, rng);
        else          short_rev(nxt, rng);

        int trials = 100 + (int)(300 * ratio);
        double val = eval(nxt, base1d, N, trials, best);

        double diff = val - cur;
        double prob = (diff>0) ? 1.0 : exp(diff / T);
        if(prob > (double)rng() / rng.max()){
            P.swap(nxt); cur = val;
            if(cur > best){ best = cur; bestP = P; }
        }
        ++iter;
    }

    /* --- 出力 --- */
    for(int idx : bestP) cout << idx / N << spa << idx % N << el;
}
