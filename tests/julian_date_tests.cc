#include <gtest/gtest.h>

#include <mystarceiling/core/time/julian_date.hh>

using msc::core::time::JulianDate;

namespace
{
    constexpr JulianDate j2000{2451545.0, 0.0};
    static_assert(j2000.part1() == 2451545.0);
    static_assert(j2000.part2() == 0.0);
} // namespace

TEST(JulianDateTest, PreservesBothPartsWithoutNormalizingTheSplit)
{
    constexpr JulianDate date{2400000.5, 54388.25};

    EXPECT_DOUBLE_EQ(date.part1(), 2400000.5);
    EXPECT_DOUBLE_EQ(date.part2(), 54388.25);
}

TEST(JulianDateTest, PreservesCallerSelectedPartOrder)
{
    constexpr JulianDate date{54388.25, 2400000.5};

    EXPECT_DOUBLE_EQ(date.part1(), 54388.25);
    EXPECT_DOUBLE_EQ(date.part2(), 2400000.5);
}
