#ifndef FORWARDDESERIALIZE_H_
#define FORWARDDESERIALIZE_H_

#include <bitset>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <complex>
#include <valarray>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <ctrl/readBuffer.h>
#include <ctrl/exception.h>

namespace ctrl {

namespace Private {

   template <class ConcreteClass_>
   void deserialize(ConcreteClass_& value, ReadBuffer& buffer, int version) throw(Exception);

   template <size_t size_>
   void deserialize(std::bitset<size_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, class Alloc_>
   void deserialize(std::deque<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, class Alloc_>
   void deserialize(std::list<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void deserialize(std::map<Key_, Value_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void deserialize(std::multimap<Key_, Value_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, class Container_>
   void deserialize(std::queue<Element_, Container_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, class Container_, class Comp_>
   void deserialize(std::priority_queue<Element_, Container_, Comp_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Key_, class Comp_, class Alloc_>
   void deserialize(std::set<Key_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Key_, class Comp_, class Alloc_>
   void deserialize(std::multiset<Key_, Comp_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, class Container_>
   void deserialize(std::stack<Element_, Container_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, class Alloc_>
   void deserialize(std::vector<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, size_t size_>
   void deserialize(std::array<Element_, size_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_, class Alloc_>
   void deserialize(std::forward_list<Element_, Alloc_>& elements, ReadBuffer& buffer, int version) throw(Exception);

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception);

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception);

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_set<Key_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception);

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void deserialize( std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>& elements
                   , ReadBuffer& buffer, int version ) throw(Exception);

   template <class Element_>
   void deserialize(boost::shared_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_>
   void deserialize(boost::weak_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_>
   void deserialize(std::shared_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_>
   void deserialize(std::weak_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_>
   void deserialize(std::auto_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_>
   void deserialize(std::unique_ptr<Element_>& ptr, ReadBuffer& buffer, int version) throw(Exception);

   template <class Element_>
   void deserialize(Element_*& ptr, ReadBuffer& buffer, int version) throw(Exception);

   template <class First_, class Second_>
   void deserialize(std::pair<First_, Second_>& obj, ReadBuffer& buffer, int version) throw(Exception);

   template <class Number_>
   void deserialize(std::complex<Number_>& obj, ReadBuffer& buffer, int version) throw(Exception);

   template <class Number_>
   void deserialize(std::valarray<Number_>& obj, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(bool& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(char& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(short& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(int& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(long& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(long long& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(unsigned char& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(unsigned short& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(unsigned int& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(unsigned long& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(unsigned long long& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(float& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(double& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(std::string& value, ReadBuffer& buffer, int version) throw(Exception);

   template <>
   void deserialize(std::wstring& value, ReadBuffer& buffer, int version) throw(Exception);

} // namespace Private

} // namespace ctrl

#endif // FORWARDDESERIALIZE_H_

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
