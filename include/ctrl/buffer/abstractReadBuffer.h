#ifndef ABSTRACTREADBUFFER_H
#define ABSTRACTREADBUFFER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <ctrl/buffer/readPointerRepository.h>
#include <ctrl/buffer/readRawPointerRepository.h>
#include <ctrl/exception.h>

namespace ctrl {

namespace Private {

   class AbstractReadBuffer
   {
   public:
      virtual ~AbstractReadBuffer();

      ReadPointerRepository<std::shared_ptr, std::weak_ptr>& getStdPointerRepository();
      ReadPointerRepository<boost::shared_ptr, boost::weak_ptr>& getBoostPointerRepository();
      ReadRawPointerRepository& getRawPointerRepository();

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

      virtual void readVersion(int& version) throw(Exception) = 0;
      virtual void readBits(char* data, long length) throw(Exception) = 0;
      virtual void readCollectionSize(std::size_t& size) throw(Exception) = 0;
      virtual void readPointerId(int& id) throw(Exception) = 0;
      virtual void readTypeId(std::string& val) throw(Exception) = 0;

      virtual void read(bool& val) throw(Exception) = 0;
      virtual void read(char& val) throw(Exception) = 0;
      virtual void read(short& val) throw(Exception) = 0;
      virtual void read(int& val) throw(Exception) = 0;
      virtual void read(long& val) throw(Exception) = 0;
      virtual void read(long long& val) throw(Exception) = 0;
      virtual void read(unsigned char& val) throw(Exception) = 0;
      virtual void read(unsigned short& val) throw(Exception) = 0;
      virtual void read(unsigned int& val) throw(Exception) = 0;
      virtual void read(unsigned long& val) throw(Exception) = 0;
      virtual void read(unsigned long long& val) throw(Exception) = 0;
      virtual void read(float& val) throw(Exception) = 0;
      virtual void read(double& val) throw(Exception) = 0;
      virtual void read(std::string& val) throw(Exception) = 0;

   protected:
      AbstractReadBuffer();

   private:
      AbstractReadBuffer(const AbstractReadBuffer&);

      ReadPointerRepository<std::shared_ptr, std::weak_ptr> m_stdPointerRepository;
      ReadPointerRepository<boost::shared_ptr, boost::weak_ptr> m_boostPointerRepository;
      ReadRawPointerRepository m_rawPointerRepository;
   };

} // namespace Private

} // namespace ctrl

#endif // ABSTRACTREADBUFFER_H

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
