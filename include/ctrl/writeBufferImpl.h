#ifndef WRITEBUFFERIMPL_H_
#define WRITEBUFFERIMPL_H_

#include <ctrl/writeBuffer.h>
#include <ctrl/integerConvertor.h>
#include <ctrl/platformFormat.h>

namespace ctrl {

namespace Private {

   template <int alignment_, int endian_>
   class WriteBufferImpl : public WriteBuffer::Impl {
   private:
      union FloatUnion { float f; int i; };
      union DoubleUnion { double d; long long l; };
   public:
      WriteBufferImpl() { }
      virtual ~WriteBufferImpl() { }

      virtual void append(const bool& val) { appendNumber(val); }
      virtual void append(const char& val) { appendNumber(val); }
      virtual void append(const short& val) { appendNumber(val); }
      virtual void append(const int& val) { appendNumber(val); }
      virtual void append(const long& val) { appendNumber(val); }
      virtual void append(const long long& val) { appendNumber(val); }
      virtual void append(const unsigned char& val) { appendNumber(val); }
      virtual void append(const unsigned short& val) { appendNumber(val); }
      virtual void append(const unsigned int& val) { appendNumber(val); }
      virtual void append(const unsigned long& val) { appendNumber(val); }
      virtual void append(const unsigned long long& val) { appendNumber(val); }
      virtual void append(const float& val) {
         FloatUnion fu;
         fu.f = val;
         appendNumber(fu.i);
      }

      virtual void append(const double& val) {
         DoubleUnion du;
         du.d = val;
         appendNumber(du.l);
      }

      virtual void append(const std::string& val) {
         std::string::size_type length = val.length();
         append(length);
         appendNoPadding(val.data(), length);
         appendNoPadding(s_padding, (alignment_ - (length % alignment_)) % alignment_);
      }

      virtual void append(const char* data, long length) {
         appendNoPadding(data, length);
         appendNoPadding(s_padding, (alignment_ - (length % alignment_)) % alignment_);
      }

   private:
      template <typename Number_>
      void appendNumber(const Number_& n) {
         Number_ out = IntegerConvertor<Number_, endian_ != CTRL_BYTE_ORDER>::convert(n);
         appendNoPadding((char*)(&out), sizeof(Number_));
         appendNoPadding(s_padding, (alignment_ - (sizeof(Number_) % alignment_)) % alignment_);
      }
   };

} // namespace Private

} // namespace ctrl

#endif // WRITEBUFFERIMPL_H_

/*
 * Copyright (C) 2010, 2012, 2013 by Gerrit Daniels <gerrit.daniels@gmail.com>
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
