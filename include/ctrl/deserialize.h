
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

#ifndef DESERIALIZE_H_
#define DESERIALIZE_H_

#include <boost/locale.hpp>
#include <ctrl/forwardDeserialize.h>
#include <ctrl/classSerializer.h>
#include <ctrl/baseClassSerializer.h>
#include <ctrl/polymorphicSerializer.h>
#include <ctrl/buffer/binaryReadBufferImpl.h>
#include <ctrl/buffer/xmlReadBuffer.h>
#include <ctrl/platformFormat.h>
#include <ctrl/exception.h>
#include <ctrl/context.h>

namespace ctrl {

template <class ConcreteClass_>
ConcreteClass_* fromReadBuffer(AbstractReadBuffer& buffer, int version) throw(Exception) {
   ConcreteClass_* ptr;
   try {
      ptr = new ConcreteClass_();
      int dataVersion;
      Context context(ClassContext(new Private::ClassContextImpl<ConcreteClass_>()));
      buffer.readVersion(dataVersion, context);
      if (dataVersion != version)
         throw Exception("deserialize: version mismatch");
      Private::deserialize(*ptr, buffer, version, context);
      return ptr;
   }
   catch(...) { delete ptr; throw; }
}

template <class ConcreteClass_, int alignment_, int endian_>
ConcreteClass_* fromBinary(const char* bytes, const long& length, int version = 1) throw(Exception) {
   Private::BinaryReadBuffer buffer(new Private::BinaryReadBufferImpl<alignment_, endian_>(bytes, length));
   ConcreteClass_* ptr = fromReadBuffer<ConcreteClass_>(buffer, version);
   if (!buffer.reachedEnd()) {
      delete ptr;
      throw Exception("Input data is corrupt");
   }
   return ptr;
}

template <class ConcreteClass_, int alignment_>
ConcreteClass_* fromBinary(const char* bytes, const long& length, int version = 1) throw(Exception) {
   return fromBinary<ConcreteClass_, alignment_, CTRL_BYTE_ORDER>(bytes, length, version);
}

template <class ConcreteClass_>
ConcreteClass_* fromBinary(const char* bytes, const long& length, int version = 1) throw(Exception) {
   return fromBinary<ConcreteClass_, CTRL_MEMORY_ALIGNMENT, CTRL_BYTE_ORDER>(bytes, length, version);
}

template <class ConcreteClass_>
ConcreteClass_* fromXml(const std::string& data, int version = 1) throw(Exception) {
   Private::XmlReadBuffer buffer(data);
   return fromReadBuffer<ConcreteClass_>(buffer, version);
}

namespace Private {

   template <class ConcreteClass_>
   void deserialize(ConcreteClass_& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.enterObject(context);
      typedef typename ConcreteClass_::CTRL_BaseClasses TList;
      BaseClassSerializer<ConcreteClass_>::deserialize(value, TList(), buffer, version, context);

      typedef typename ConcreteClass_::CTRL_MemberIndices Indices;
      ClassSerializer<ConcreteClass_>::deserialize(value, Indices(), buffer, version, context);
      buffer.enterObject(context);
      ConcreteClass_::initialize(value, version);
   }

   template <size_t size_>
   void deserialize(std::bitset<size_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      size_t nbChars = size_ / 8 + (size_ % 8 == 0 ? 0 : 1);
      char* bits = new char[nbChars];
      buffer.readBits(bits, size_, context);

      unsigned char mask = 0x80;
      for (size_t i = 0; i < size_; ++i) {
         if ((bits[i / 8] & mask) != 0)
            elements.set(i);
         if (mask == 0x01)
            mask = 0x80;
         else
            mask = mask >> 1;
      }
	  delete[] bits;
   }

   template <class Element_, class Alloc_>
   void deserialize(std::deque<Element_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::deque<Element_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::deque<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         buffer.nextCollectionElement(context);
         deserialize(el, buffer, version, context);
         elements.push_back(std::move(el));
      }
      buffer.leaveCollection(context);
   }

   template <class Element_, class Alloc_>
   void deserialize(std::list<Element_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::list<Element_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::list<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         buffer.nextCollectionElement(context);
         deserialize(el, buffer, version, context);
         elements.push_back(std::move(el));
      }
      buffer.leaveCollection(context);
   }

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void deserialize(std::map<Key_, Value_, Comp_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::map<Key_, Value_, Comp_, Alloc_>::size_type size;
      buffer.enterMap(context);
      buffer.readCollectionSize(size, context);
      for (typename std::map<Key_, Value_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         buffer.nextCollectionElement(context);
         Key_ key;
         buffer.enterKey(context);
         deserialize(key, buffer, version, context);
         buffer.leaveKey(context);
         Value_ val;
         buffer.enterValue(context);
         deserialize(val, buffer, version, context);
         buffer.leaveValue(context);
         elements[std::move(key)] = std::move(val);
      }
      buffer.leaveMap(context);
   }

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void deserialize(std::multimap<Key_, Value_, Comp_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::multimap<Key_, Value_, Comp_, Alloc_>::size_type size;
      buffer.enterMap(context);
      buffer.readCollectionSize(size, context);
      for (typename std::multimap<Key_, Value_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         buffer.nextCollectionElement(context);
         Key_ key;
         buffer.enterKey(context);
         deserialize(key, buffer, version, context);
         buffer.leaveKey(context);
         Value_ val;
         buffer.enterValue(context);
         deserialize(val, buffer, version, context);
         buffer.leaveValue(context);
         elements.insert( std::move(std::pair<Key_, Value_>(std::move(key), std::move(val))) );
      }
      buffer.leaveMap(context);
   }

   template <class Element_, class Container_>
   void deserialize(std::queue<Element_, Container_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::queue<Element_, Container_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::queue<Element_, Container_>::size_type i = 0; i < size; ++i) {
         Element_ element;
         buffer.nextCollectionElement(context);
         deserialize(element, buffer, version, context);
         elements.push(std::move(element));
      }
      buffer.leaveCollection(context);
   }

   template <class Element_, class Container_, class Comp_>
   void deserialize(std::priority_queue<Element_, Container_, Comp_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::priority_queue<Element_, Container_, Comp_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::priority_queue<Element_, Container_, Comp_>::size_type i = 0; i < size; ++i) {
         Element_ element;
         buffer.nextCollectionElement(context);
         deserialize(element, buffer, version, context);
         elements.push(std::move(element));
      }
      buffer.leaveCollection(context);
   }

   template <class Key_, class Comp_, class Alloc_>
   void deserialize(std::set<Key_, Comp_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::set<Key_, Comp_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::set<Key_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         buffer.nextCollectionElement(context);
         deserialize(element, buffer, version, context);
         elements.insert(std::move(element));
      }
      buffer.leaveCollection(context);
   }

   template <class Key_, class Comp_, class Alloc_>
   void deserialize(std::multiset<Key_, Comp_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::multiset<Key_, Comp_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::multiset<Key_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         buffer.nextCollectionElement(context);
         deserialize(element, buffer, version, context);
         elements.insert(std::move(element));
      }
      buffer.leaveCollection(context);
   }

   template <class Element_, class Container_>
   void deserialize(std::stack<Element_, Container_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::stack<Element_, Container_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      std::vector<Element_> temp;
      for (typename std::stack<Element_, Container_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         buffer.nextCollectionElement(context);
         deserialize(el, buffer, version, context);
         temp.push_back(el);
      }
      buffer.leaveCollection(context);
      for ( typename std::vector<Element_>::const_reverse_iterator iter = temp.rbegin();
            iter != temp.rend(); ++iter )
         elements.push(std::move(*iter));
   }

   template <class Element_, class Alloc_>
   void deserialize(std::vector<Element_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::vector<Element_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::vector<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         buffer.nextCollectionElement(context);
         deserialize(el, buffer, version, context);
         elements.push_back(std::move(el));
      }
      buffer.leaveCollection(context);
   }

   template <class Element_, size_t size_>
   void deserialize(std::array<Element_, size_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::array<Element_, size_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::array<Element_, size_>::size_type i = 0; i < size; ++i) {
         buffer.nextCollectionElement(context);
         deserialize(elements[i], buffer, version, context);
      }
      buffer.leaveCollection(context);
   }

   template <class Element_, class Alloc_>
   void deserialize(std::forward_list<Element_, Alloc_>& elements, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      typename std::forward_list<Element_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      typename std::forward_list<Element_, Alloc_>::const_iterator iter = elements.before_begin();
      for (typename std::forward_list<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         buffer.nextCollectionElement(context);
         deserialize(el, buffer, version, context);
         elements.insert_after(iter, std::move(el));
         ++iter;
      }
      buffer.leaveCollection(context);
   }

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                   , AbstractReadBuffer& buffer, int version, const Context& context ) throw(Exception) {
      typename std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.enterMap(context);
      buffer.readCollectionSize(size, context);
      for (typename std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         buffer.nextCollectionElement(context);
         Key_ key;
         buffer.enterKey(context);
         deserialize(key, buffer, version, context);
         buffer.leaveKey(context);
         Value_ val;
         buffer.enterValue(context);
         deserialize(val, buffer, version, context);
         buffer.leaveValue(context);
         elements[std::move(key)] = std::move(val);
      }
      buffer.leaveMap(context);
   }

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                   , AbstractReadBuffer& buffer, int version, const Context& context ) throw(Exception) {
      typename std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.enterMap(context);
      buffer.readCollectionSize(size, context);
      for (typename std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         buffer.nextCollectionElement(context);
         Key_ key;
         buffer.enterKey(context);
         deserialize(key, buffer, version, context);
         buffer.leaveKey(context);
         Value_ val;
         buffer.enterValue(context);
         deserialize(val, buffer, version, context);
         buffer.leaveValue(context);
         elements.insert( std::move(std::pair<Key_, Value_>(std::move(key), std::move(val))) );
      }
      buffer.leaveMap(context);
   }

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_set<Key_, Hash_, Pred_, Alloc_>& elements
                   , AbstractReadBuffer& buffer, int version, const Context& context ) throw(Exception) {
      typename std::unordered_set<Key_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::unordered_set<Key_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         buffer.nextCollectionElement(context);
         deserialize(element, buffer, version, context);
         elements.insert(std::move(element));
      }
      buffer.leaveCollection(context);
   }

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>& elements
                   , AbstractReadBuffer& buffer, int version, const Context& context ) throw(Exception) {
      typename std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      for (typename std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         buffer.nextCollectionElement(context);
         deserialize(element, buffer, version, context);
         elements.insert(std::move(element));
      }
      buffer.leaveCollection(context);
   }

   template <class Element_>
   void deserialize(boost::shared_ptr<Element_>& ptr, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      Context staticContext(context, Element_::CTRL_staticContext());
      IdField idField = staticContext.getClassContext().getRootIdField();
      idField.read(buffer, staticContext);
      if (idField.isNull()) {
         return;
      }

      boost::shared_ptr<void> voidPtr;
      std::string staticName(Element_::CTRL_staticName());

      if (buffer.getBoostPointerRepository().isRegistered(idField)) {
         voidPtr = buffer.getBoostPointerRepository().get(idField);
         void* p = voidPtr.get();

         std::string className(buffer.getBoostPointerRepository().getTypeName(idField));
         p = PolymorphicSerializer::instance().cast(className, staticName, p);

         ptr = boost::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      } else {
         if (PolymorphicSerializer::instance().isPolymorph(staticName)) {
            std::string dynamicName;
            buffer.readTypeId(dynamicName, staticContext);
            void* p = PolymorphicSerializer::instance().deserialize(dynamicName, buffer, version, idField, staticContext);
            p = PolymorphicSerializer::instance().cast(dynamicName, staticName, p);
            ptr = boost::shared_ptr<Element_>(reinterpret_cast<Element_*>(p));
         } else {
            ptr = boost::shared_ptr<Element_>(new Element_());
            idField.assign(reinterpret_cast<void*>(ptr.get()), staticName);
            deserialize(*ptr, buffer, version, staticContext);
         }
         voidPtr = boost::shared_ptr<void>(ptr);
         buffer.getBoostPointerRepository().add(idField, voidPtr, staticName);

         if (buffer.getBoostPointerRepository().hasDelayedAssignment(idField))
            buffer.getBoostPointerRepository().doAssignment(idField, voidPtr, staticName);
      }
   }

   template <class Element_>
   void deserialize(boost::weak_ptr<Element_>& ptr, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      Context staticContext(context, Element_::CTRL_staticContext());
      IdField idField = staticContext.getClassContext().getRootIdField();
      idField.read(buffer, staticContext);
      if (idField.isNull()) {
         return;
      }

      if (buffer.getBoostPointerRepository().isRegistered(idField)) {
         boost::shared_ptr<void> voidPtr = buffer.getBoostPointerRepository().get(idField);
         void* p = voidPtr.get();

         std::string className(buffer.getBoostPointerRepository().getTypeName(idField));
         std::string staticClassName(Element_::CTRL_staticName());
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);

         ptr = boost::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      }
      else
         buffer.getBoostPointerRepository().delayAssignment(idField, ptr);
   }

   template <class Element_>
   void deserialize(std::shared_ptr<Element_>& ptr, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      Context staticContext(context, Element_::CTRL_staticContext());
      IdField idField = staticContext.getClassContext().getRootIdField();
      idField.read(buffer, staticContext);
      if (idField.isNull()) {
         return;
      }

      std::shared_ptr<void> voidPtr;
      std::string staticName(Element_::CTRL_staticName());

      if (buffer.getStdPointerRepository().isRegistered(idField)) {
         voidPtr = buffer.getStdPointerRepository().get(idField);
         void* p = voidPtr.get();

         std::string className(buffer.getStdPointerRepository().getTypeName(idField));
         p = PolymorphicSerializer::instance().cast(className, staticName, p);

         ptr = std::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      } else {
         if (PolymorphicSerializer::instance().isPolymorph(staticName)) {
            std::string dynamicName;
            buffer.readTypeId(dynamicName, staticContext);
            void* p = PolymorphicSerializer::instance().deserialize(dynamicName, buffer, version, idField, staticContext);
            p = PolymorphicSerializer::instance().getCast(dynamicName, staticName)(p);
            ptr = std::shared_ptr<Element_>(reinterpret_cast<Element_*>(p));
         } else {
            ptr = std::shared_ptr<Element_>(new Element_());
            idField.assign(reinterpret_cast<void*>(ptr.get()), staticName);
            deserialize(*ptr, buffer, version, staticContext);
         }
         voidPtr = std::shared_ptr<void>(ptr);
         buffer.getStdPointerRepository().add(idField, voidPtr, staticName);

         if (buffer.getStdPointerRepository().hasDelayedAssignment(idField))
            buffer.getStdPointerRepository().doAssignment(idField, voidPtr, staticName);
      }
   }

   template <class Element_>
   void deserialize(std::weak_ptr<Element_>& ptr, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      Context staticContext(context, Element_::CTRL_staticContext());
      IdField idField = staticContext.getClassContext().getRootIdField();
      idField.read(buffer, staticContext);
      if (idField.isNull()) {
         return;
      }

      if (buffer.getStdPointerRepository().isRegistered(idField)) {
         std::shared_ptr<void> voidPtr = buffer.getStdPointerRepository().get(idField);
         void* p = voidPtr.get();

         std::string className(buffer.getStdPointerRepository().getTypeName(idField));
         std::string staticClassName(Element_::CTRL_staticName());
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);

         ptr = std::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      }
      else
         buffer.getStdPointerRepository().delayAssignment(idField, ptr);
   }

   template <class Element_>
   void deserialize(std::auto_ptr<Element_>& ptr, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      Context staticContext(context, Element_::CTRL_staticContext());
      IdField idField = staticContext.getClassContext().getRootIdField();
      idField.read(buffer, staticContext);
      if (idField.isNull()) {
         return;
      }

      std::string staticName(Element_::CTRL_staticName());
      if (PolymorphicSerializer::instance().isPolymorph(staticName)) {
         std::string className;
         buffer.readTypeId(className, staticContext);
         void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version, idField, staticContext);
         // we can't delete p as long as it is a void pointer
         if (PolymorphicSerializer::instance().hasCast(className, staticName))
            p = PolymorphicSerializer::instance().getCast(className, staticName)(p);
         ptr = std::auto_ptr<Element_>(reinterpret_cast<Element_*>(p));
      } else {
         ptr = std::auto_ptr<Element_>(new Element_());
         idField.assign(reinterpret_cast<void*>(ptr.get()), staticName);
         deserialize(*ptr, buffer, version, staticContext);
      }
   }

   template <class Element_>
   void deserialize(std::unique_ptr<Element_>& ptr, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      Context staticContext(context, Element_::CTRL_staticContext());
      IdField idField = staticContext.getClassContext().getRootIdField();
      idField.read(buffer, staticContext);
      if (idField.isNull()) {
         return;
      }

      std::string staticName(Element_::CTRL_staticName());
      if (PolymorphicSerializer::instance().isPolymorph(staticName)) {
         std::string className;
         buffer.readTypeId(className, staticContext);
         void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version, idField, staticContext);
         // we can't delete p as long as it is a void pointer
         if (PolymorphicSerializer::instance().hasCast(className, staticName))
            p = PolymorphicSerializer::instance().getCast(className, staticName)(p);
         ptr = std::unique_ptr<Element_>(reinterpret_cast<Element_*>(p));
      } else {
         ptr = std::unique_ptr<Element_>(new Element_());
         idField.assign(reinterpret_cast<void*>(ptr.get()), staticName);
         deserialize(*ptr, buffer, version, context);
      }
   }

   template <class Element_>
   void deserialize(Element_*& ptr, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      Context staticContext(context, Element_::CTRL_staticContext());
      IdField idField = staticContext.getClassContext().getRootIdField();
      idField.read(buffer, staticContext);
      if (idField.isNull()) {
         return;
      }

      std::string staticName(Element_::CTRL_staticName());
      if (buffer.getRawPointerRepository().isRegistered(idField)) {
         void* p = buffer.getRawPointerRepository().get(idField);

         std::string className(buffer.getRawPointerRepository().getTypeName(idField));
         p = PolymorphicSerializer::instance().cast(className, staticName, p);

         ptr = reinterpret_cast<Element_*>(p);
      } else {
         if (PolymorphicSerializer::instance().isPolymorph(staticName)) {
            std::string className;
            buffer.readTypeId(className, context);
            void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version, idField, staticContext);
            p = PolymorphicSerializer::instance().cast(className, staticName, p);
            ptr = reinterpret_cast<Element_*>(p);
         } else {
            ptr = new Element_();
            idField.assign(reinterpret_cast<void*>(ptr), staticName);
            deserialize(*ptr, buffer, version, staticContext);
         }
         buffer.getRawPointerRepository().add(idField, reinterpret_cast<void*>(ptr), staticName);
      }
   }

   template <class First_, class Second_>
   void deserialize(std::pair<First_, Second_>& obj, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.enterMember(context, "first");
      deserialize(obj.first, buffer, version, context);
      buffer.leaveMember(context);
      buffer.enterMember(context, "second");
      deserialize(obj.second, buffer, version, context);
      buffer.leaveMember(context);
   }

   template <class Number_>
   void deserialize(std::complex<Number_>& obj, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      double real, imag;
      buffer.enterMember(context, "real");
      deserialize(real, buffer, version, context);
      buffer.leaveMember(context);
      buffer.enterMember(context, "imag");
      deserialize(imag, buffer, version, context);
      buffer.leaveMember(context);
      obj = std::complex<Number_>(real, imag);
   }

   template <class Number_>
   void deserialize(std::valarray<Number_>& obj, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      size_t size;
      buffer.enterCollection(context);
      buffer.readCollectionSize(size, context);
      obj = std::valarray<Number_>(size);
      for (size_t i = 0; i < size; ++i) {
         buffer.nextCollectionElement(context);
         deserialize(obj[i], buffer, version, context);
      }
      buffer.leaveCollection(context);
   }

   template <>
   void deserialize(bool& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(char& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(short& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(int& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(long& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(long long& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(unsigned char& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(unsigned short& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(unsigned int& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(unsigned long& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(unsigned long long& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(float& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(double& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(std::string& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      buffer.read(value, context);
   }

   template <>
   void deserialize(std::wstring& value, AbstractReadBuffer& buffer, int version, const Context& context) throw(Exception) {
      std::string tmp;
      buffer.read(tmp, context);
      value = boost::locale::conv::utf_to_utf<wchar_t>(tmp);
   }

} // namespace Private

} // namespace ctrl

#endif // DESERIALIZE_H_
