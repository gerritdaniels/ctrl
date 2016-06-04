#ifndef READBUFFERIMPL_H_
#define READBUFFERIMPL_H_

#include <ctrl/readBuffer.h>
#include <ctrl/integerConvertor.h>
#include <ctrl/platformFormat.h>
#include <ctrl/exception.h>

namespace ctrl {

namespace Private {

   template <int alignment_, int endian_>
   class ReadBufferImpl : public ReadBuffer::Impl {
   private:
      union FloatUnion { float f; int i; };
      union DoubleUnion { double d; long long l; };
   public:
      ReadBufferImpl(const char* data, const long& length)
         : m_data(data)
         , m_dataEnd(data + length) {

      }

      virtual ~ReadBufferImpl() { }

      virtual void read(bool& val) throw(Exception) { readNumber(val); }
      virtual void read(char& val) throw(Exception) { readNumber(val); }
      virtual void read(short& val) throw(Exception) { readNumber(val); }
      virtual void read(int& val) throw(Exception) { readNumber(val); }
      virtual void read(long& val) throw(Exception) { readNumber(val); }
      virtual void read(long long& val) throw(Exception) { readNumber(val); }
      virtual void read(unsigned char& val) throw(Exception) { readNumber(val); }
      virtual void read(unsigned short& val) throw(Exception) { readNumber(val); }
      virtual void read(unsigned int& val) throw(Exception) { readNumber(val); }
      virtual void read(unsigned long& val) throw(Exception) { readNumber(val); }
      virtual void read(unsigned long long& val) throw(Exception) { readNumber(val); }

      virtual void read(float& val) throw(Exception) {
         FloatUnion fu;
         readNumber(fu.i);
         val = fu.f;
      }

      virtual void read(double& val) throw(Exception) {
         DoubleUnion du;
         readNumber(du.l);
         val = du.d;
      }

      virtual void read(std::string& val) throw(Exception) {
         std::string::size_type length;
         readNumber(length);
         std::string::size_type advance = length + (alignment_ - length % alignment_) % alignment_;
         if (m_data + advance > m_dataEnd)
            throw Exception("Input data is corrupt");
         val = std::string(m_data, length);
         m_data += advance;
      }

      virtual void read(char* data, long length) throw(Exception) {
         long advance = length + (alignment_ - length % alignment_) % alignment_;
         if (m_data + advance > m_dataEnd)
            throw Exception("Input data is corrupt");
         for (long i = 0; i < length; ++i)
            *(data + i) = *(m_data + i);
         m_data += advance;
      }

      virtual bool reachedEnd() const {
         return m_data == m_dataEnd;
      }

   private:
      template <typename Number_>
      void readNumber(Number_& n) throw(Exception) {
         if (m_data + sizeof(Number_) + (alignment_ - sizeof(Number_) % alignment_) % alignment_ > m_dataEnd)
            throw Exception("Input data is corrupt");
         Number_ in = *(Number_*)(m_data);
         n = IntegerConvertor<Number_, endian_ != CTRL_BYTE_ORDER>::convert(in);
         m_data += sizeof(Number_) + (alignment_ - sizeof(Number_) % alignment_) % alignment_;
      }

      const char* m_data;
      const char* m_dataEnd;
   };

} // namespace Private

} // namespace ctrl

#endif // READBUFFERIMPL_H_

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
