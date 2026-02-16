/**
 * Author: folklore
 * Date: 2025-11-06
 * License: CC0
 * Source: https://codeforces.com/blog/entry/61587
 * Description:
 * High-quality uniform RNG for integers.
 * Usage: urand(a, b) returns an integer in [a, b].
 * Time: 400ms for 1e8 numbers.
 * Status: untested
 */
// can also use mt19937_64 for 64-bit integers
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int urand(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }