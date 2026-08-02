// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.

#include <gtest/gtest.h>

#include <mhx/which>

#include <string>
#include <tuple>
#include <type_traits>

using mhx::which;

TEST(StructuredBindings, OkYieldsNonNullValAndNullErr) {
    auto [val, err] = which<int, std::string>::ok(10);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 10);
    EXPECT_EQ(err, nullptr);
}

TEST(StructuredBindings, ErrYieldsNullValAndNonNullErr) {
    auto [val, err] = which<int, std::string>::err("failure");
    EXPECT_EQ(val, nullptr);
    ASSERT_NE(err, nullptr);
    EXPECT_EQ(*err, "failure");
}

TEST(StructuredBindings, MutatingBoundValMutatesUnderlyingStorage) {
    auto w = which<int, std::string>::ok(1);
    auto& [val, err] = w;
    ASSERT_NE(val, nullptr);
    *val = 2;
    EXPECT_EQ(w.value(), 2);
}

TEST(StructuredBindings, ConstBinding) {
    const auto w = which<int, std::string>::ok(21);
    auto [val, err] = w;
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 21);
    EXPECT_EQ(err, nullptr);
}

TEST(StructuredBindings, RvalueBinding) {
    auto make = [] { return which<int, std::string>::ok(99); };
    auto [val, err] = make();
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 99);
}

TEST(StructuredBindings, TupleSizeIsTwo) {
    static_assert(std::tuple_size_v<which<int, std::string>> == 2);
}

TEST(StructuredBindings, TupleElementTypes) {
    static_assert(std::is_same_v<std::tuple_element_t<0, which<int, std::string>>, int*>);
    static_assert(std::is_same_v<std::tuple_element_t<1, which<int, std::string>>, std::string*>);
}
