
/*
 * Copyright (C) 2012, 2013, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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

#ifndef POLYMORPHICFACTORYIMPL_H_
#define POLYMORPHICFACTORYIMPL_H_

#include <ctrl/polymorphicFactory.h>
#include <ctrl/forwardDeserialize.h>
#include <ctrl/idField.h>

namespace ctrl {

namespace Private {

   template <class ConcreteClass_>
   class PolymorphicFactoryImpl : public PolymorphicFactory::Impl {
   public:
      virtual void* deserialize(AbstractReadBuffer& buffer, const IdField& idField, const std::string& className,
                                int version, const Context& context) const {
         ConcreteClass_* ptr;
         try {
            ptr = new ConcreteClass_();
            idField.assign(reinterpret_cast<void*>(ptr), className);
            ctrl::Private::deserialize(*ptr, buffer, version, Context(context, ptr->CTRL_dynamicContext()));
            return reinterpret_cast<void*>(ptr);
         }
         catch(...) { delete ptr; throw; }
      }
   };

} // namespace Private

} // namespace ctrl

#endif // POLYMORPHICFACTORYIMPL_H_
