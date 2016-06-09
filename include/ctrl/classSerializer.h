
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

#ifndef CLASSSERIALIZER_H_
#define CLASSSERIALIZER_H_

#include <ctrl/forwardDeserialize.h>
#include <ctrl/forwardSerialize.h>
#include <ctrl/buffer/abstractReadBuffer.h>
#include <ctrl/buffer/abstractWriteBuffer.h>
#include <ctrl/typemanip.h>
#include <ctrl/context.h>

namespace ctrl {

class WithVersion;

namespace Private {

   template <class ConcreteClass_>
   class ClassSerializer {
   public:
      template <class Indices_>
      static void serialize(const ConcreteClass_& object, Indices_ indices, AbstractWriteBuffer& buffer, int version, const Context& context) {
         Context memberContext(context, MemberContext(new MemberContextImpl<ConcreteClass_, Indices_::Head::value>()));
         if (version >= memberContext.getOwningMember().getProperty<WithVersion>()) {
            buffer.enterMember(memberContext);
            ctrl::Private::serialize( object.* ConcreteClass_::__getMemberPtr(typename Indices_::Head()),
                                      buffer, version, memberContext );
            buffer.leaveMember(memberContext);
         }
         serialize(object, typename Indices_::Tail(), buffer, version, context);
      }

      static void serialize(const ConcreteClass_& object, NullType indices, AbstractWriteBuffer& buffer, int version, const Context& context) {

      }

      template <class Indices_>
      static void deserialize(ConcreteClass_& object, Indices_ indices, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
         Context memberContext(context, MemberContext(new MemberContextImpl<ConcreteClass_, Indices_::Head::value>()));
         if (version >= memberContext.getOwningMember().getProperty<WithVersion>()) {
            buffer.enterMember(memberContext);
            ctrl::Private::deserialize( object.* ConcreteClass_::__getMemberPtr(typename Indices_::Head()),
                                        buffer, version, memberContext );
            buffer.leaveMember(memberContext);
         }
         deserialize(object, typename Indices_::Tail(), buffer, version, context);
      }

      static void deserialize(ConcreteClass_& object, NullType indices, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {

      }
   };

} // namespace Private

} // namespace ctrl

#endif // CLASSSERIALIZER_H_
