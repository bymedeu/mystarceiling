#include <gtest/gtest.h>

#include <mystarceiling/core/maths/vector3.hh>

using msc::core::maths::Vector3;

namespace {
constexpr auto vector = Vector3{1.0, -2.0, 3.5};
static_assert(vector.x() == 1.0);
static_assert(vector.y() == -2.0);
static_assert(vector.z() == 3.5);
} // namespace

TEST(Vector3Test, PreservesComponents) {
  const Vector3 vector{1.25, -2.5, 3.75};

  EXPECT_DOUBLE_EQ(vector.x(), 1.25);
  EXPECT_DOUBLE_EQ(vector.y(), -2.5);
  EXPECT_DOUBLE_EQ(vector.z(), 3.75);
}
