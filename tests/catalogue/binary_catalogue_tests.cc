#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <mystarceiling/catalogue/binary_catalogue.hh>
#include <mystarceiling/catalogue/star.hh>
#include <mystarceiling/catalogue/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <vector>

using msc::catalogue::ApparentMagnitude;
using msc::catalogue::ColourIndex;
using msc::catalogue::Parallax;
using msc::catalogue::ProperMotion;
using msc::catalogue::read_binary_catalogue;
using msc::catalogue::ReferenceEpoch;
using msc::catalogue::Star;
using msc::catalogue::write_binary_catalogue;
using msc::core::coordinates::Declination;
using msc::core::coordinates::IcrsDirection;
using msc::core::coordinates::RightAscension;
using msc::core::maths::Angle;

TEST(BinaryCatalogueTest, PreservesAllSelectedValues) {
  const auto epoch = ReferenceEpoch::try_from_julian_year(2016.0);
  const auto ra = RightAscension::try_from(Angle::from_degrees(10.5));
  const auto dec = Declination::try_from(Angle::from_degrees(-20.25));
  const auto pmra = ProperMotion::try_from_milliarcseconds_per_year(3.5);
  const auto pmdec = ProperMotion::try_from_milliarcseconds_per_year(-4.5);
  const auto parallax = Parallax::try_from_milliarcseconds(-0.25);
  const auto magnitude = ApparentMagnitude::try_from_gaia_g(5.75);
  const auto colour = ColourIndex::try_from_gaia_bp_minus_rp(1.25);
  ASSERT_TRUE(epoch && ra && dec && pmra && pmdec && parallax && magnitude &&
              colour);
  const std::vector stars{Star{42, *epoch, IcrsDirection{*ra, *dec}, *pmra,
                               *pmdec, *parallax, *magnitude, *colour}};
  const auto filename = std::filesystem::temp_directory_path() /
                        "mystarceiling-catalogue-test.bin";

  ASSERT_TRUE(write_binary_catalogue(filename, stars).has_value());
  const auto loaded = read_binary_catalogue(filename);
  std::filesystem::remove(filename);

  ASSERT_TRUE(loaded.has_value()) << loaded.error().detail;
  ASSERT_EQ(loaded->size(), 1);
  EXPECT_EQ(loaded->front().source_id(), 42);
  EXPECT_DOUBLE_EQ(loaded->front().reference_epoch().julian_year(), 2016.0);
  EXPECT_DOUBLE_EQ(loaded->front()
                       .direction()
                       .get_right_ascension()
                       .get_angle()
                       .to_degrees(),
                   10.5);
  EXPECT_DOUBLE_EQ(
      loaded->front().direction().get_declination().get_angle().to_degrees(),
      -20.25);
  EXPECT_DOUBLE_EQ(
      loaded->front().right_ascension_motion().milliarcseconds_per_year(), 3.5);
  EXPECT_DOUBLE_EQ(
      loaded->front().declination_motion().milliarcseconds_per_year(), -4.5);
  EXPECT_DOUBLE_EQ(loaded->front().parallax().milliarcseconds(), -0.25);
  EXPECT_DOUBLE_EQ(loaded->front().magnitude().gaia_g(), 5.75);
  EXPECT_DOUBLE_EQ(loaded->front().colour().gaia_bp_minus_rp(), 1.25);
}
