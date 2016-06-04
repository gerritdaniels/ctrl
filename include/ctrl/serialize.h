#ifndef SERIALIZE_H_
#define SERIALIZE_H_


#include <boost/locale.hpp>
#include <ctrl/forwardSerialize.h>
#include <ctrl/classSerializer.h>
#include <ctrl/baseClassSerializer.h>
#include <ctrl/polymorphicSerializer.h>
#include <ctrl/writeBufferImpl.h>
#include <ctrl/platformFormat.h>

namespace ctrl {

template <int alignment_, int endian_, class ConcreteClass_>
char* serialize(const ConcreteClass_& object, long& length, int version = 1) {
   Private::WriteBuffer buffer(new Private::WriteBufferImpl<alignment_, endian_>());
   buffer.append(version);
   Private::serialize(object, buffer, version);
   length = buffer.length();
   return buffer.getData();
}

template <int alignment_, class ConcreteClass_>
char* serialize(const ConcreteClass_& object, long& length, int version = 1) {
   return serialize<alignment_, CTRL_BYTE_ORDER, ConcreteClass_>(object, length, version);
}

template <class ConcreteClass_>
char* serialize(const ConcreteClass_& object, long& length, int version = 1) {
   return serialize<CTRL_MEMORY_ALIGNMENT, CTRL_BYTE_ORDER, ConcreteClass_>(object, length, version);
}

namespace Private {

   template <class ConcreteClass_>
   void serialize(const ConcreteClass_& object, WriteBuffer& buffer, int version) {
      typedef typename ConcreteClass_::__BaseClasses BaseClasses;
      BaseClassSerializer<ConcreteClass_>::serialize(object, BaseClasses(), buffer, version);

      typedef typename ConcreteClass_::__MemberIndices Indices;
      ClassSerializer<ConcreteClass_>::serialize(object, Indices(), buffer, version);
   }

   template <size_t size_>
   void serialize(const std::bitset<size_>& elements, WriteBuffer& buffer, int version) {
      size_t nbChars = size_ / 8 + (size_ % 8 == 0 ? 0 : 1);
      char bits[nbChars];
      for (size_t i = 0; i < nbChars; ++i)
         bits[i] = 0;

      unsigned char mask = 0x80;
      for (size_t i = 0; i < size_; ++i) {
         if (elements[i])
            bits[i / 8] = bits[i / 8] | mask;
         if (mask == 0x01)
            mask = 0x80;
         else
            mask = mask >> 1;
      }
      buffer.append(bits, nbChars);
   }

   template <class Element_, class Alloc_>
   void serialize(const std::deque<Element_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for ( typename std::deque<Element_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter )
           serialize(*iter, buffer, version);
   }

   template <class Element_, class Alloc_>
   void serialize(const std::list<Element_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for ( typename std::list<Element_, Alloc_>::const_iterator iter = elements.begin();
            iter != elements.end(); ++iter )
         serialize(*iter, buffer, version);
   }

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void serialize(const std::map<Key_, Value_, Comp_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for ( typename std::map<Key_, Value_, Comp_, Alloc_>::const_iterator iter = elements.begin();
            iter != elements.end(); ++iter ) {
         serialize(iter->first, buffer, version);
         serialize(iter->second, buffer, version);
      }
   }

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void serialize(const std::multimap<Key_, Value_, Comp_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for ( typename std::multimap<Key_, Value_, Comp_, Alloc_>::const_iterator iter = elements.begin();
            iter != elements.end(); ++iter ) {
         serialize(iter->first, buffer, version);
         serialize(iter->second, buffer, version);
      }
   }

   template <class Element_, class Container_>
   void serialize(const std::queue<Element_, Container_>& elements, WriteBuffer& buffer, int version) {
      std::queue<Element_, Container_> copy(elements);
      buffer.append(copy.size());
      while (!copy.empty()) {
         serialize(copy.front(), buffer, version);
         copy.pop();
      }
   }

   template <class Element_, class Container_, class Comp_>
   void serialize(const std::priority_queue<Element_, Container_, Comp_>& elements, WriteBuffer& buffer, int version) {
      std::priority_queue<Element_, Container_, Comp_> copy(elements);
      buffer.append(copy.size());
      while (!copy.empty()) {
         serialize(copy.top(), buffer, version);
         copy.pop();
      }
   }

   template <class Key_, class Comp_, class Alloc_>
   void serialize(const std::set<Key_, Comp_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for ( typename std::set<Key_, Comp_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter )
         serialize(*iter, buffer, version);
   }

   template <class Key_, class Comp_, class Alloc_>
   void serialize(const std::multiset<Key_, Comp_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for ( typename std::multiset<Key_, Comp_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter )
         serialize(*iter, buffer, version);
   }

   template <class Element_, class Container_>
   void serialize(const std::stack<Element_, Container_>& elements, WriteBuffer& buffer, int version) {
      std::stack<Element_, Container_> copy(elements);
      buffer.append(copy.size());
      while (!copy.empty()) {
         serialize(copy.top(), buffer, version);
         copy.pop();
      }
   }

   template <class Element_, class Alloc_>
   void serialize(const std::vector<Element_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for ( typename std::vector<Element_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter )
         serialize(*iter, buffer, version);
   }

      template <class Element_, size_t size_>
   void serialize(const std::array<Element_, size_>& elements, WriteBuffer& buffer, int version) {
      buffer.append(elements.size());
      for (typename std::array<Element_, size_>::const_iterator iter = elements.begin(); iter != elements.end(); ++iter)
         serialize(*iter, buffer, version);
   }

   template <class Element_, class Alloc_>
   void serialize(const std::forward_list<Element_, Alloc_>& elements, WriteBuffer& buffer, int version) {
      size_t size = 0;
      for ( typename std::forward_list<Element_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter )
         ++size;
      buffer.append(size);

      for ( typename std::forward_list<Element_, Alloc_>::const_iterator iter = elements.begin();
            iter != elements.end(); ++iter )
         serialize(*iter, buffer, version);
   }

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                 , WriteBuffer& buffer, int version ) {
      buffer.append(elements.size());
      for ( typename std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter ) {
         serialize(iter->first, buffer, version);
         serialize(iter->second, buffer, version);
      }
   }

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                 , WriteBuffer& buffer, int version ) {
      buffer.append(elements.size());
      for ( typename std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter ) {
         serialize(iter->first, buffer, version);
         serialize(iter->second, buffer, version);
      }
   }

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_set<Key_, Hash_, Pred_, Alloc_>& elements
                 , WriteBuffer& buffer, int version ) {
      buffer.append(elements.size());
      for ( typename std::unordered_set<Key_, Hash_, Pred_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter )
           serialize(*iter, buffer, version);
   }

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>& elements
                 , WriteBuffer& buffer, int version ) {
      buffer.append(elements.size());
      for ( typename std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>::const_iterator iter = elements.begin();
           iter != elements.end(); ++iter )
           serialize(*iter, buffer, version);
   }

   template <class Element_>
   void serialize(const boost::shared_ptr<Element_>& ptr, WriteBuffer& buffer, int version) {
      if (ptr.get() == 0) {
         buffer.append((int)0);
         return;
      }
      void* p = reinterpret_cast<void*>(ptr.get());
      std::string className = ptr->__className();
      std::string staticClassName = Element_::__staticClassName();

      if (PolymorphicSerializer::instance().hasCast(staticClassName, className))
         p = PolymorphicSerializer::instance().getCast(staticClassName, className)(p);

      if (buffer.getPointerRepository().isRegistered(p))
         buffer.append(buffer.getPointerRepository().get(p));
      else {
         if (buffer.getPointerRepository().isReserved(p)) {
            buffer.append(buffer.getPointerRepository().getReservedIndex(p));
            buffer.getPointerRepository().clearReserved(p);
         } else
            buffer.append(buffer.getPointerRepository().add(p));

         if (PolymorphicSerializer::instance().isPolymorph(className)) {
            buffer.append(className);
            ptr->__serialize(buffer, version);
         } else
            serialize(*ptr, buffer, version);
      }
   }

   template <class Element_>
   void serialize(const boost::weak_ptr<Element_>& ptr, WriteBuffer& buffer, int version) {
      boost::shared_ptr<Element_> shared = ptr.lock();
      if (shared.get() == 0) {
         buffer.append((int)0);
         return;
      }

      void* p = reinterpret_cast<void*>(shared.get());
      std::string className = shared->__className();
      std::string staticClassName = Element_::__staticClassName();
      if (PolymorphicSerializer::instance().hasCast(staticClassName, className))
         p = PolymorphicSerializer::instance().getCast(staticClassName, className)(p);

      if (buffer.getPointerRepository().isRegistered(p))
         buffer.append(buffer.getPointerRepository().get(p));
      else
         buffer.append(buffer.getPointerRepository().reserveIndex(p));
   }

   template <class Element_>
   void serialize(const std::shared_ptr<Element_>& ptr, WriteBuffer& buffer, int version) {
      if (ptr.get() == 0) {
         buffer.append((int)0);
         return;
      }
      void* p = reinterpret_cast<void*>(ptr.get());
      std::string className = ptr->__className();
      std::string staticClassName = Element_::__staticClassName();

      if (PolymorphicSerializer::instance().hasCast(staticClassName, className))
         p = PolymorphicSerializer::instance().getCast(staticClassName, className)(p);

      if (buffer.getPointerRepository().isRegistered(p))
         buffer.append(buffer.getPointerRepository().get(p)); else {
         if (buffer.getPointerRepository().isReserved(p)) {
            buffer.append(buffer.getPointerRepository().getReservedIndex(p));
            buffer.getPointerRepository().clearReserved(p);
         }
         else
            buffer.append(buffer.getPointerRepository().add(p));

         if (PolymorphicSerializer::instance().isPolymorph(className)) {
            buffer.append(className);
            ptr->__serialize(buffer, version);
         }
         else
            serialize(*ptr, buffer, version);
      }
   }

   template <class Element_>
   void serialize(const std::weak_ptr<Element_>& ptr, WriteBuffer& buffer, int version) {
      std::shared_ptr<Element_> shared = ptr.lock();
      if (shared.get() == 0) {
         buffer.append((int)0);
         return;
      }

      void* p = reinterpret_cast<void*>(shared.get());
      std::string className = shared->__className();
      std::string staticClassName = Element_::__staticClassName();
      if (PolymorphicSerializer::instance().hasCast(staticClassName, className))
         p = PolymorphicSerializer::instance().getCast(staticClassName, className)(p);

      if (buffer.getPointerRepository().isRegistered(p))
         buffer.append(buffer.getPointerRepository().get(p));
      else
         buffer.append(buffer.getPointerRepository().reserveIndex(p));
   }

   template <class Element_>
   void serialize(const std::unique_ptr<Element_>& ptr, WriteBuffer& buffer, int version) {
      if (ptr.get() == 0) {
         buffer.append((int)0);
         return;
      }
      buffer.append((int)1);
      std::string className(ptr->__className());
      if (PolymorphicSerializer::instance().isPolymorph(className)) {
         buffer.append(className);
         ptr->__serialize(buffer, version);
      }
      else
         serialize(*ptr, buffer, version);
   }

   template <class Element_>
   void serialize(const std::auto_ptr<Element_>& ptr, WriteBuffer& buffer, int version) {
      if (ptr.get() == 0) {
         buffer.append((int)0);
         return;
      }
      buffer.append((int)1);
      std::string className(ptr->__className());
      if (PolymorphicSerializer::instance().isPolymorph(className)) {
         buffer.append(className);
         ptr->__serialize(buffer, version);
      }
      else
         serialize(*ptr, buffer, version);
   }

   template <class Element_>
   void serialize(Element_* ptr, WriteBuffer& buffer, int version) {
      if (ptr == 0) {
         buffer.append((int)0);
         return;
      }
      void* p = reinterpret_cast<void*>(ptr);
      std::string className = ptr->__className();
      std::string staticClassName = Element_::__staticClassName();
      if (PolymorphicSerializer::instance().hasCast(staticClassName, className))
         p = PolymorphicSerializer::instance().getCast(staticClassName, className)(p);

      if (buffer.getPointerRepository().isRegistered(p))
         buffer.append(buffer.getPointerRepository().get(p)); else {
         buffer.append(buffer.getPointerRepository().add(p));
         if (PolymorphicSerializer::instance().isPolymorph(className)) {
            buffer.append(className);
            ptr->__serialize(buffer, version);
         }
         else
            serialize(*ptr, buffer, version);
      }
   }

   template <class First_, class Second_>
   void serialize(const std::pair<First_, Second_>& obj, WriteBuffer& buffer, int version) {
      serialize(obj.first, buffer, version);
      serialize(obj.second, buffer, version);
   }

   template <class Number_>
   void serialize(const std::complex<Number_>& obj, WriteBuffer& buffer, int version) {
      serialize(obj.real(), buffer, version);
      serialize(obj.imag(), buffer, version);
   }

   template <class Number_>
   void serialize(const std::valarray<Number_>& obj, WriteBuffer& buffer, int version) {
      buffer.append(obj.size());
      for (size_t i = 0; i < obj.size(); ++i)
         buffer.append(obj[i]);
   }

   template <>
   void serialize(const bool& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const char& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const short& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const int& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const long& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const long long& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const unsigned char& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const unsigned short& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const unsigned int& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const unsigned long& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const unsigned long long& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const float& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const double& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const std::string& value, WriteBuffer& buffer, int version) {
      buffer.append(value);
   }

   template <>
   void serialize(const std::wstring& value, WriteBuffer& buffer, int version) {
      buffer.append(boost::locale::conv::utf_to_utf<char>(value));
   }

} // namespace Private

} // namespace ctrl

#endif // SERIALIZE_H_

/*
 * Copyright (C) 2010, 2012, 2013, 2015 by Gerrit Daniels <gerrit.daniels@gmail.com>
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
