#ifndef BASECLASSSERIALIZER_H_
#define BASECLASSSERIALIZER_H_

#include <ctrl/writeBuffer.h>
#include <ctrl/readBuffer.h>
#include <ctrl/typemanip.h>

namespace ctrl {

namespace Private {

   template <class ConcreteClass_>
   void serialize(const ConcreteClass_& object, WriteBuffer& buffer, int version);

   template <class ConcreteClass_>
   void deserialize(ConcreteClass_& value, ReadBuffer& buffer, int version) throw(Exception);



   template <class ConcreteClass_>
   struct BaseClassSerializer {
      template <class TList_>
      static void serialize(const ConcreteClass_& object, TList_, WriteBuffer& buffer, int version) {
         ctrl::Private::serialize(dynamic_cast<const typename TList_::Head&>(object), buffer, version);
         serialize(object, typename TList_::Tail(), buffer, version);
      }

      static void serialize(const ConcreteClass_& object, NullType, WriteBuffer& buffer, int version) {

      }

      template <class TList_>
      static void deserialize(ConcreteClass_& object, TList_, ReadBuffer& buffer, int version) throw(Exception) {
         ctrl::Private::deserialize(dynamic_cast<typename TList_::Head&>(object), buffer, version);
         deserialize(object, typename TList_::Tail(), buffer, version);
      }

      static void deserialize(ConcreteClass_& object, NullType, ReadBuffer& buffer, int version) throw(Exception) {

      }
   };

} // namespace Private

} // namespace ctrl

#endif // BASECLASSSERIALIZER_H_

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