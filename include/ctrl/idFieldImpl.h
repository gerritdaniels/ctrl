
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

#ifndef IDFIELDIMPL_H_
#define IDFIELDIMPL_H_

#include <ctrl/idField.h>
#include <ctrl/polymorphicSerializer.h>
#include <ctrl/buffer/abstractReadBuffer.h>
#include <ctrl/buffer/abstractWriteBuffer.h>

namespace ctrl {

namespace Private {

   template <class ConcreteClass_, class Indices_, int index_>
   struct RootHasIdFieldImpl {
      typedef typename ConcreteClass_::template __HasProperty<AsIdField, Indices_::Head::value> HasProperty;

      enum { definesProperty = HasProperty::value };
      enum { tmpIndex = SelectInt<definesProperty, Indices_::Head::value, index_>::value };

      typedef RootHasIdFieldImpl<ConcreteClass_, typename Indices_::Tail, tmpIndex> Following;

      enum { nbDefined = Following::nbDefined + (definesProperty ? 1 : 0) };
      enum { index = Following::index };
      enum { present = index != -1 };
   };

   template <class ConcreteClass_, int index_>
   struct RootHasIdFieldImpl<ConcreteClass_, NullType, index_> {
      enum { nbDefined = 0 };
      enum { index = index_ };
      enum { present = index != -1 };
   };

   template <class ConcreteClass_>
   struct RootHasIdField {
      typedef RootHasIdFieldImpl<ConcreteClass_, typename ConcreteClass_::__MemberIndices, -1> Impl;

      enum { error = Impl::nbDefined > 1 };
      enum { index = Impl::index };
      enum { present = Impl::present };
   };

   template <class H_, class T_>
   struct RootHasIdField<TypeList<H_, T_>> {
      enum { error = false };
      enum { index = -1 };
      enum { present = false };
   };

   template <class ConcreteClass_, int index_>
   struct IdFieldGetter {
      typedef typename ConcreteClass_::template __MemberType<index_>::Type Type;

      static Type& get(ConcreteClass_* ptr) {
         return ptr->* ConcreteClass_::__getMemberPtr(Int2Type<index_>());
      }
   };

   template <class ConcreteClass_>
   struct IdFieldGetter<ConcreteClass_, -1> {
      typedef int Type;

      static int dummy;
      template <class T_>
      static int& get(T_* ptr) {
         return dummy;
      }
   };

   template<class ConcreteClass_>
   int IdFieldGetter<ConcreteClass_, -1>::dummy = 0;

   template <class ConcreteClass_>
   class IdFieldImpl : public IdField::Impl {
   private:
      typedef DerivationRoots<ConcreteClass_> Roots;
      typedef RootHasIdField<typename Roots::Roots> Info;
      typedef IdFieldGetter<typename Roots::Roots, Info::index> Getter;

      typename Getter::Type m_value;
      bool m_null = false;

   public:
      virtual const void* value() const {
         return reinterpret_cast<const void*>(&m_value);
      }

      virtual const std::type_info& rootType() const {
         return typeid(typename Roots::Roots);
      }

      virtual bool isDefault() const {
         return !Info::present;
      }

      virtual void setDefaultValue(void* val) {
         if (isDefault()) {
            m_value = *reinterpret_cast<typename Getter::Type*>(val);
         }
      }

      virtual MemberContext getMemberContext() const {
         return MemberContext(new MemberContextImpl<typename Roots::Roots, Info::index>());
      }

      virtual void write(void* p, const std::string& dynamicName, AbstractWriteBuffer& buffer, const Context& context) {
         if (Info::present) {
            p = PolymorphicSerializer::instance().cast(dynamicName, ConcreteClass_::__staticName(), p);
            ConcreteClass_* ptr = reinterpret_cast<ConcreteClass_*>(p);
            m_value = Getter::get(ptr);
         }
         if (!Info::error && IsFundamental<typename Getter::Type>::value) {
            buffer.enterIdField(context);
            buffer.appendNonNullId(context);
            buffer.append(m_value, context);
            buffer.leaveIdField(context);
         } else {
            bool error = Info::error;
            bool present = Info::present;
            int index = Info::index;
            bool fundamental = IsFundamental<typename Getter::Type>::value;
            throw Exception("Error processing id field of class: " + ConcreteClass_::__staticName());
         }
      }

      virtual void read(AbstractReadBuffer& buffer, const Context& context) {
         if (!Info::error && IsFundamental<typename Getter::Type>::value) {
            buffer.enterIdField(context);
            m_null = buffer.isNullId(context);
            if (!m_null) {
               buffer.read(m_value, context);
            }
            buffer.leaveIdField(context);
         } else {
            throw Exception("Error processing id field of class: " + ConcreteClass_::__staticName());
         }
      }

      virtual void assign(void* p, const std::string& dynamicName) {
         if (Info::present) {
            p = PolymorphicSerializer::instance().cast(dynamicName, ConcreteClass_::__staticName(), p);
            ConcreteClass_* ptr = reinterpret_cast<ConcreteClass_*>(p);
            Getter::get(ptr) = m_value;
         }
      }

      virtual bool operator==(const IdField::Impl& that) const {
         return (!Info::present || rootType() == that.rootType())
            && m_value == *reinterpret_cast<const typename Getter::Type*>(that.value());
      }

      virtual bool isNull() const {
         return m_null;
      }

      virtual size_t hash() const {
         return (Info::present ? std::hash<const std::type_info*>()(&rootType()) : 0)
         + 13 * std::hash<typename Getter::Type>()(m_value);
      }
   };

} // namespace Private

} // namespace ctrl

#endif // IDFIELDIMPL_H_
