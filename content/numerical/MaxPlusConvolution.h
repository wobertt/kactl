/**
 * Author: Arnav Sastry
 * Date: 2024-10-29
 * Description: Given arrays $a$, $b$ where $b$ is concave,
 *   return the array $c_k = \max_{i+j=k}(a_i + b_j)$ of size equal to $a$.
 *   A zero is implicitly inserted at the start of $b$.
 * Source: http://serjudging.vanb.org/?p=2081
 * Time: O(N \log N)
 * Status: Tested on https://codeforces.com/problemset/problem/808/E, https://open.kattis.com/problems/balatro/
 */
using vll = vector<ll>;
vll maxPlusConv(const vll& a, const vll& b) {
	if (b.empty()) return a;
	using Seg = array<ll, 3>;

	auto eval = [&](Seg& seg, ll x) {
		ll offset = x - seg[0];
		assert(0 <= seg[1]+offset && seg[1]+offset < sz(b));
		return b[seg[1] + offset] + seg[2];
	};
	auto endIndex = [&](Seg& seg) {
		return seg[0] + sz(b) - seg[1];
	};

	vector<Seg> stk;
	rep(i, 0, sz(a)) {
		Seg seg{i + 1, 0, a[i]};
		while (!stk.empty()) {
			ll idx = stk.back()[0];
			if (idx<seg[0] || eval(stk.back(), idx) > eval(seg, idx))
				break;
			stk.pop_back();
		}

		if (!stk.empty() && endIndex(stk.back()) > seg[0]
				&& eval(stk.back(), seg[0]) > eval(seg, seg[0])) {
			ll lo = seg[0];
			ll hi = min(endIndex(stk.back()), endIndex(seg));
			while (lo + 1 < hi) {
				ll mid = midpoint(lo, hi);
				if (eval(stk.back(), mid) > eval(seg, mid)) lo = mid;
				else hi = mid;
			}
			seg[1] = hi - seg[0];
			seg[0] = hi;
		}
		stk.push_back(seg);
	}

	vll ans = a;
	auto it = stk.begin();
	rep(i, 1, sz(ans)) {
		while (next(it) != stk.end() && (*next(it))[0] <= i)
			++it;
		ans[i] = max(ans[i], eval(*it, i));
	}

	return ans;
}
