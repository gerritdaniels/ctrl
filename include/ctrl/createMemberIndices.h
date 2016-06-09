
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

#ifndef CREATEMEMBERINDICES_H_
#define CREATEMEMBERINDICES_H_

#include <ctrl/typemanip.h>

namespace ctrl {

namespace Private {

   template <class TList_, int startLine_, int endLine_, class ConcreteClass_>
   struct CreateMemberIndicesImpl {
      typedef typename ConcreteClass_::template __IsMemberPresent<endLine_> IsMemberPresent;
      enum { isMemberPresent = IsMemberPresent::value };

      typedef typename Select< isMemberPresent
                             , TypeList<Int2Type<IsMemberPresent::index>, TList_>
                             , TList_ >::Result NewTList;

      typedef CreateMemberIndicesImpl<NewTList, startLine_, endLine_ - 1, ConcreteClass_> MemberIndicesImpl;
      typedef typename MemberIndicesImpl::Indices Indices;
   };


   template <class TList_, int startLine_, class ConcreteClass_>
   struct CreateMemberIndicesImpl<TList_, startLine_, startLine_, ConcreteClass_> {
      typedef TList_ Indices;
   };


   template <int startLine_, int endLine_, class ConcreteClass_>
   struct CreateMemberIndices {
      typedef CreateMemberIndicesImpl< NullType, startLine_
                                     , endLine_ - 1, ConcreteClass_ > MemberIndicesImpl;
      typedef typename MemberIndicesImpl::Indices Indices;
   };

} // namespace Private

} // namespace ctrl

#endif /* CREATEMEMBERINDICES_H_ */
