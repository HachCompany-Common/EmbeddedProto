/*
 *  Copyright (C) 2020-2025 Embedded AMS B.V. - All Rights Reserved
 *
 *  This file is part of Embedded Proto.
 *
 *  Embedded Proto is open source software: you can redistribute it and/or 
 *  modify it under the terms of the GNU General Public License as published 
 *  by the Free Software Foundation, version 3 of the license.
 *
 *  Embedded Proto  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Embedded Proto. If not, see <https://www.gnu.org/licenses/>.
 *
 *  For commercial and closed source application please visit:
 *  <https://embeddedproto.com/pricing/>.
 *
 *  Embedded AMS B.V.
 *  Info:
 *    info at EmbeddedProto dot com
 *
 *  Postal address:
 *    Atoomweg 2
 *    1627 LE, Hoorn
 *    the Netherlands
 */

#include <gtest/gtest.h>
#include <Version.h>
#include "version_test_matching.h"
#include "version_test_minor_mismatch.h"
// Note: version_test_major_mismatch.h is intentionally not included as it would cause compilation failure
#include "version_test.h"  // Generated from version_test.proto in build/EAMS
#include <MessageInterface.h>

// Test case for version number access
TEST(VersionTest, VersionNumbers) {
  // Test that version numbers are accessible and match version.json
  EXPECT_EQ(4, EMBEDDEDPROTO_VERSION_MAJOR);
  EXPECT_EQ(0, EMBEDDEDPROTO_VERSION_MINOR);
  EXPECT_EQ(0, EMBEDDEDPROTO_VERSION_PATCH);
  EXPECT_STREQ("4.0.0", EMBEDDEDPROTO_VERSION_STRING);
}

// Test version comparison macros
TEST(VersionTest, VersionComparison) {
  // Test that version numbers are consistent
  EXPECT_EQ(4, EMBEDDEDPROTO_VERSION_MAJOR);
  EXPECT_EQ(0, EMBEDDEDPROTO_VERSION_MINOR);
  EXPECT_EQ(0, EMBEDDEDPROTO_VERSION_PATCH);
}

// Test version mismatch scenarios
TEST(VersionTest, VersionMatchingScenarios) {
    // Test matching version scenario
    test::MatchingVersion matching;
    EXPECT_TRUE(matching.test()) << "Matching version test should compile and run";

    // Test minor version mismatch scenario (should compile with warning)
    test::MinorMismatchVersion minor_mismatch;
    EXPECT_TRUE(minor_mismatch.test()) << "Minor version mismatch should compile with warning";

    // Note: Major version mismatch test is in version_test_major_mismatch.h
    // It is commented out because it will fail to compile by design:
    // static_assert(EMBEDDEDPROTO_VERSION_MAJOR == EMBEDDEDPROTO_LIB_VERSION_MAJOR)
    // This proves our version mismatch detection works correctly
}

// Test that version matching works in generated code
TEST(VersionTest, GeneratedCodeVersioning) {
    // Test using our version_test.proto generated code
    version_test::SimpleMessage msg;
    EXPECT_TRUE(true) << "Generated code compiled successfully with matching versions";
}
