// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.

#include <gtest/gtest.h>

#include <mhx/which>

#include <memory>
#include <string>

using mhx::which;

TEST(Accessors, ArrowOperatorOnOk) {
    auto w = which<std::string, int>::ok("payload");
    ASSERT_NE(w.operator->(), nullptr);
    EXPECT_EQ(w->size(), 7u);
}

TEST(Accessors, ArrowOperatorOnErr) {
    auto w = which<std::string, int>::err(99);
    EXPECT_EQ(w.operator->(), nullptr);
}

TEST(Accessors, ConstArrowOperator) {
    const auto w = which<std::string, int>::ok("const-payload");
    ASSERT_NE(w.operator->(), nullptr);
    EXPECT_EQ(w->size(), 13u);
}

TEST(Accessors, DereferenceLvalue) {
    auto w = which<int, std::string>::ok(5);
    EXPECT_EQ(*w, 5);
    *w = 6;
    EXPECT_EQ(*w, 6);
}

TEST(Accessors, DereferenceConstLvalue) {
    const auto w = which<int, std::string>::ok(5);
    EXPECT_EQ(*w, 5);
}

TEST(Accessors, DereferenceRvalue) {
    auto make = [] { return which<std::string, int>::ok("rvalue-payload"); };
    std::string moved = *make();
    EXPECT_EQ(moved, "rvalue-payload");
}

TEST(Accessors, ValueRvalueMovesOut) {
    auto make = [] { return which<std::unique_ptr<int>, int>::ok(std::make_unique<int>(42)); };
    auto ptr = std::move(make()).value();
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(Accessors, ErrorRvalueMovesOut) {
    auto make = [] { return which<int, std::unique_ptr<int>>::err(std::make_unique<int>(7)); };
    auto ptr = std::move(make()).error();
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 7);
}

TEST(Accessors, ValueAndErrorAreIndependentStorage) {
    auto w = which<int, std::string>::ok(3);
    EXPECT_EQ(w.value(), 3);
    w.value() = 4;
    EXPECT_EQ(w.value(), 4);
}
