#ifndef POLYMORPHICFACTORY_H_
#define POLYMORPHICFACTORY_H_

#include <boost/shared_ptr.hpp>

namespace ctrl {

namespace Private {

   class ReadBuffer;

   class PolymorphicFactory {
   public:
      class Impl {
      public:
         Impl();
         virtual ~Impl();
         virtual void* deserialize(ReadBuffer& buffer, int version) const = 0;
      };

      PolymorphicFactory();
      PolymorphicFactory(boost::shared_ptr<Impl> pimpl);
      PolymorphicFactory(const PolymorphicFactory& that);

      PolymorphicFactory& operator=(const PolymorphicFactory& that);

      void* deserialize(ReadBuffer& buffer, int version) const;

   private:
      boost::shared_ptr<Impl> m_pimpl;
   };

} // namespace Private

} // namespace ctrl

#endif // POLYMORPHICFACTORY_H_

/*
 * Copyright (C) 2012, 2013 by Gerrit Daniels <gerrit.daniels@gmail.com>
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