
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

#ifndef MEMBERCONTEXT_H_
#define MEMBERCONTEXT_H_

#include <memory>
#include <string>
#include <ctrl/typemanip.h>

namespace ctrl {

namespace Private {

   class AbstractMemberContextImpl {
   public:
      virtual void* getProperty(const std::type_info& type) const = 0;
      virtual std::string getName() const = 0;
      virtual bool isFundamental() const = 0;
      virtual bool isCollection() const = 0;
      virtual bool isMap() const = 0;
      virtual bool isMapKeyFundamental() const = 0;
      virtual bool isPointer() const = 0;
   };

   template <class ConcreteClass_, int index_>
   class MemberContextImpl : public AbstractMemberContextImpl {
   public:
      virtual void* getProperty(const std::type_info& type) const {
         return getProperty(typename ConcreteClass_::CTRL_KnownProperties(), type);
      }

      virtual std::string getName() const {
         return ConcreteClass_::CTRL_getMemberName(Int2Type<index_>());
      }

      virtual bool isFundamental() const {
         return IsFundamental<typename ConcreteClass_::template CTRL_MemberType<index_>::Type>::value;
      }

      virtual bool isCollection() const {
         return IsCollection<typename ConcreteClass_::template CTRL_MemberType<index_>::Type>::value;
      }

      virtual bool isMap() const {
         return IsMap<typename ConcreteClass_::template CTRL_MemberType<index_>::Type>::value;
      }

      virtual bool isMapKeyFundamental() const {
         return IsMap<typename ConcreteClass_::template CTRL_MemberType<index_>::Type>::keyFundamental;
      }

      virtual bool isPointer() const {
         return IsPointer<typename ConcreteClass_::template CTRL_MemberType<index_>::Type>::value;
      }

   private:
      template <class TList_>
      void* getProperty(TList_, const std::type_info& type) const {
         typedef typename TList_::Head PropertyId;
         void* voidPtr = 0;
         if (typeid(PropertyId) == type) {
            voidPtr = ConcreteClass_::CTRL_getProperty(PropertyId(), Int2Type<index_>());
         }
         if (voidPtr == 0) {
            return getProperty(typename TList_::Tail(), type);
         }
         return voidPtr;
      }

      void* getProperty(NullType, const std::type_info& type) const {
         return 0;
      }
   };

   template <class ConcreteClass_>
   class MemberContextImpl<ConcreteClass_, -1> : public AbstractMemberContextImpl {
   public:
      virtual void* getProperty(const std::type_info& type) const {
         return 0;
      }

      virtual std::string getName() const {
         return "__id";
      }

      virtual bool isFundamental() const {
         return true;
      }
      virtual bool isCollection() const {
         return false;
      }

      virtual bool isMap() const {
         return false;
      }

      virtual bool isMapKeyFundamental() const {
         return false;
      }

      virtual bool isPointer() const {
         return false;
      }

   };

} // namespace Private

class MemberContext {
public:

   MemberContext(Private::AbstractMemberContextImpl* pimpl) : m_pimpl(pimpl) {

   }

   MemberContext(const MemberContext& that) : m_pimpl(that.m_pimpl) {

   }

   template <class PropertyId_>
   typename PropertyId_::ValueType getProperty() const {
      if (m_pimpl.get() == 0) {
         return PropertyId_::defaultValue();
      }
      void* voidPtr = m_pimpl->getProperty(typeid(PropertyId_));
      if (voidPtr == 0) {
         return PropertyId_::defaultValue();
      } else {
         typename PropertyId_::ValueType* valuePtr = reinterpret_cast<typename PropertyId_::ValueType*>(voidPtr);
         typename PropertyId_::ValueType value = *valuePtr;
         delete valuePtr;
         return value;
      }
   }

   std::string getName() const {
      return m_pimpl->getName();
   }

   bool isFundamental() const {
      return m_pimpl->isFundamental();
   }

   bool isCollection() const {
	   return m_pimpl->isCollection();
   }

   bool isMap() const {
	   return m_pimpl->isMap();
   }

   bool isMapKeyFundamental() const {
	   return m_pimpl->isMapKeyFundamental();
   }

   bool isPointer() const {
	   return m_pimpl->isPointer();
   }

private:
   std::shared_ptr<Private::AbstractMemberContextImpl> m_pimpl;
};

} // namespace ctrl

#endif // MEMBERCONTEXT_H_
