#include "Optimizer.h"
#include <array>
#include <functional>
#include <iostream>
#include <set>

Optimizer::Optimizer(Data d) {
    data = d;
    dispo first = solve();
    history.push_back(Step { -1, -1, first });
    pos = 0;
}

dispo Optimizer::exclude(int valv) {
    dispo d_old = get_disp();
    int spes = d_old[valv];
    if (spes==-1) return d_old;

    while (history.size()>pos+1) {
        Step top = history.back(); history.pop_back();
        exclusions[top.valvola].erase(top.misura);
    }

    exclusions[valv].insert(spes);
    dispo d = solve();
    pos++;

    history.push_back(Step{valv, spes, d});
    return d;
}

dispo Optimizer::get_disp() {
    return history.at(pos).disposizione;
}

dispo Optimizer::undo() {
    if (pos>0) pos--;
    return get_disp();
}

dispo Optimizer::redo() {
    if (pos+1 < history.size()) pos++;
    return get_disp();
}

bool Optimizer::can_undo() {
    return pos>0;
}

bool Optimizer::can_redo() {
    return pos+1<history.size();
}

dispo match(std::array<std::set<int>, 16> compatibility) {
    dispo result;
    result.fill(-1);

    // matchB[v] = u means B_v is currently matched to A_u (or -1)
    std::vector<int> matchB(16, -1);

    // DFS trying to find augmenting path from left node u
    std::function<bool(int, std::vector<char>&)> dfs = [&](int u, std::vector<char>& seen) -> bool {
        for (auto it = compatibility[u].rbegin(); it != compatibility[u].rend(); ++it) {
            int v = *it;
            if (v < 0 || v >= 16) continue;
            if (seen[v]) continue;
            seen[v] = 1;
            if (matchB[v] == -1 || dfs(matchB[v], seen)) {
                matchB[v] = u;
                return true;
            }
        }
        return false;
    };

    // Try to find matches for every left node
    for (int u = 0; u < 16; ++u) {
        std::vector<char> seen(16, 0);
        dfs(u, seen);
    }

    // Build result from matchB
    for (int v = 0; v < 16; ++v) {
        if (matchB[v] != -1) result[matchB[v]] = v;
    }

    return result;
}

dispo Optimizer::solve() {
    std::array<int, 16> sums;
    std::array<int, 16> spessori;
    for (int i=0; i<8; i++) {
        sums[i] = data.asp_misure[i] + data.asp_spessori[i];
        spessori[i] = data.asp_spessori[i];
    }
    for (int i=0; i<8; i++) {
        sums[i+8] = data.sca_misure[i] + data.sca_spessori[i];
        spessori[i+8] = data.sca_spessori[i];
    }

    std::array<std::set<int>, 16> compatibility;
    for (int i=0; i<8; i++) { //asp_misure
        for (int j=0; j<16; j++) { //spessori
            int t = sums[i]-spessori[j];
            if ( exclusions.at(i).find(j) == exclusions.at(i).end()
                    && data.asp_low <= t && t <= data.asp_high) {
                compatibility[i].insert(j);
            }
        }
    }
    for (int i=0; i<8; i++) { //sca_misure
        for (int j=0; j<16; j++) { //spessori
            int I = i+8;
            int t = sums[I]-spessori[j];
            if ( exclusions.at(I).find(j) == exclusions.at(I).end()
                    && data.sca_low <= t && t <= data.sca_high) {
                compatibility[I].insert(j);
            }
        }
    }

    return match(compatibility);
}
