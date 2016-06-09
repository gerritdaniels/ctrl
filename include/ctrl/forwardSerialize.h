
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

#ifndef FORWARDSERIALIZE_H_
#define FORWARDSERIALIZE_H_

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

namespace ctrl {

class AbstractWriteBuffer;

class Context;

namespace Private {

   template <class ConcreteClass_>
   void serialize(const ConcreteClass_& object, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <size_t size_>
   void serialize(const std::bitset<size_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, class Alloc_>
   void serialize(const std::deque<Element_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, class Alloc_>
   void serialize(const std::list<Element_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void serialize(const std::map<Key_, Value_, Comp_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Value_, class Comp_, class Alloc_>
   void serialize(const std::multimap<Key_, Value_, Comp_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, class Container_>
   void serialize(const std::queue<Element_, Container_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, class Container_, class Comp_>
   void serialize(const std::priority_queue<Element_, Container_, Comp_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Comp_, class Alloc_>
   void serialize(const std::set<Key_, Comp_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Comp_, class Alloc_>
   void serialize(const std::multiset<Key_, Comp_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, class Container_>
   void serialize(const std::stack<Element_, Container_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, class Alloc_>
   void serialize(const std::vector<Element_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, size_t size_>
   void serialize(const std::array<Element_, size_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_, class Alloc_>
   void serialize(const std::forward_list<Element_, Alloc_>& elements, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                 , AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>& elements
                 , AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_set<Key_, Hash_, Pred_, Alloc_>& elements
                 , AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   void serialize( const std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>& elements
                 , AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_>
   void serialize(const boost::shared_ptr<Element_>& ptr, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_>
   void serialize(const boost::weak_ptr<Element_>& ptr, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_>
   void serialize(const std::shared_ptr<Element_>& ptr, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_>
   void serialize(const std::weak_ptr<Element_>& ptr, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_>
   void serialize(const std::auto_ptr<Element_>& ptr, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_>
   void serialize(const std::unique_ptr<Element_>& ptr, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Element_>
   void serialize(Element_* ptr, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class First_, class Second_>
   void serialize(const std::pair<First_, Second_>& obj, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Number_>
   void serialize(const std::complex<Number_>& obj, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <class Number_>
   void serialize(const std::valarray<Number_>& obj, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const bool& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const char& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const short& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const int& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const long& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const long long& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const unsigned char& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const unsigned short& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const unsigned int& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const unsigned long& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const unsigned long long& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const float& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const double& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const std::string& value, AbstractWriteBuffer& buffer, int version, const Context& context);

   template <>
   void serialize(const std::wstring& value, AbstractWriteBuffer& buffer, int version, const Context& context);

} // namespace Private

} // namespace ctrl

#endif // FORWARDSERIALIZE_H_
