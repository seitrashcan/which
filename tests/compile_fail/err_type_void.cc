// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.
//
// Expected to fail: `which<T, E>` requires E to not be `void`.

#include <mhx/which>

int main() {
    mhx::which<int, void> w = mhx::which<int, void>::ok(1);
    (void)w;
}
