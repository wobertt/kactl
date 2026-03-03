/**
 * Author: Robert Chung
 * License: CC0
 * Source: CP-algorithms
 * Description: Creates an expression tree for left-associative operations.
 * Change N and prec() as needed.
 * Usage: Val *root = parseExpression(s); // tree dp...
 * Time: O(N)
 * Status: tested on https://dmoj.ca/problem/acsl1p6, https://dmoj.ca/problem/riddletroll
 */
#pragma once

const int N = 2e6; // max node count in expr tree. |S| is enough
struct Val { // 32 bytes/node
    int v; // only used if op = 0
    char op; Val *l, *r; // only used if op != 0
    Val(int v=0) : v(v), op(0), l(0), r(0) {}
    Val(char op, Val *l, Val *r) : op(op), l(l), r(r) {}
};
Val arr[N]; int last = 0;

Val *parseExpression(string &s) {
    using C = array<int, 3>; // precedence comparator
    vector<pair<C, char>> ops;
    vector<Val*> vals;
    int depth = 0;

    auto prec = [&](char c, int i) -> C { // change to your ops
        int r;
        if (c == '+' || c == '-') r = 0;
        else if (c == '*' || c == '/' || c == '_') r = 1;
        else assert(0); // i for right assoc, -i for left
        return {depth, r, c == '_' ? i : -i}; 
    };
    auto clear = [&](C minp) {
        while (!ops.empty()) {
            auto [p, c] = ops.back();
            if (p < minp) break;
            ops.pop_back();
            auto b = vals.back(); vals.pop_back();
            if (c == '_') {  // unary op
                vals.pb(&(arr[last++] = Val(c, b, 0)));
            } else { // binary op
                auto a = vals.back(); vals.pop_back();
                vals.pb(&(arr[last++] = Val(c, a, b)));
            }
        }
    };
    for (int i = -1; char c : s) { i++;
        bool pdigit = i >= 1 && '0' <= s[i-1] && s[i-1] <= '9';
        if (c == '(') depth++;
        else if (c == ')') depth--;
        else if ('0' <= c && c <= '9') {
            if (pdigit) vals.back()->v = 10*vals.back()->v + c-'0';
            else vals.pb(&(arr[last++] = Val(c-'0')));
        } else {
            if (c == '-' && !pdigit
                && !(i >= 1 && s[i-1] == ')')) c = '_'; // unary op
            clear(prec(c, i)); ops.pb({prec(c, i), c});
        }
    }
    clear({-1});
    return vals[0];
}