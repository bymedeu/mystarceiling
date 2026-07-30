#include <gtest/gtest.h>
#include <ostream>
#include "gtest/gtest.h"
#include "mystarceiling/core/maths/declination.hh"

using mystarceiling::core::maths::Angle;
using mystarceiling::core::maths::Declination;
using mystarceiling::core::maths::DeclinationError;

namespace
{
    struct DeclinationCase
    {
        Angle value;
    };

    class ValidDeclinationTest
        : public ::testing::TestWithParam<DeclinationCase>
    {};

    void PrintTo(const DeclinationCase* tcase, std::ostream* os)
    {
        *os << "value: " << tcase->value.to_degrees() << "°";
    }
} // namespace

TEST_P(ValidDeclinationTest, TestsValidDeclination)
{
    const auto tcase = GetParam();
    const auto dec = Declination::try_from(tcase.value);
    EXPECT_TRUE(dec.has_value());
    EXPECT_EQ(dec->get_angle().to_radian(), tcase.value.to_radian());
}

// I should remember to remove this comment, but I agree with you for the
// errors, you convinced me. How does ordinary overloads work? I have the basics
// of raii from C then, but probably not from threading.
TEST_P(DeclinationTest, TestsInvalidDeclinationNonNumber)
{
    const auto tcase = GetParam();
    const auto dec = Declination::try_from(tcase.value);

    ASSERT_FALSE(dec.has_value());
    EXPECT_EQ(dec.error(), DeclinationError::NonFinite);
}

TEST_P(DeclinationTest, TestsInvalidDeclinationNonValid)
{
    const auto tcase = GetParam();
    const auto dec = Declination::try_from(tcase.value);

    ASSERT_FALSE(dec.has_value());
    EXPECT_EQ(dec.error(), DeclinationError::OutOfRange);
}
