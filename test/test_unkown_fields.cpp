/*
 *  Copyright (C) 2021 Embedded AMS B.V. - All Rights Reserved
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

// In this file we test what happens when we send a newer vesion of a message and deserialize 
// that data in an older object.

#include "gtest/gtest.h"

#include <WireFormatter.h>
#include <ReadBufferFixedSize.h>
#include <ReadBufferMock.h>
#include <WriteBufferMock.h>

#include <cstdint>    
#include <limits> 

// EAMS message definitions
#include <simple_types.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::DoAll;

namespace test_EmbeddedAMS_UnknownFields
{

TEST(UnknownFields, varint) 
{
  ::Test_Simple_Types msg;

  ::EmbeddedProto::ReadBufferFixedSize<19> buffer( 
                                        { 0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, // a_int32
                                          0x90, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, // Additional uint32
                                          0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F,  // a_uint32
                                        } );

  EXPECT_EQ(::EmbeddedProto::Error::NO_ERRORS, msg.deserialize(buffer));

  EXPECT_EQ(std::numeric_limits<int32_t>::max(), msg.get_a_int32()); 
  EXPECT_EQ(std::numeric_limits<uint32_t>::max(), msg.get_a_uint32());  
}

TEST(UnknownFields, fixed32) 
{
  ::Test_Simple_Types msg;

  ::EmbeddedProto::ReadBufferFixedSize<18> buffer(
                                        { 0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, // a_int32
                                          0x9D, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, // Additional fixed32
                                          0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, // a_uint32
                                        } );

  EXPECT_EQ(::EmbeddedProto::Error::NO_ERRORS, msg.deserialize(buffer));

  EXPECT_EQ(std::numeric_limits<int32_t>::max(), msg.get_a_int32()); 
  EXPECT_EQ(std::numeric_limits<uint32_t>::max(), msg.get_a_uint32());  
}

TEST(UnknownFields, fixed64) 
{
  ::Test_Simple_Types msg;

  ::EmbeddedProto::ReadBufferFixedSize<22> buffer(
                                  { 0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, // a_int32
                                    0xA1, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Additional fixed64
                                    0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, // a_uint32
                                  } );

  EXPECT_EQ(::EmbeddedProto::Error::NO_ERRORS, msg.deserialize(buffer));

  EXPECT_EQ(std::numeric_limits<int32_t>::max(), msg.get_a_int32()); 
  EXPECT_EQ(std::numeric_limits<uint32_t>::max(), msg.get_a_uint32());  
}

TEST(UnknownFields, length_delimited) 
{
  ::Test_Simple_Types msg;

  ::EmbeddedProto::ReadBufferFixedSize<20> buffer( {
                            0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, // a_int32
                            0xaa, 0x03, 0x05,  // Id and size of the unkown field.
                            0x00, 0x00, 0x00, 0x00, 0x00, // Actuall unkown field
                            0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F // a_uint32
                        } );

  EXPECT_EQ(::EmbeddedProto::Error::NO_ERRORS, msg.deserialize(buffer));

  EXPECT_EQ(std::numeric_limits<int32_t>::max(), msg.get_a_int32()); 
  EXPECT_EQ(std::numeric_limits<uint32_t>::max(), msg.get_a_uint32());  
}

} // End of namespace test_EmbeddedAMS_UnknownFields
