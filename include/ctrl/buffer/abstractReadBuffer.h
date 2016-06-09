
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

#ifndef ABSTRACTREADBUFFER_H
#define ABSTRACTREADBUFFER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <ctrl/buffer/readPointerRepository.h>
#include <ctrl/buffer/readRawPointerRepository.h>
#include <ctrl/exception.h>

namespace ctrl {

   class Context;

   class AbstractReadBuffer
   {
   public:
      virtual ~AbstractReadBuffer();

      Private::ReadPointerRepository<std::shared_ptr, std::weak_ptr>& getStdPointerRepository();
      Private::ReadPointerRepository<boost::shared_ptr, boost::weak_ptr>& getBoostPointerRepository();
      Private::ReadRawPointerRepository& getRawPointerRepository();

      virtual void enterObject(const Context& context) throw(Exception) = 0;
      virtual void enterMember(const Context& context, const char* suggested = 0) throw(Exception) = 0;
      virtual void leaveMember(const Context& context) throw(Exception) = 0;
      virtual void leaveObject(const Context& context) throw(Exception) = 0;

      virtual void enterIdField(const Context& context) throw(Exception) = 0;
      virtual bool isNullId(const Context& context) throw(Exception) = 0;
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

      virtual void readVersion(int& version, const Context& context) throw(Exception) = 0;
      virtual void readBits(char* data, long length, const Context& context) throw(Exception) = 0;
      virtual void readCollectionSize(std::size_t& size, const Context& context) throw(Exception) = 0;
      virtual void readTypeId(std::string& val, const Context& context) throw(Exception) = 0;

      virtual void read(bool& val, const Context& context) throw(Exception) = 0;
      virtual void read(char& val, const Context& context) throw(Exception) = 0;
      virtual void read(short& val, const Context& context) throw(Exception) = 0;
      virtual void read(int& val, const Context& context) throw(Exception) = 0;
      virtual void read(long& val, const Context& context) throw(Exception) = 0;
      virtual void read(long long& val, const Context& context) throw(Exception) = 0;
      virtual void read(unsigned char& val, const Context& context) throw(Exception) = 0;
      virtual void read(unsigned short& val, const Context& context) throw(Exception) = 0;
      virtual void read(unsigned int& val, const Context& context) throw(Exception) = 0;
      virtual void read(unsigned long& val, const Context& context) throw(Exception) = 0;
      virtual void read(unsigned long long& val, const Context& context) throw(Exception) = 0;
      virtual void read(float& val, const Context& context) throw(Exception) = 0;
      virtual void read(double& val, const Context& context) throw(Exception) = 0;
      virtual void read(std::string& val, const Context& context) throw(Exception) = 0;

   protected:
      AbstractReadBuffer();

   private:
      AbstractReadBuffer(const AbstractReadBuffer&);

      Private::ReadPointerRepository<std::shared_ptr, std::weak_ptr> m_stdPointerRepository;
      Private::ReadPointerRepository<boost::shared_ptr, boost::weak_ptr> m_boostPointerRepository;
      Private::ReadRawPointerRepository m_rawPointerRepository;
   };

} // namespace ctrl

#endif // ABSTRACTREADBUFFER_H
