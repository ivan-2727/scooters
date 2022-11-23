#include <bits/stdc++.h>
using namespace std; 
using lli = long long int;

int cap = 25;
int n, m, k; 
vector<vector<int>> D;
vector<vector<int>> lim;

class Path {
public:
    vector<int> order;
    unordered_set<int> incl;
    int dist;
    Path() {dist = 0;}
    bool has(int idx) {
        return incl.find(idx) != incl.end(); 
    }
    void add(int idx) {
        if (idx != 0) dist += D[order.back()][idx]; 
        order.push_back(idx);
        incl.insert(idx);
    }
};

int main(void) {
    int tests = 30;
    int tot = 0;
    int max_tot = 0;
    for (int test = 1; test <= tests; test++) {
        ifstream myin; myin.open("input" + to_string(test) + ".txt"); 
        //myin.open("sample.txt");
        myin >> n >> m >> k;
        D = vector <vector<int>> (n+m+1, vector<int>(n+m+1));
        for (int i = 0; i <= n+m; i++) {
            for (int j = 0; j <= n+m; j++) {
                myin >> D[i][j]; 
            }
        }
        lim = vector<vector<int>> (k, vector<int>(2));
        for (int l = 0; l < k; l++) {
            myin >> lim[l][0];
            lim[l][1] = l; 
        }
        sort(lim.begin(), lim.end());

        vector<vector<int>> ans(k); 
        vector <bool> vis(n+m+1, false); 
        for (int l = 0; l < k; l++) {
            vector<vector<Path>> dp(n+1, vector<Path>(cap+1));
            dp[0][0].add(0);
            for (int x = 0; x <= n; x++) {
                for (int t = 0; t <= cap; t++) {
                    if (dp[x][t].order.empty()) continue;
                    if (t+1<=cap) {
                        for (int i = 1; i <= n; i++) {
                            if (vis[i]) continue; 
                            if (dp[x][t].has(i)) continue; 
                            int upd_dist = dp[x][t].dist + D[dp[x][t].order.back()][i]; 
                            if (upd_dist > lim[l][0]) continue;
                            if (dp[x][t+1].order.empty() || dp[x][t+1].dist > upd_dist) {
                                dp[x][t+1] = dp[x][t];
                                dp[x][t+1].add(i);
                            }
                        }
                    }
                    if (t-1>=0 && x+1<=n) {
                        for (int i = n+1; i <= n+m; i++) {
                            if (vis[i]) continue; 
                            if (dp[x][t].has(i)) continue; 
                            int upd_dist = dp[x][t].dist + D[dp[x][t].order.back()][i]; 
                            if (upd_dist > lim[l][0]) continue;
                            if (dp[x+1][t-1].order.empty() || dp[x+1][t-1].dist > upd_dist) {
                                dp[x+1][t-1] = dp[x][t];
                                dp[x+1][t-1].add(i);
                            }
                        }
                    }
                }
            }

            for (int x = n; x >= 0; x--) {
                if (!dp[x][0].order.empty()) {
                    ans[lim[l][1]] = dp[x][0].order;
                    for (int i : ans[lim[l][1]]) vis[i] = true; 
                    break;
                }
            }
        }
        int res = 0; 
        for (int l = 0; l < k; l++) {
            printf("Car #%d: (%d) ", l, ans[l].size()-1);
            res += (ans[l].size()-1)/2; 
            for (int i : ans[l]) printf("%d ", i);
            printf("\n");
        }
        tot += res; 
        max_tot += n; 
        printf("Delivered in case #%d: %d of %d\n", test, res, n);
    }
    printf("Total delivered in all tests: %d of %d\n", tot, max_tot);
    return 0;
}