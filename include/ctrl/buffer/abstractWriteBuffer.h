#ifndef ABSTRACTWRITEBUFFER_H
#define ABSTRACTWRITEBUFFER_H

#include <string>
#include <ctrl/exception.h>
#include <ctrl/buffer/writePointerRepository.h>

namespace ctrl {

namespace Private {

   class AbstractWriteBuffer
   {
   public:
      virtual ~AbstractWriteBuffer();

      WritePointerRepository& getPointerRepository();

      virtual void enterObject() throw(Exception) = 0;
      virtual void enterMember(const char* name) throw(Exception) = 0;
      virtual void leaveMember() throw(Exception) = 0;
      virtual void leaveObject() throw(Exception) = 0;

      virtual void enterCollection() throw(Exception) = 0;
      virtual void nextCollectionElement() throw(Exception) = 0;
      virtual void leaveCollection() throw(Exception) = 0;

      virtual void enterMap() throw(Exception) = 0;
      virtual void enterKey() throw(Exception) = 0;
      virtual void leaveKey() throw(Exception) = 0;
      virtual void enterValue() throw(Exception) = 0;
      virtual void leaveValue() throw(Exception) = 0;
      virtual void leaveMap() throw(Exception) = 0;

      virtual void appendVersion(const int& version) throw(Exception) = 0;
      virtual void appendBits(const char* data, long length) throw(Exception) = 0;
      virtual void appendCollectionSize(const std::size_t& size) throw(Exception) = 0;
      virtual void appendPointerId(const int& id) throw(Exception) = 0;
      virtual void appendTypeId(const char* val) throw(Exception) = 0;

      virtual void append(const bool& val) throw(Exception) = 0;
      virtual void append(const char& val) throw(Exception) = 0;
      virtual void append(const short& val) throw(Exception) = 0;
      virtual void append(const int& val) throw(Exception) = 0;
      virtual void append(const long& val) throw(Exception) = 0;
      virtual void append(const long long& val) throw(Exception) = 0;
      virtual void append(const unsigned char& val) throw(Exception) = 0;
      virtual void append(const unsigned short& val) throw(Exception) = 0;
      virtual void append(const unsigned int& val) throw(Exception) = 0;
      virtual void append(const unsigned long& val) throw(Exception) = 0;
      virtual void append(const unsigned long long& val) throw(Exception) = 0;
      virtual void append(const float& val) throw(Exception) = 0;
      virtual void append(const double& val) throw(Exception) = 0;
      virtual void append(const std::string& val) throw(Exception) = 0;

   protected:
      AbstractWriteBuffer();

   private:
      WritePointerRepository m_pointerRepository;
   };

} // namespace Private

} // namespace ctrl

#endif // ABSTRACTWRITEBUFFER_H

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
