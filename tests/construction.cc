// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.

#include <gtest/gtest.h>

#include <mhx/which>

#include <string>

using mhx::which;

TEST(Construction, OkHoldsValue) {
    auto w = which<int, std::string>::ok(42);
    EXPECT_TRUE(w.is_ok());
    EXPECT_FALSE(w.is_err());
    EXPECT_EQ(w.value(), 42);
}

TEST(Construction, ErrHoldsValue) {
    auto w = which<int, std::string>::err("something went wrong");
    EXPECT_TRUE(w.is_err());
    EXPECT_FALSE(w.is_ok());
    EXPECT_EQ(w.error(), "something went wrong");
}

TEST(Construction, OkForwardsRvalue) {
    auto w = which<std::string, int>::ok(std::string("hello"));
    EXPECT_TRUE(w.is_ok());
    EXPECT_EQ(w.value(), "hello");
}

TEST(Construction, ErrForwardsRvalue) {
    auto w = which<int, std::string>::err(std::string("boom"));
    EXPECT_TRUE(w.is_err());
    EXPECT_EQ(w.error(), "boom");
}

TEST(Construction, OkFromLvalue) {
    std::string s = "lvalue";
    auto w = which<std::string, int>::ok(s);
    EXPECT_TRUE(w.is_ok());
    EXPECT_EQ(w.value(), "lvalue");
    // Source is untouched; `ok()` copies from an lvalue.
    EXPECT_EQ(s, "lvalue");
}

TEST(Construction, ErrFromLvalue) {
    std::string e = "err-lvalue";
    auto w = which<int, std::string>::err(e);
    EXPECT_TRUE(w.is_err());
    EXPECT_EQ(w.error(), "err-lvalue");
    EXPECT_EQ(e, "err-lvalue");
}

TEST(Construction, CopyConstructionPreservesOk) {
    auto w1 = which<int, std::string>::ok(7);
    auto w2 = w1;
    EXPECT_TRUE(w2.is_ok());
    EXPECT_EQ(w2.value(), 7);
    // Original is unaffected by copy.
    EXPECT_TRUE(w1.is_ok());
    EXPECT_EQ(w1.value(), 7);
}

TEST(Construction, CopyConstructionPreservesErr) {
    auto w1 = which<int, std::string>::err("copied error");
    auto w2 = w1;
    EXPECT_TRUE(w2.is_err());
    EXPECT_EQ(w2.error(), "copied error");
    EXPECT_TRUE(w1.is_err());
}

TEST(Construction, MoveConstructionPreservesOk) {
    auto w1 = which<std::string, int>::ok("moved");
    auto w2 = std::move(w1);
    EXPECT_TRUE(w2.is_ok());
    EXPECT_EQ(w2.value(), "moved");
}

TEST(Construction, MoveConstructionPreservesErr) {
    auto w1 = which<int, std::string>::err("moved error");
    auto w2 = std::move(w1);
    EXPECT_TRUE(w2.is_err());
    EXPECT_EQ(w2.error(), "moved error");
}

TEST(Construction, NotDefaultConstructible) {
    static_assert(!std::is_default_constructible_v<which<int, std::string>>);
}

TEST(Construction, NotCopyAssignable) {
    static_assert(!std::is_copy_assignable_v<which<int, std::string>>);
}

TEST(Construction, NotMoveAssignable) {
    static_assert(!std::is_move_assignable_v<which<int, std::string>>);
}
