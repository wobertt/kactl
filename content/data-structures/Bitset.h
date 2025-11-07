/**
 * Author: None
 * Date: 2025-11-05
 * License: CC0
 * Source: https://en.cppreference.com/w/cpp/utility/bitset.html
 * Description: None
 */
#pragma once

void solve() {
  bitset<4> b;
  // operations on individual bits:
  b[0] = 1; // 0001
  b.flip(3); // 1001 (toggle single bit)
  // bitwise operations:
  b |= 0b0100; // 1101
  b ^= 0b0011; // 1110
  b &= 0b1100; // 1100
  auto b2 = ~b; // 0011
  // operations on the whole set:
  b.flip(); // 0011 (toggle all bits)
  b.reset(); // 0000
  b.set(); // 1111
  cout << b.count(); // 4 (popcount)
  // conversions:
  cout << b2.to_string(); // "0011"
  cout << b2.to_ullong(); // 3
}
