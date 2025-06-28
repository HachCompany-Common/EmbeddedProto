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

#ifndef _TEST_VERSION_TEST_MINOR_MISMATCH_H_
#define _TEST_VERSION_TEST_MINOR_MISMATCH_H_

#include <Version.h>

// This simulates a generated message file with a minor version mismatch
#define TEST_GENERATOR_VERSION_MAJOR 4  // Same major version
#define TEST_GENERATOR_VERSION_MINOR 1  // Different minor version
#define TEST_GENERATOR_VERSION_PATCH 0

static_assert(TEST_GENERATOR_VERSION_MAJOR == EMBEDDEDPROTO_VERSION_MAJOR,
    "Major version mismatch between generated code and library");

#if TEST_GENERATOR_VERSION_MINOR != EMBEDDEDPROTO_VERSION_MINOR
#warning "Minor version mismatch between generated code and library"
#endif

namespace test {
    // Empty test class to verify compilation
    class MinorMismatchVersion {
    public:
        bool test() { return true; }
    };
}

#endif // _TEST_VERSION_TEST_MINOR_MISMATCH_H_
