
/*
 * Copyright (C) 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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

#ifndef ABSTRACTWRITEBUFFER_H
#define ABSTRACTWRITEBUFFER_H

#include <string>
#include <ctrl/exception.h>
#include <ctrl/buffer/writePointerRepository.h>

namespace ctrl {

   class Context;

   class AbstractWriteBuffer
   {
   public:
      virtual ~AbstractWriteBuffer();

      Private::WritePointerRepository& getPointerRepository();

      virtual void enterObject(const Context& context) throw(Exception) = 0;
      virtual void enterMember(const Context& context, const char* suggested = 0) throw(Exception) = 0;
      virtual void leaveMember(const Context& context) throw(Exception) = 0;
      virtual void leaveObject(const Context& context) throw(Exception) = 0;

      virtual void enterIdField(const Context& context) throw(Exception) = 0;
      virtual void appendNullId(const Context& context) throw(Exception) = 0;
      virtual void appendNonNullId(const Context& context) throw(Exception) = 0;
      virtual void leaveIdField(const Context& context) throw(Exception) = 0;

      virtual void enterCollection(const Context& context) throw(Exception) = 0;
      virtual void nextCollectionElement(const Context& context) throw(Exception) = 0;
      virtual void leaveCollection(const Context& context) throw(Exception) = 0;

      virtual void enterMap(const Context& context) throw(Exception) = 0;
      virtual void enterKey(const Context& context) throw(Exception) = 0;
      virtual void leaveKey(const Context& context) throw(Exception) = 0;
      virtual void enterValue(const Context& context) throw(Exception) = 0;
      virtual void leaveValue(const Context& context) throw(Exception) = 0;
      virtual void leaveMap(const Context& context) throw(Exception) = 0;

      virtual void appendVersion(const int& version, const Context& context) throw(Exception) = 0;
      virtual void appendBits(const char* data, long length, const Context& context) throw(Exception) = 0;
      virtual void appendCollectionSize(const std::size_t& size, const Context& context) throw(Exception) = 0;
      virtual void appendTypeId(const std::string& val, const Context& context) throw(Exception) = 0;

      virtual void append(const bool& val, const Context& context) throw(Exception) = 0;
      virtual void append(const char& val, const Context& context) throw(Exception) = 0;
      virtual void append(const short& val, const Context& context) throw(Exception) = 0;
      virtual void append(const int& val, const Context& context) throw(Exception) = 0;
      virtual void append(const long& val, const Context& context) throw(Exception) = 0;
      virtual void append(const long long& val, const Context& context) throw(Exception) = 0;
      virtual void append(const unsigned char& val, const Context& context) throw(Exception) = 0;
      virtual void append(const unsigned short& val, const Context& context) throw(Exception) = 0;
      virtual void append(const unsigned int& val, const Context& context) throw(Exception) = 0;
      virtual void append(const unsigned long& val, const Context& context) throw(Exception) = 0;
      virtual void append(const unsigned long long& val, const Context& context) throw(Exception) = 0;
      virtual void append(const float& val, const Context& context) throw(Exception) = 0;
      virtual void append(const double& val, const Context& context) throw(Exception) = 0;
      virtual void append(const std::string& val, const Context& context) throw(Exception) = 0;

   protected:
      AbstractWriteBuffer();

   private:
      Private::WritePointerRepository m_pointerRepository;
   };

} // namespace ctrl

#endif // ABSTRACTWRITEBUFFER_H
