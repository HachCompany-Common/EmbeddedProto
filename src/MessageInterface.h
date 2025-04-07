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

#ifndef _MESSAGE_INTERFACE_H_
#define _MESSAGE_INTERFACE_H_

#include "WireFormatter.h"
#include "Fields.h"
#include "Errors.h"

#include <cstdint>


namespace EmbeddedProto 
{

class MessageInterface : public ::EmbeddedProto::Field
{
  public:

    MessageInterface() = default;

    ~MessageInterface() override = default;

    //! \see Field::serialize_with_id()
    Error serialize_with_id(uint32_t field_number, 
                            ::EmbeddedProto::WriteBufferInterface& buffer,
                            const bool optional) const final;

    //! \see Field::deserialize()
    Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override = 0;

    //! \see Field::deserialize()
    Error deserialize_check_type(::EmbeddedProto::ReadBufferInterface& buffer, 
                                 const ::EmbeddedProto::WireFormatter::WireType& wire_type) final;

    //! Clear the content of this message and set it to it's default state.
    /*!
        The defaults are to be set according to the Protobuf standard.
    */
    void clear() override = 0;
    
    //! Function for testing purposes.
    void set_max_value() override = 0;

  protected:

    class FieldReference
    {
      public:
        FieldReference(Field& f, uint32_t fn) : field(f), field_number(fn), present(false) {}

        Field& field;
        uint32_t field_number;
        bool present;

      private:
        FieldReference() = default;
    };

    /*!
      \param[in/out] deserialize_id_number the deserialize state of the derived class.
    */
    Error deserialize_field_list(::EmbeddedProto::ReadBufferInterface& buffer, const auto& begin, const auto& end, uint32_t& deserialize_id_number)
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      uint32_t id_tag = 0; // FieldNumber::NOT_SET;
      ::EmbeddedProto::WireFormatter::WireType deserialize_wire_type_;


      // If we did not already deserialize a tag, obtain the next one. When we did remember a tag 
      // have been deserializing a field which was not yet fully received in the read buffer.
      if(0 == deserialize_id_number)
      {
        return_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, deserialize_wire_type_, deserialize_id_number);
      }
 
      bool continue_deserializing = ::EmbeddedProto::Error::NO_ERRORS == return_value;
      while(continue_deserializing)
      {
        auto field_it = begin;
        while(end != field_it) 
        { 
          if(deserialize_id_number == field_it->field_number)
          {
            break; // stop the loop.
          }
          else 
          {
            ++field_it;
          }
        }
        
        if(end != field_it)
        {
          return_value = field_it->field.deserialize_check_type(buffer, deserialize_wire_type_);
        }
        else
        {
          return_value = skip_unknown_field(buffer, deserialize_wire_type_);
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag but first reset the state such that in the case of an error reading 
          // the tag we will try again on the next call of this function.
          deserialize_id_number = 0;
          return_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, deserialize_wire_type_, deserialize_id_number);
        
          if(::EmbeddedProto::Error::NO_ERRORS != return_value)
          {
            // If we reached the end of the buffer here assume the message is complete.
            if(::EmbeddedProto::Error::END_OF_BUFFER == return_value)
            {
              return_value = ::EmbeddedProto::Error::NO_ERRORS;
            }
            continue_deserializing = false;
          }
        }
        else
        {
          continue_deserializing = false;
        }        
      }

      return return_value;
    };

    //! When deserializing skip the bytes in the buffer of an unknown field.
    /*! 
        This function is used when a field with an unknown id is encountered to move through the 
        buffer to the next tag.
    */
    Error skip_unknown_field(::EmbeddedProto::ReadBufferInterface& buffer, 
                             const ::EmbeddedProto::WireFormatter::WireType& wire_type) const;

    Error skip_varint(::EmbeddedProto::ReadBufferInterface& buffer) const;
    Error skip_fixed32(::EmbeddedProto::ReadBufferInterface& buffer) const;
    Error skip_fixed64(::EmbeddedProto::ReadBufferInterface& buffer) const;
    Error skip_length_delimited(::EmbeddedProto::ReadBufferInterface& buffer) const;


    uint32_t n_bytes_to_include_in_section_ = 0;
};

} // End of namespace EmbeddedProto

#endif // _MESSAGE_INTERFACE_H_
