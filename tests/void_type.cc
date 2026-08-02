// This file is part of a project that is licensed under Apache-2.0.
// See `LICENSE` for details.

#include <gtest/gtest.h>

#include <mhx/which>

#include <string>

using mhx::which;

TEST(VoidType, OkTakesNoArgument) {
    auto w = which<void, std::string>::ok();
    EXPECT_TRUE(w.is_ok());
    EXPECT_FALSE(w.is_err());
}

TEST(VoidType, ErrStillHoldsErrorValue) {
    auto w = which<void, std::string>::err("void failure");
    EXPECT_TRUE(w.is_err());
    EXPECT_EQ(w.error(), "void failure");
}

TEST(VoidType, StructuredBindingValPointerNonNullOnOk) {
    auto [val, err] = which<void, std::string>::ok();
    EXPECT_NE(val, nullptr);
    EXPECT_EQ(err, nullptr);
}

TEST(VoidType, StructuredBindingValPointerNullOnErr) {
    auto [val, err] = which<void, std::string>::err("nope");
    EXPECT_EQ(val, nullptr);
    ASSERT_NE(err, nullptr);
    EXPECT_EQ(*err, "nope");
}

TEST(VoidType, BoolConversion) {
    auto ok = which<void, std::string>::ok();
    auto err = which<void, std::string>::err("bad");
    EXPECT_TRUE(static_cast<bool>(ok));
    EXPECT_FALSE(static_cast<bool>(err));
}
