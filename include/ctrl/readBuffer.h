#ifndef READBUFFER_H_
#define READBUFFER_H_

#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <ctrl/readPointerRepository.h>
#include <ctrl/readRawPointerRepository.h>
#include <ctrl/exception.h>

namespace ctrl {

namespace Private {

   class ReadBuffer {
   public:
      class Impl {
      public:
         Impl();
         virtual ~Impl();

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
         virtual void read(char* data, long length) throw(Exception) = 0;
         virtual bool reachedEnd() const = 0;
      }; // class Impl

      ReadBuffer(Impl* pimpl);

      template <class T>
      void read(T& val) throw(Exception) {
         m_pimpl->read(val);
      }

      void read(char* data, long length) throw(Exception);
      bool reachedEnd() const;

      ReadPointerRepository<std::shared_ptr, std::weak_ptr>& getStdPointerRepository();
      ReadPointerRepository<boost::shared_ptr, boost::weak_ptr>& getBoostPointerRepository();
      ReadRawPointerRepository& getRawPointerRepository();

   private:
      ReadBuffer(const ReadBuffer&);

      std::unique_ptr<Impl> m_pimpl;
      ReadPointerRepository<std::shared_ptr, std::weak_ptr> m_stdPointerRepository;
      ReadPointerRepository<boost::shared_ptr, boost::weak_ptr> m_boostPointerRepository;
      ReadRawPointerRepository m_rawPointerRepository;

   };

} // namespace Private

} // namespace ctrl

#endif // READBUFFER_H_

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
