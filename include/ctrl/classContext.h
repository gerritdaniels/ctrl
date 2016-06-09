
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

#ifndef CLASSCONTEXT_H_
#define CLASSCONTEXT_H_

#include <memory>
#include <ctrl/derivationRoots.h>
#include <ctrl/exception.h>
#include <ctrl/idField.h>
#include <ctrl/idFieldImpl.h>


namespace ctrl {

namespace Private {

   template <class Single_>
   struct IsSingleRootConflict {
      static bool result(const std::type_info* type_info) {
         return false;
      }
   };

   template <class Head_, class Tail_>
   struct IsSingleRootConflict<TypeList<Head_, Tail_>> {
      static bool result(const std::type_info* type_info) {
         if (&typeid(Head_) == type_info) {
            return true;
         } else {
            return IsSingleRootConflict<Tail_>::result(type_info);
         }
      }
   };

   template <class Head_>
   struct IsSingleRootConflict<TypeList<Head_, NullType>> {
      static bool result(const std::type_info* type_info) {
         return &typeid(Head_) == type_info;
      }
   };



   class DummyClassContextImpl {
   public:
      void* getProperty(const std::type_info& type) const {
         return 0;
      }
   };



   class AbstractClassContextImpl {
   public:
      virtual void* getProperty(const std::type_info& type) const = 0;
      virtual bool hasSingleRoot() const = 0;
      virtual void* getRootProperty(const std::type_info& type) const = 0;
      virtual const std::type_info* getSingleRootId() const = 0;
      virtual bool isSingleRootConflict(const std::type_info* type_info) const = 0;
      virtual std::string getName() const = 0;
      virtual IdField getRootIdField() const = 0;
   };



   template <class ConcreteClass_>
   class ClassContextImpl : public AbstractClassContextImpl {
   private:
      template <class Root_, int i_>
      struct ClassContextSelector {
         typedef DummyClassContextImpl Context;
      };

      template <class Root_>
      struct ClassContextSelector<Root_, true> {
         typedef ClassContextImpl<Root_> Context;
      };

   public:
      virtual void* getProperty(const std::type_info& type) const {
         return getProperty(typename ConcreteClass_::__KnownProperties(), type);
      }

      virtual bool hasSingleRoot() const {
         return DerivationRoots<ConcreteClass_>::isSingleRoot;
      }

      virtual void* getRootProperty(const std::type_info& type) const {
         if (!hasSingleRoot()) {
            throw Exception("Single root property requested for multiply inherited class: " + ConcreteClass_::__staticName());
         }
         typedef DerivationRoots<ConcreteClass_> DerivationRoots;
         typename ClassContextSelector<typename DerivationRoots::Roots, DerivationRoots::isSingleRoot>::Context rootContext;
         return rootContext.getProperty(type);
      }

      virtual const std::type_info* getSingleRootId() const {
         return &typeid(typename DerivationRoots<ConcreteClass_>::Roots);
      }

      virtual bool isSingleRootConflict(const std::type_info* type_info) const {
         return IsSingleRootConflict<typename DerivationRoots<ConcreteClass_>::Roots>::result(type_info);
      }

      virtual std::string getName() const {
         return ConcreteClass_::__staticName();
      }

      virtual IdField getRootIdField() const {
         return IdField(new IdFieldImpl<ConcreteClass_>());
      }

   private:
      template <class TList_>
      void* getProperty(TList_, const std::type_info& type) const {
         typedef typename TList_::Head PropertyId;
         void* voidPtr = 0;
         if (typeid(PropertyId) == type) {
            voidPtr = ConcreteClass_::__getProperty(PropertyId(), Int2Type<-1>());
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

} // namespace Private

class Context;

class ClassContext {
public:
   ClassContext(Private::AbstractClassContextImpl* pimpl) : m_pimpl(pimpl),
         m_singleRootPropertyRequested(false) {

   }

   ClassContext(const ClassContext& that) : m_pimpl(that.m_pimpl),
         m_singleRootPropertyRequested(that.m_singleRootPropertyRequested) {

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

   template <class PropertyId_>
   bool hasRootProperty() const {
      if (m_pimpl.get() == 0) {
         return false;
      }
      if (!m_pimpl->hasSingleRoot()) {
         return false;
      }
      void* voidPtr = m_pimpl->getRootProperty(typeid(PropertyId_));
      if (voidPtr == 0) {
         return false;
      } else {
         typename PropertyId_::ValueType* valuePtr = reinterpret_cast<typename PropertyId_::ValueType*>(voidPtr);
         delete valuePtr;
         return true;
      }
   }

   template <class PropertyId_>
   typename PropertyId_::ValueType getRootProperty() const {
      if (!hasRootProperty<PropertyId_>()) {
         return PropertyId_::defaultValue();
      }
      void* voidPtr = m_pimpl->getRootProperty(typeid(PropertyId_));
      if (voidPtr == 0) {
         return PropertyId_::defaultValue();
      } else {
         typename PropertyId_::ValueType* valuePtr = reinterpret_cast<typename PropertyId_::ValueType*>(voidPtr);
         typename PropertyId_::ValueType value = *valuePtr;
         delete valuePtr;
         m_singleRootPropertyRequested = true;
         return value;
      }
   }

   bool hasSingleRoot() const {
      return m_pimpl->hasSingleRoot();
   }

   IdField getRootIdField() const {
      IdField field = m_pimpl->getRootIdField();
      m_singleRootPropertyRequested = !(field.isDefault());
      return field;
   }

private:
   friend Context;

   std::string getName() const {
      return m_pimpl->getName();
   }

   const std::type_info* getSingleRootId() const {
      return m_pimpl->getSingleRootId();
   }

   bool singleRootPropertyRequested() const {
      return m_singleRootPropertyRequested;
   }

   bool isSingleRootConflict(const std::type_info* type_info) const {
      if (m_pimpl.get() == 0) {
         return false;
      }
      return m_pimpl->isSingleRootConflict(type_info);
   }

   std::shared_ptr<Private::AbstractClassContextImpl> m_pimpl;
   mutable bool m_singleRootPropertyRequested;
};

} // namespace ctrl


#endif // CLASSCONTEXT_H_
