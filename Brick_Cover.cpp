#include <bits/stdc++.h>
using namespace std;

struct brick
{
    int x, y, l;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w, h, k;
    cin >> w >> h >> k;

    int c[10] = {0};
    cin >> c[1] >> c[3] >> c[5] >> c[7] >> c[9];

    vector<vector<int>> holesbyrow(h);
    for (int i = 0; i < k; i++)
    {
        int a, b;
        cin >> a >> b;
        holesbyrow[b].push_back(a);
    }

    vector<int> widths = {1, 3, 5, 7, 9};

    vector<vector<char>> covered(h, vector<char>(w, 0));
    vector<brick> bricks;
    vector<set<int>> pendingforrow(h);

    for (int y = 0; y < h; y++)
    {
        auto hs = holesbyrow[y];
        if (hs.empty())
        {
            continue;
        }
        sort(hs.begin(), hs.end());
        int m = (int)hs.size();
        vector<long long> dp(m + 1, LLONG_MAX / 2);
        vector<int> choicej(m + 1, -1), choicel(m + 1, -1), choicex(m + 1, -1);
        dp[0] = 0;
        for (int i = 1; i <= m; i++)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                int need = hs[i - 1] - hs[j] + 1;
                if (need > 9)
                {
                    break;
                }
                for (int l : widths)
                {
                    if (l < need)
                    {
                        continue;
                    }
                    int x = hs[j];
                    if (x + l > w)
                    {
                        continue;
                    }
                    if (i < m)
                    {
                        int nexthole = hs[i];
                        if (x + l - 1 >= nexthole)
                        {
                            continue;
                        }
                    }
                    long long cost = dp[j] + c[l];
                    if (cost < dp[i])
                    {
                        dp[i] = cost;
                        choicej[i] = j;
                        choicel[i] = l;
                        choicex[i] = x;
                    }
                }
            }
        }
        vector<array<int, 2>> rowbricks;
        int i = m;
        while (i > 0)
        {
            int j = choicej[i];
            int l = choicel[i];
            int x = choicex[i];
            rowbricks.push_back({x, l});
            i = j;
        }
        for (auto &b : rowbricks)
        {
            int x = b[0];
            int l = b[1];
            bricks.push_back({x, y, l});
            for (int p = 0; p < l; p++)
            {
                covered[y][x + p] = 1;
            }
            if (y > 0)
            {
                int center = x + (l - 1) / 2;
                pendingforrow[y - 1].insert(center);
            }
        }
    }

    for (int y = h - 1; y >= 0; y--)
    {
        if (pendingforrow[y].empty())
        {
            continue;
        }
        vector<int> need(pendingforrow[y].begin(), pendingforrow[y].end());
        for (int x : need)
        {
            if (covered[y][x])
            {
                continue;
            }

            int bestlen = 1;
            long long bestcost = c[1];
            int bestx = x;

            for (int l : widths)
            {
                int sx = x - (l - 1) / 2;
                if (sx < 0 || sx + l > w)
                {
                    continue;
                }
                bool ok = true;
                for (int p = 0; p < l; p++)
                {
                    if (covered[y][sx + p])
                    {
                        ok = false;
                        break;
                    }
                }
                if (!ok)
                {
                    continue;
                }
                if (y > 0)
                {
                    int center = sx + (l - 1) / 2;
                    if (!covered[y - 1][center])
                    {
                        ok = false;
                    }
                }
                if (!ok)
                {
                    continue;
                }
                if (c[l] < bestcost)
                {
                    bestcost = c[l];
                    bestlen = l;
                    bestx = sx;
                }
            }

            bricks.push_back({bestx, y, bestlen});
            for (int p = 0; p < bestlen; p++)
            {
                covered[y][bestx + p] = 1;
            }
            if (y > 0)
            {
                int center = bestx + (bestlen - 1) / 2;
                pendingforrow[y - 1].insert(center);
            }
        }
    }

    cout << (int)bricks.size() << endl;
    for (auto &b : bricks)
    {
        cout << b.x << " " << b.y << " " << b.l << endl;
    }

    return 0;
}