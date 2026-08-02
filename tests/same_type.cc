// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.

#include <gtest/gtest.h>

#include <mhx/which>

using mhx::which;

TEST(SameType, OkDisambiguatesFromErr) {
    auto w = which<int, int>::ok(1);
    EXPECT_TRUE(w.is_ok());
    EXPECT_EQ(w.value(), 1);
}

TEST(SameType, ErrDisambiguatesFromOk) {
    auto w = which<int, int>::err(2);
    EXPECT_TRUE(w.is_err());
    EXPECT_EQ(w.error(), 2);
}

TEST(SameType, StructuredBindingWithSameType) {
    auto [val, err] = which<int, int>::ok(3);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 3);
    EXPECT_EQ(err, nullptr);
}

TEST(SameType, CopyPreservesActiveAlternative) {
    auto w1 = which<int, int>::err(4);
    auto w2 = w1;
    EXPECT_TRUE(w2.is_err());
    EXPECT_EQ(w2.error(), 4);
}
