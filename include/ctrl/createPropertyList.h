
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

#ifndef CREATEPROPERTYLIST_H_
#define CREATEPROPERTYLIST_H_

#include <ctrl/typemanip.h>

namespace ctrl {

namespace Private {

   template <class TList_, int startLine_, int endLine_, class ConcreteClass_>
   struct CreatePropertyListImpl {
      typedef typename ConcreteClass_::template __IsPropertyPresent<endLine_>
                     IsPropertyPresent;
      enum { isPropertyPresent = IsPropertyPresent::value };

      typedef typename Select< isPropertyPresent
                             , TypeList<typename IsPropertyPresent::PropertyId, TList_>
                             , TList_ >::Result NewTList;

      typedef CreatePropertyListImpl<NewTList, startLine_, endLine_ - 1, ConcreteClass_> PropertyListImpl;
      typedef typename PropertyListImpl::TList TList;
   };


   template <class TList_, int startLine_, class ConcreteClass_>
   struct CreatePropertyListImpl<TList_, startLine_, startLine_, ConcreteClass_> {
      typedef TList_ TList;
   };


   template <int startLine_, int endLine_, class ConcreteClass_>
   struct CreatePropertyList {
      typedef CreatePropertyListImpl< NullType, startLine_
                                     , endLine_ - 1, ConcreteClass_ > PropertyListImpl;
      typedef typename PropertyListImpl::TList TList;
   };

} // namespace ctrl

} // namespace Private

#endif // CREATEPROPERTYLIST_H_