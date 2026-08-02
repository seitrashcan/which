// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.

#include <gtest/gtest.h>

#include <mhx/which>

#include <memory>
#include <string>
#include <type_traits>
#include <utility>

using mhx::which;

TEST(MoveSemantics, MoveOnlyOkTransfersOwnership) {
    auto w1 = which<std::unique_ptr<int>, int>::ok(std::make_unique<int>(11));
    ASSERT_TRUE(w1.is_ok());
    auto w2 = std::move(w1);
    ASSERT_TRUE(w2.is_ok());
    ASSERT_NE(w2.value(), nullptr);
    EXPECT_EQ(*w2.value(), 11);
}

TEST(MoveSemantics, MoveOnlyErrTransfersOwnership) {
    auto w1 = which<int, std::unique_ptr<std::string>>::err(std::make_unique<std::string>("oops"));
    ASSERT_TRUE(w1.is_err());
    auto w2 = std::move(w1);
    ASSERT_TRUE(w2.is_err());
    ASSERT_NE(w2.error(), nullptr);
    EXPECT_EQ(*w2.error(), "oops");
}

TEST(MoveSemantics, IsMoveConstructible) {
    static_assert(std::is_move_constructible_v<which<std::unique_ptr<int>, int>>);
}

TEST(MoveSemantics, IsCopyConstructibleOnlyWhenAlternativesAre) {
    static_assert(std::is_copy_constructible_v<which<int, std::string>>);
    static_assert(!std::is_copy_constructible_v<which<std::unique_ptr<int>, int>>);
}

TEST(MoveSemantics, ReturnByValueUsesMove) {
    auto make = [] { return which<std::unique_ptr<int>, int>::ok(std::make_unique<int>(5)); };
    auto w = make();
    ASSERT_TRUE(w.is_ok());
    EXPECT_EQ(*w.value(), 5);
}
