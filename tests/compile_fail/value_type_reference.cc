// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.
//
// Expected to fail: `which<T, E>` does not support reference types for T.

#include <mhx/which>

int main() {
    int x = 1;
    mhx::which<int&, int> w = mhx::which<int&, int>::ok(x);
    (void)w;
}
