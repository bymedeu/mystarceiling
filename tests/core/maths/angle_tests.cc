#include <gtest/gtest.h>

#include <mystarceiling/core/maths/angle.hh>
#include <numbers>
#include <ostream>

using msc::core::maths::Angle;

namespace {
constexpr auto angle = Angle::from_radian(1.25);
static_assert(angle.to_radian() == 1.25);

struct AllTypesCase {
  double degree;
  double hours;
  double radian;
};

class AllTypesTest : public ::testing::TestWithParam<AllTypesCase> {};
void PrintTo(const struct AllTypesCase &tcase, std::ostream *os) {
  *os << "degree: " << tcase.degree << ", hours: " << tcase.hours
      << ", rad: " << tcase.radian;
}

} // namespace

void test_hours(const Angle &angle, double exp) {
  EXPECT_NEAR(angle.to_hours(), exp, 1e-12)
      << "exp: " << exp << ", got: " << angle.to_hours();
}

void test_rad(const Angle &angle, double exp) {
  EXPECT_NEAR(angle.to_radian(), exp, 1e-12)
      << "exp: " << exp << ", got: " << angle.to_radian();
}
void test_degrees(const Angle &angle, double exp) {
  EXPECT_NEAR(angle.to_degrees(), exp, 1e-12)
      << "exp: " << exp << ", got: " << angle.to_degrees();
}

TEST_P(AllTypesTest, ConvertsWithAllTypes) {
  const auto &param = GetParam();
  auto degree = Angle::from_degrees(param.degree);
  auto hours = Angle::from_hours(param.hours);
  auto radian = Angle::from_radian(param.radian);

  test_degrees(hours, param.degree);
  test_hours(hours, param.hours);
  test_rad(hours, param.radian);
  test_degrees(radian, param.degree);
  test_hours(radian, param.hours);
  test_rad(radian, param.radian);
  test_degrees(degree, param.degree);
  test_hours(degree, param.hours);
  test_rad(degree, param.radian);
}

// deg hours rad
INSTANTIATE_TEST_SUITE_P(
    ConvertsWithAllTypes, AllTypesTest,
    ::testing::Values(AllTypesCase{0, 0, 0},
                      AllTypesCase{180, 12, std::numbers::pi},
                      AllTypesCase{720, 48, std::numbers::pi * 4},
                      AllTypesCase{-450, -30, -std::numbers::pi * 2.5},
                      AllTypesCase{360, 24, std::numbers::pi * 2},
                      AllTypesCase{-90, -6, -std::numbers::pi / 2},
                      AllTypesCase{45, 3, std::numbers::pi / 4},
                      AllTypesCase{12.5, 5.0 / 6,
                                   12.5 * std::numbers::pi / 180},
                      AllTypesCase{90, 6, std::numbers::pi / 2}));
