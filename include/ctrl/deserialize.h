#ifndef DESERIALIZE_H_
#define DESERIALIZE_H_

#include <boost/locale.hpp>
#include <ctrl/forwardDeserialize.h>
#include <ctrl/classSerializer.h>
#include <ctrl/baseClassSerializer.h>
#include <ctrl/polymorphicSerializer.h>
#include <ctrl/readBufferImpl.h>
#include <ctrl/platformFormat.h>
#include <ctrl/exception.h>

namespace ctrl {

template <class ConcreteClass_, int alignment_, int endian_>
ConcreteClass_* deserialize(const char* bytes, const long& length, int version = 1) throw(Exception) {
   ConcreteClass_* ptr;
   try {
      ptr = new ConcreteClass_();
      Private::ReadBuffer buffer(
               new Private::ReadBufferImpl<alignment_, endian_>(bytes, length));
      int dataVersion;
      buffer.read(dataVersion);
      if (dataVersion != version)
         throw Exception("deserialize: version mismatch");
      Private::deserialize(*ptr, buffer, version);
      if (!buffer.reachedEnd())
         throw Exception("Input data is corrupt");
      return ptr;
   }
   catch(...) { delete ptr; throw; }
}

template <class ConcreteClass_, int alignment_>
ConcreteClass_* deserialize(const char* bytes, const long& length, int version = 1) throw(Exception) {
   return deserialize<ConcreteClass_, alignment_, CTRL_BYTE_ORDER>(bytes, length, version);
}

template <class ConcreteClass_>
ConcreteClass_* deserialize(const char* bytes, const long& length, int version = 1) throw(Exception) {
   return deserialize<ConcreteClass_, CTRL_MEMORY_ALIGNMENT, CTRL_BYTE_ORDER>(bytes, length, version);
}

namespace Private {

   template <class ConcreteClass_>
   void deserialize(ConcreteClass_& value, ReadBuffer& buffer, int version) throw(Exception) {
      typedef typename ConcreteClass_::__BaseClasses TList;
      BaseClassSerializer<ConcreteClass_>::deserialize(value, TList(), buffer, version);

      typedef typename ConcreteClass_::__MemberIndices Indices;
      ClassSerializer<ConcreteClass_>::deserialize(value, Indices(), buffer, version);

      ConcreteClass_::__initialize(value, version);
   }

   template <size_t size_>
   void deserialize(std::bitset<size_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      size_t nbChars = size_ / 8 + (size_ % 8 == 0 ? 0 : 1);
      char bits[nbChars];
      buffer.read(bits, nbChars);

      unsigned char mask = 0x80;
      for (size_t i = 0; i < size_; ++i) {
         if ((bits[i / 8] & mask) != 0)
            elements.set(i);
         if (mask == 0x01)
            mask = 0x80;
         else
            mask = mask >> 1;
      }
   }

   template <class Element_, class Alloc_>
   void deserialize(std::deque<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::deque<Element_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::deque<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         deserialize(el, buffer, version);
         elements.push_back(std::move(el));
      }
   }

   template <class Element_, class Alloc_>
   void deserialize(std::list<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::list<Element_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::list<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         deserialize(el, buffer, version);
         elements.push_back(std::move(el));
      }
   }

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void deserialize(std::map<Key_, Value_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::map<Key_, Value_, Comp_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::map<Key_, Value_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ key;
         deserialize(key, buffer, version);
         Value_ val;
         deserialize(val, buffer, version);
         elements[std::move(key)] = std::move(val);
      }
   }

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void deserialize(std::multimap<Key_, Value_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::multimap<Key_, Value_, Comp_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::multimap<Key_, Value_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ key;
         deserialize(key, buffer, version);
         Value_ val;
         deserialize(val, buffer, version);
         elements.insert( std::move(std::pair<Key_, Value_>(std::move(key), std::move(val))) );
      }
   }

   template <class Element_, class Container_>
   void deserialize(std::queue<Element_, Container_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::queue<Element_, Container_>::size_type size;
      buffer.read(size);
      for (typename std::queue<Element_, Container_>::size_type i = 0; i < size; ++i) {
         Element_ element;
         deserialize(element, buffer, version);
         elements.push(std::move(element));
      }
   }

   template <class Element_, class Container_, class Comp_>
   void deserialize(std::priority_queue<Element_, Container_, Comp_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::priority_queue<Element_, Container_, Comp_>::size_type size;
      buffer.read(size);
      for (typename std::priority_queue<Element_, Container_, Comp_>::size_type i = 0; i < size; ++i) {
         Element_ element;
         deserialize(element, buffer, version);
         elements.push(std::move(element));
      }
   }

   template <class Key_, class Comp_, class Alloc_>
   void deserialize(std::set<Key_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::set<Key_, Comp_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::set<Key_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         deserialize(element, buffer, version);
         elements.insert(std::move(element));
      }
   }

   template <class Key_, class Comp_, class Alloc_>
   void deserialize(std::multiset<Key_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::multiset<Key_, Comp_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::multiset<Key_, Comp_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         deserialize(element, buffer, version);
         elements.insert(std::move(element));
      }
   }

   template <class Element_, class Container_>
   void deserialize(std::stack<Element_, Container_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::stack<Element_, Container_>::size_type size;
      buffer.read(size);
      std::vector<Element_> temp;
      for (typename std::stack<Element_, Container_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         deserialize(el, buffer, version);
         temp.push_back(el);
      }
      for ( typename std::vector<Element_>::const_reverse_iterator iter = temp.rbegin();
            iter != temp.rend(); ++iter )
         elements.push(std::move(*iter));
   }

   template <class Element_, class Alloc_>
   void deserialize(std::vector<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::vector<Element_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::vector<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         deserialize(el, buffer, version);
         elements.push_back(std::move(el));
      }
   }

   template <class Element_, size_t size_>
   void deserialize(std::array<Element_, size_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::array<Element_, size_>::size_type size;
      buffer.read(size);
      for (typename std::array<Element_, size_>::size_type i = 0; i < size; ++i)
         deserialize(elements[i], buffer, version);
   }

   template <class Element_, class Alloc_>
   void deserialize(std::forward_list<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception) {
      typename std::forward_list<Element_, Alloc_>::size_type size;
      buffer.read(size);
      typename std::forward_list<Element_, Alloc_>::const_iterator iter = elements.before_begin();
      for (typename std::forward_list<Element_, Alloc_>::size_type i = 0; i < size; ++i) {
         Element_ el;
         deserialize(el, buffer, version);
         elements.insert_after(iter, std::move(el));
         ++iter;
      }
   }

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception) {
      typename std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ key;
         deserialize(key, buffer, version);
         Value_ val;
         deserialize(val, buffer, version);
         elements[std::move(key)] = std::move(val);
      }
   }

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception) {
      typename std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ key;
         deserialize(key, buffer, version);
         Value_ val;
         deserialize(val, buffer, version);
         elements.insert( std::move(std::pair<Key_, Value_>(std::move(key), std::move(val))) );
      }
   }

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_set<Key_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception) {
      typename std::unordered_set<Key_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::unordered_set<Key_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         deserialize(element, buffer, version);
         elements.insert(std::move(element));
      }
   }

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception) {
      typename std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>::size_type size;
      buffer.read(size);
      for (typename std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>::size_type i = 0; i < size; ++i) {
         Key_ element;
         deserialize(element, buffer, version);
         elements.insert(std::move(element));
      }
   }

   template <class Element_>
   void deserialize(boost::shared_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception) {
      int index;
      buffer.read(index);
      if (index == 0)
         return;

      boost::shared_ptr<void> voidPtr;
      std::string staticClassName(Element_::__staticClassName());

      if (buffer.getBoostPointerRepository().isRegistered(index)) {
         voidPtr = buffer.getBoostPointerRepository().get(index);
         void* p = voidPtr.get();

         std::string className(buffer.getBoostPointerRepository().getTypeName(index));
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);

         ptr = boost::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      } else {
         if (PolymorphicSerializer::instance().isPolymorph(staticClassName)) {
            std::string className;
            buffer.read(className);
            void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version);
            // we can't delete p as long as it is a void pointer
            if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
               p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);
            ptr = boost::shared_ptr<Element_>(reinterpret_cast<Element_*>(p));
         } else {
            ptr = boost::shared_ptr<Element_>(new Element_());
            deserialize(*ptr, buffer, version);
         }
         voidPtr = boost::shared_ptr<void>(ptr);
         buffer.getBoostPointerRepository().add(index, voidPtr, staticClassName);

         if (buffer.getBoostPointerRepository().hasDelayedAssignment(index))
            buffer.getBoostPointerRepository().doAssignment(index, voidPtr, staticClassName);
      }
   }

   template <class Element_>
   void deserialize(boost::weak_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception) {
      int index;
      buffer.read(index);
      if (index == 0)
         return;

      if (buffer.getBoostPointerRepository().isRegistered(index)) {
         boost::shared_ptr<void> voidPtr = buffer.getBoostPointerRepository().get(index);
         void* p = voidPtr.get();

         std::string className(buffer.getBoostPointerRepository().getTypeName(index));
         std::string staticClassName(Element_::__staticClassName());
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);

         ptr = boost::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      }
      else
         buffer.getBoostPointerRepository().delayAssignment(index, ptr);
   }

   template <class Element_>
   void deserialize(std::shared_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception) {
      int index;
      buffer.read(index);
      if (index == 0)
         return;

      std::shared_ptr<void> voidPtr;
      std::string staticClassName(Element_::__staticClassName());

      if (buffer.getStdPointerRepository().isRegistered(index)) {
         voidPtr = buffer.getStdPointerRepository().get(index);
         void* p = voidPtr.get();

         std::string className(buffer.getStdPointerRepository().getTypeName(index));
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);

         ptr = std::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      } else {
         if (PolymorphicSerializer::instance().isPolymorph(staticClassName)) {
            std::string className;
            buffer.read(className);
            void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version);
            // we can't delete p as long as it is a void pointer
            if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
               p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);
            ptr = std::shared_ptr<Element_>(reinterpret_cast<Element_*>(p));
         } else {
            ptr = std::shared_ptr<Element_>(new Element_());
            deserialize(*ptr, buffer, version);
         }
         voidPtr = std::shared_ptr<void>(ptr);
         buffer.getStdPointerRepository().add(index, voidPtr, staticClassName);

         if (buffer.getStdPointerRepository().hasDelayedAssignment(index))
            buffer.getStdPointerRepository().doAssignment(index, voidPtr, staticClassName);
      }
   }

   template <class Element_>
   void deserialize(std::weak_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception) {
      int index;
      buffer.read(index);
      if (index == 0)
         return;

      if (buffer.getStdPointerRepository().isRegistered(index)) {
         std::shared_ptr<void> voidPtr = buffer.getStdPointerRepository().get(index);
         void* p = voidPtr.get();

         std::string className(buffer.getStdPointerRepository().getTypeName(index));
         std::string staticClassName(Element_::__staticClassName());
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);

         ptr = std::shared_ptr<Element_>( voidPtr, reinterpret_cast<Element_*>(p) );
      }
      else
         buffer.getStdPointerRepository().delayAssignment<>(index, ptr);
   }

   template <class Element_>
   void deserialize(std::auto_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception) {
      int index;
      buffer.read(index);
      if (index == 0)
         return;

      std::string staticClassName(Element_::__staticClassName());
      if (PolymorphicSerializer::instance().isPolymorph(staticClassName)) {
         std::string className;
         buffer.read(className);
         void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version);
         // we can't delete p as long as it is a void pointer
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);
         ptr = std::auto_ptr<Element_>(reinterpret_cast<Element_*>(p));
      } else {
         ptr = std::auto_ptr<Element_>(new Element_());
         deserialize(*ptr, buffer, version);
      }
   }

   template <class Element_>
   void deserialize(std::unique_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception) {
      int index;
      buffer.read(index);
      if (index == 0)
         return;

      std::string staticClassName(Element_::__staticClassName());
      if (PolymorphicSerializer::instance().isPolymorph(staticClassName)) {
         std::string className;
         buffer.read(className);
         void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version);
         // we can't delete p as long as it is a void pointer
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);
         ptr = std::unique_ptr<Element_>(reinterpret_cast<Element_*>(p));
      } else {
         ptr = std::unique_ptr<Element_>(new Element_());
         deserialize(*ptr, buffer, version);
      }
   }

   template <class Element_>
   void deserialize(Element_*& ptr, ReadBuffer& buffer, int version) throw(Exception) {
      int index;
      buffer.read(index);
      if (index == 0)
         return;

      std::string staticClassName(Element_::__staticClassName());
      if (buffer.getRawPointerRepository().isRegistered(index)) {
         void* p = buffer.getRawPointerRepository().get(index);

         std::string className(buffer.getRawPointerRepository().getTypeName(index));
         if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
            p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);

         ptr = reinterpret_cast<Element_*>(p);
      } else {
         if (PolymorphicSerializer::instance().isPolymorph(staticClassName)) {
            std::string className;
            buffer.read(className);
            void* p = PolymorphicSerializer::instance().deserialize(className, buffer, version);
            if (PolymorphicSerializer::instance().hasCast(className, staticClassName))
               p = PolymorphicSerializer::instance().getCast(className, staticClassName)(p);
            ptr = reinterpret_cast<Element_*>(p);
         } else {
            ptr = new Element_();
            deserialize(*ptr, buffer, version);
         }
         buffer.getRawPointerRepository().add(index, reinterpret_cast<void*>(ptr), staticClassName);
      }
   }

   template <class First_, class Second_>
   void deserialize(std::pair<First_, Second_>& obj, ReadBuffer& buffer, int version) throw(Exception) {
      deserialize(obj.first, buffer, version);
      deserialize(obj.second, buffer, version);
   }

   template <class Number_>
   void deserialize(std::complex<Number_>& obj, ReadBuffer& buffer, int version) throw(Exception) {
      double real, imag;
      deserialize(real, buffer, version);
      deserialize(imag, buffer, version);
      obj = std::complex<Number_>(real, imag);
   }

   template <class Number_>
   void deserialize(std::valarray<Number_>& obj, ReadBuffer& buffer, int version) throw(Exception) {
      size_t size;
      buffer.read(size);
      obj = std::valarray<Number_>(size);
      for (size_t i = 0; i < size; ++i)
         deserialize(obj[i], buffer, version);
   }

   template <>
   void deserialize(bool& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(char& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(short& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(int& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(long& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(long long& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(unsigned char& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(unsigned short& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(unsigned int& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(unsigned long& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(unsigned long long& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(float& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(double& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(std::string& value, ReadBuffer& buffer, int version) throw(Exception) {
      buffer.read(value);
   }

   template <>
   void deserialize(std::wstring& value, ReadBuffer& buffer, int version) throw(Exception) {
      std::string tmp;
      buffer.read(tmp);
      value = boost::locale::conv::utf_to_utf<wchar_t>(tmp);

   }

} // namespace Private

} // namespace ctrl

#endif // DESERIALIZE_H_

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
