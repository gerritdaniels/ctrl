
/*
 * Copyright (C) 2010, 2012, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BINARYWRITEBUFFER_H_
#define BINARYWRITEBUFFER_H_

#include <string>
#include <memory>
#include <ctrl/buffer/abstractWriteBuffer.h>

namespace ctrl {

namespace Private {

   class BinaryWriteBuffer : public ctrl::AbstractWriteBuffer {
   public:
      class Impl {
      public:
         Impl();
         virtual ~Impl();

         long length();
         char* getData();

         virtual void append(const bool& val) = 0;
         virtual void append(const char& val) = 0;
         virtual void append(const short& val) = 0;
         virtual void append(const int& val) = 0;
         virtual void append(const long& val) = 0;
         virtual void append(const long long& val) = 0;
         virtual void append(const unsigned char& val) = 0;
         virtual void append(const unsigned short& val) = 0;
         virtual void append(const unsigned int& val) = 0;
         virtual void append(const unsigned long& val) = 0;
         virtual void append(const unsigned long long& val) = 0;
         virtual void append(const float& val) = 0;
         virtual void append(const double& val) = 0;
         virtual void append(const std::string& val) = 0;
         virtual void append(const char* data, long length) = 0;

      protected:
         static const char* s_padding;
         void appendNoPadding(const char* data, long length);
         void realloc(long newLength);

      private:
         char* m_data;
         long m_capacity;
         long m_length;
         bool m_owner;
      }; // class Impl

      BinaryWriteBuffer(Impl* pimpl);

      virtual void enterObject(const Context& context) throw(Exception);
      virtual void enterMember(const Context& context, const char* suggested = 0) throw(Exception);
      virtual void leaveMember(const Context& context) throw(Exception);
      virtual void leaveObject(const Context& context) throw(Exception);

      virtual void enterIdField(const Context& context) throw(Exception);
      virtual void appendNullId(const Context& context) throw(Exception);
      virtual void appendNonNullId(const Context& context) throw(Exception);
      virtual void leaveIdField(const Context& context) throw(Exception);

      virtual void enterCollection(const Context& context) throw(Exception);
      virtual void nextCollectionElement(const Context& context) throw(Exception);
      virtual void leaveCollection(const Context& context) throw(Exception);

      virtual void enterMap(const Context& context) throw(Exception);
      virtual void enterKey(const Context& context) throw(Exception);
      virtual void leaveKey(const Context& context) throw(Exception);
      virtual void enterValue(const Context& context) throw(Exception);
      virtual void leaveValue(const Context& context) throw(Exception);
      virtual void leaveMap(const Context& context) throw(Exception);

      virtual void appendVersion(const int& version, const Context& context) throw(Exception);
      virtual void appendBits(const char* data, long length, const Context& context) throw(Exception);
      virtual void appendCollectionSize(const std::size_t& size, const Context& context) throw(Exception);
      virtual void appendTypeId(const std::string& val, const Context& context) throw(Exception);

      virtual void append(const bool& val, const Context& context) throw(Exception);
      virtual void append(const char& val, const Context& context) throw(Exception);
      virtual void append(const short& val, const Context& context) throw(Exception);
      virtual void append(const int& val, const Context& context) throw(Exception);
      virtual void append(const long& val, const Context& context) throw(Exception);
      virtual void append(const long long& val, const Context& context) throw(Exception);
      virtual void append(const unsigned char& val, const Context& context) throw(Exception);
      virtual void append(const unsigned short& val, const Context& context) throw(Exception);
      virtual void append(const unsigned int& val, const Context& context) throw(Exception);
      virtual void append(const unsigned long& val, const Context& context) throw(Exception);
      virtual void append(const unsigned long long& val, const Context& context) throw(Exception);
      virtual void append(const float& val, const Context& context) throw(Exception);
      virtual void append(const double& val, const Context& context) throw(Exception);
      virtual void append(const std::string& val, const Context& context) throw(Exception);

      long length();
      char* getData();

   private:
      std::unique_ptr<Impl> m_pimpl;
      bool m_skipNextFundamental;
   };

} // namespace Private

} // namespace ctrl

#endif // BINARYWRITEBUFFER_H_
