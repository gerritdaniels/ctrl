#ifndef TYPEMANIP_H_
#define TYPEMANIP_H_

namespace ctrl {

namespace Private {

   template <class Head_, class Tail_>
   struct TypeList {
      typedef Head_ Head;
      typedef Tail_ Tail;
   };

   class NullType {};

   template <int i_>
   struct Int2Type {
      enum { value = i_ };
   };

   template <int condition_, class T0_, class T1_>
   struct Select {
      typedef T0_ Result;
   };

   template <class T0_, class T1_>
   struct Select<false, T0_, T1_> {
      typedef T1_ Result;
   };

   template <int condition_, int v0_, int v1_>
   struct SelectInt {
      enum { value = v0_ };
   };
   
   template <int v0_, int v1_>
   struct SelectInt<false, v0_, v1_> {
      enum { value = v1_ };
   };
   
} // namespace Private

} // namespace ctrl

#endif // TYPEMANIP_H_

/*
 * Copyright (C) 2010, 2012 by Gerrit Daniels <gerrit.daniels@gmail.com>
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
