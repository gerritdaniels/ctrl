
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

#ifndef DERIVATIONROOTS_H_
#define DERIVATIONROOTS_H_

#include <ctrl/typemanip.h>

namespace ctrl {

namespace Private {

   template <class BaseClasses_>
   struct GatherBaseClasses {
      typedef NullType Result;
   };

   template <class Next_, class Previous_>
   struct GatherBaseClassesContinue {
      typedef typename GatherBaseClasses<Next_>::Result Result;
   };

   template <class T_>
   struct GatherBaseClassesContinue<T_, T_> {
      typedef T_ Result;
   };

   template <class Head_, class Tail_>
   struct GatherBaseClasses<TypeList<Head_, Tail_>> {
      typedef typename Head_::__BaseClasses LevelUp;

      typedef typename Select<IsNullType<LevelUp>::value, Head_, LevelUp>::Result NewHead;

      typedef typename GatherBaseClasses<Tail_>::Result NewTail;

      typedef TypeList<NewHead, NewTail> Next;

      typedef typename GatherBaseClassesContinue<Next, TypeList<Head_, Tail_>>::Result Result;
   };


   template <class Head_, class Tail_, class TList_>
   struct GatherBaseClasses<TypeList<TypeList<Head_, Tail_>, TList_>> {

      typedef typename GatherBaseClasses<TypeList<Head_, Tail_>>::Result NewHead;
      typedef typename GatherBaseClasses<TList_>::Result NewTail;

      typedef TypeList<NewHead, NewTail> Next;

      typedef typename GatherBaseClassesContinue<Next, TypeList<TypeList<Head_, Tail_>, TList_>>::Result Result;
   };

   template <class ConcreteClass_>
   class DerivationRoots {
      typedef typename GatherBaseClasses<TypeList<ConcreteClass_, NullType>>::Result BaseClasses;
      typedef typename SimplifyTypeList<BaseClasses>::Result Simplified;

   public:
      typedef typename UniquifyTypeList<Simplified>::Result Roots;
      enum { isSingleRoot = !IsTypeList<Roots>::value };
   };

} // namespace Private

} // namespace ctrl

#endif // DERIVATIONROOTS_H_
