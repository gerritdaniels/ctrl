#ifndef WEAKPTRWRAPPERIMPL_H_
#define WEAKPTRWRAPPERIMPL_H_

#include <string>
#include <ctrl/weakPtrWrapper.h>
#include <ctrl/polymorphicSerializer.h>

#include <iostream>
#include <iomanip>

namespace ctrl {

namespace Private {

   template <template <class> class Shared_, template <class> class Weak_, class Element_>
   class WeakPtrWrapperImpl : public WeakPtrWrapper<Shared_>::Impl {
   public:
      WeakPtrWrapperImpl(Weak_<Element_>& ptr) : m_ptr(ptr) { }

      virtual void assign(const Shared_<void>& ptr, const std::string& sharedName) {
         std::string weakName = Element_::__staticClassName();
         void* p = ptr.get();

         if (PolymorphicSerializer::instance().hasCast(sharedName, weakName))
            p = PolymorphicSerializer::instance().getCast(sharedName, weakName)(p);

         m_ptr = Shared_<Element_>(ptr, reinterpret_cast<Element_*>(p));
      }

   private:
      Weak_<Element_>& m_ptr;
   };


} // namespace Private

} // namespace ctrl

#endif // WEAKPTRWRAPPERIMPL_H_

/*
 * Copyright (C) 2010, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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
