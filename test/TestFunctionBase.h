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

#ifndef _TEST_FUNCTION_BASE_H_
#define _TEST_FUNCTION_BASE_H_

 class TestFunctionBase
 {
  public:
    TestFunctionBase() = default;
    virtual ~TestFunctionBase() = default;
   
    virtual void run() = 0;

    void set_next(TestFunctionBase& next_function) { next_ = &next_function; }
  
    TestFunctionBase* get_next() { return next_; }

  private:

    TestFunctionBase* next_ = nullptr;
 };

 class TestFunctionCollection 
 {
    public:
      // Get the single instance of the class
      static TestFunctionCollection& getInstance() {
          static TestFunctionCollection instance; // Guaranteed to be created once (C++11+ thread-safe)
          return instance;
      }
  
      // Delete copy constructor and assignment operator to prevent copying
      TestFunctionCollection(const TestFunctionCollection&) = delete;
      TestFunctionCollection& operator=(const TestFunctionCollection&) = delete;
 
      // Register the next test function.
      void registerFunction(TestFunctionBase& testFunction)
      {
        if(nullptr == first_) 
        {
          first_ = &testFunction;
        }
        else
        {
          last_->set_next(testFunction);
        }
        last_ = &testFunction;
      };

      // Run all tests.
      void run() 
      {
        TestFunctionBase* current = first_;
        while(nullptr != current)
        {
          current->run();
          current = current->get_next();
        }
      }
  
    private:
      TestFunctionCollection() = default; // Private constructor
      ~TestFunctionCollection() = default;  // Private destructor

      TestFunctionBase* first_ = nullptr;
      TestFunctionBase* last_ = nullptr;
  };

  #endif // _TEST_FUNCTION_BASE_H_