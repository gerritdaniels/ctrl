#ifndef BYTESWAPPER_H_
#define BYTESWAPPER_H_

namespace ctrl {

namespace Private {

   template <class Number_, int length_>
   struct ByteSwapper {
      static void swap(Number_& val) { }
   };

   template <class Number_>
   struct ByteSwapper<Number_, 1> {
      static void swap(Number_& val) { }
   };

   template <class Number_>
   struct ByteSwapper<Number_, 2> {
      static void swap(Number_& val) {
         val = ((val & 0xff00) >> 8)
             | ((val & 0x00ff) << 8);
      }
   };

   template <class Number_>
   struct ByteSwapper<Number_, 4> {
      static void swap(Number_& val) {
         val = ((val & 0xff000000) >> 24)
             | ((val & 0x00ff0000) >> 8)
             | ((val & 0x0000ff00) << 8)
             | ((val & 0x000000ff) << 24);
      }
   };

   template <class Number_>
   struct ByteSwapper<Number_, 8> {
      static void swap(Number_& val) {
         val = ((val & 0xff00000000000000) >> 56)
             | ((val & 0x00ff000000000000) >> 40)
             | ((val & 0x0000ff0000000000) >> 24)
             | ((val & 0x000000ff00000000) >> 8)
             | ((val & 0x00000000ff000000) << 8)
             | ((val & 0x0000000000ff0000) << 24)
             | ((val & 0x000000000000ff00) << 40)
             | ((val & 0x00000000000000ff) << 56);
      }
   };

} // namespace Private

} // namespace ctrl

#endif // BYTESWAPPER_H_

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
