#ifndef WRITEBUFFER_H_
#define WRITEBUFFER_H_

#include <string>
#include <memory>
#include <ctrl/writePointerRepository.h>

namespace ctrl {

namespace Private {

   class WriteBuffer {
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

      WriteBuffer(Impl* pimpl);

      long length();
      char* getData();

      template <class T>
      void append(const T& val) {
         m_pimpl->append(val);
      }

      void append(const char* data, long length);

      WritePointerRepository& getPointerRepository();

   private:
      std::unique_ptr<Impl> m_pimpl;
      WritePointerRepository m_pointerRepository;
   };

} // namespace Private

} // namespace ctrl

#endif // WRITEBUFFER_H_

/*
 * Copyright (C) 2010, 2012 by Gerrit Daniels <gerrit.daniels@gmail.com>
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
