
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

#ifndef TYPEMANIP_H_
#define TYPEMANIP_H_

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

namespace Private {

   template <class Head_, class Tail_>
   struct TypeList {
      typedef Head_ Head;
      typedef Tail_ Tail;
   };

   class NullType {};

   template <int i_>
   struct Int2Type {
      enum { value = i_ };
   };



   template <int condition_, class T0_, class T1_>
   struct Select {
      typedef T0_ Result;
   };

   template <class T0_, class T1_>
   struct Select<false, T0_, T1_> {
      typedef T1_ Result;
   };



   template <int condition_, int v0_, int v1_>
   struct SelectInt {
      enum { value = v0_ };
   };

   template <int v0_, int v1_>
   struct SelectInt<false, v0_, v1_> {
      enum { value = v1_ };
   };



   template <class T_>
   struct IsTypeList {
      enum { value = false };
   };

   template <class H_, class T_>
   struct IsTypeList<TypeList<H_, T_>> {
      enum { value = true };
   };



   template <class T_>
   struct IsNullType {
      enum { value = false };
   };

   template <>
   struct IsNullType<NullType> {
      enum { value = true };
   };



   template <class T1_, class T2_>
   struct IsSameType {
      enum { value = false };
   };

   template <class T_>
   struct IsSameType<T_, T_> {
      enum { value = true };
   };



   template <class T_, class TList_>
   struct TypeListContains {
      enum { value = false };
   };

   template <class T_>
   struct TypeListContains<T_, T_> {
      enum { value = true };
   };

   template <class T_>
   struct TypeListContains<T_, NullType> {
      enum { value = false };
   };

   template <class T_, class Head_, class Tail_>
   struct TypeListContains<T_, TypeList<Head_, Tail_>> {
      enum { value = SelectInt<IsSameType<T_, Head_>::value, true, TypeListContains<T_, Tail_>::value>::value };
   };



   template <class TList_>
   struct ExctractSingleton {
      typedef TList_ Result;
   };

   template <class T_>
   struct ExctractSingleton<TypeList<T_, NullType>> {
      typedef typename Select<IsTypeList<T_>::value, TypeList<T_, NullType>, T_>::Result Result;
   };



   template <class T_>
   struct UniquifyTypeList {
      typedef T_ Result;
   };

   template <class Head_, class Tail_>
   struct UniquifyTypeList<TypeList<Head_, Tail_>> {
      typedef typename UniquifyTypeList<Tail_>::Result NewTList;

      typedef typename Select<TypeListContains<Head_, NewTList>::value, NewTList, TypeList<Head_, NewTList>>::Result Tmp;

      typedef typename ExctractSingleton<Tmp>::Result Result;
   };

   template <class T_>
   struct UniquifyTypeList<TypeList<T_, NullType>> {
      typedef TypeList<T_, NullType> Result;
   };



   template <class ConcreteClass_>
   struct SimplifyTypeList {
      typedef ConcreteClass_ Result;
   };

   template <class Next_, class Previous_>
   struct SimplifyContinue {
      typedef typename SimplifyTypeList<Next_>::Result Result;
   };

   template <class T_>
   struct SimplifyContinue<T_, T_> {
      typedef T_ Result;
   };

   template <class Head_>
   struct SimplifyTypeList<TypeList<Head_, NullType>> {
      typedef typename SimplifyTypeList<Head_>::Result Result;
   };

   template <class Head_, class Tail_>
   struct SimplifyTypeList<TypeList<Head_, Tail_>> {
      typedef typename SimplifyTypeList<Head_>::Result NewHead;
      typedef typename SimplifyTypeList<typename Tail_::Head>::Result NewTail;

      typedef TypeList<NewHead, TypeList<NewTail, typename Tail_::Tail>> Next;

      typedef typename SimplifyContinue<Next, TypeList<Head_, Tail_>>::Result Result;
   };



   template <class T_>
   struct IsFundamental { enum { value = false }; };

   template <>
   struct IsFundamental<bool> { enum { value = true }; };

   template <>
   struct IsFundamental<char> { enum { value = true }; };

   template <>
   struct IsFundamental<short> { enum { value = true }; };

   template <>
   struct IsFundamental<int> { enum { value = true }; };

   template <>
   struct IsFundamental<long> { enum { value = true }; };

   template <>
   struct IsFundamental<long long> { enum { value = true }; };

   template <>
   struct IsFundamental<unsigned char> { enum { value = true }; };

   template <>
   struct IsFundamental<unsigned short> { enum { value = true }; };

   template <>
   struct IsFundamental<unsigned int> { enum { value = true }; };

   template <>
   struct IsFundamental<unsigned long> { enum { value = true }; };

   template <>
   struct IsFundamental<unsigned long long> { enum { value = true }; };

   template <>
   struct IsFundamental<float> { enum { value = true }; };

   template <>
   struct IsFundamental<double> { enum { value = true }; };

   template <>
   struct IsFundamental<std::string> { enum { value = true }; };

   template <>
   struct IsFundamental<std::wstring> { enum { value = true }; };

   template <size_t size_>
   struct IsFundamental<std::bitset<size_>> { enum { value = true }; };



   template <class T_>
   struct IsPointer { enum { value = false }; };

   template <class Element_>
   struct IsPointer<boost::shared_ptr<Element_>> { enum { value = true }; };

   template <class Element_>
   struct IsPointer<boost::weak_ptr<Element_>> { enum { value = true }; };

   template <class Element_>
   struct IsPointer<std::shared_ptr<Element_>> { enum { value = true }; };

   template <class Element_>
   struct IsPointer<std::weak_ptr<Element_>> { enum { value = true }; };

   template <class Element_>
   struct IsPointer<std::auto_ptr<Element_>> { enum { value = true }; };

   template <class Element_>
   struct IsPointer<std::unique_ptr<Element_>> { enum { value = true }; };

   template <class Element_>
   struct IsPointer<Element_*> { enum { value = true }; };



   template <class T_>
   struct IsCollection {
      enum { value = false };
   };

   template <class Element_, class Alloc_>
   struct IsCollection<std::deque<Element_, Alloc_>> { enum { value = true }; };

   template <class Element_, class Alloc_>
   struct IsCollection<std::list<Element_, Alloc_>> { enum { value = true }; };

   template <class Element_, class Container_>
   struct IsCollection<std::queue<Element_, Container_>> { enum { value = true }; };

   template <class Element_, class Container_, class Comp_>
   struct IsCollection<std::priority_queue<Element_, Container_, Comp_>> { enum { value = true }; };

   template <class Key_, class Comp_, class Alloc_>
   struct IsCollection<std::set<Key_, Comp_, Alloc_>> { enum { value = true }; };

   template <class Key_, class Comp_, class Alloc_>
   struct IsCollection<std::multiset<Key_, Comp_, Alloc_>> { enum { value = true }; };

   template <class Element_, class Container_>
   struct IsCollection<std::stack<Element_, Container_>> { enum { value = true }; };

   template <class Element_, class Alloc_>
   struct IsCollection<std::vector<Element_, Alloc_>> { enum { value = true }; };

   template <class Element_, size_t size_>
   struct IsCollection<std::array<Element_, size_>> { enum { value = true }; };

   template <class Element_, class Alloc_>
   struct IsCollection<std::forward_list<Element_, Alloc_>> { enum { value = true }; };

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   struct IsCollection<std::unordered_set<Key_, Hash_, Pred_, Alloc_>> { enum { value = true }; };

   template <class Key_, class Hash_, class Pred_, class Alloc_>
   struct IsCollection<std::unordered_multiset<Key_, Hash_, Pred_, Alloc_>> { enum { value = true }; };

   template <class Number_>
   struct IsCollection<std::valarray<Number_>> { enum { value = true }; };



   template <class T_>
   struct IsMap { enum { value = false, keyFundamental = false }; };

   template <class Key_, class Value_, class Comp_, class Alloc_>
   struct IsMap<std::map<Key_, Value_, Comp_, Alloc_>> { enum { value = true, keyFundamental = IsFundamental<Key_>::value }; };

   template <class Key_, class Value_, class Comp_, class Alloc_>
   struct IsMap<std::multimap<Key_, Value_, Comp_, Alloc_>> { enum { value = true, keyFundamental = IsFundamental<Key_>::value }; };

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   struct IsMap<std::unordered_map<Key_, Value_, Hash_, Pred_, Alloc_>> { enum { value = true, keyFundamental = IsFundamental<Key_>::value }; };

   template <class Key_, class Value_, class Hash_, class Pred_, class Alloc_>
   struct IsMap<std::unordered_multimap<Key_, Value_, Hash_, Pred_, Alloc_>> { enum { value = true, keyFundamental = IsFundamental<Key_>::value }; };



   template <class T_>
   struct IsPair {
      enum { value = false };
   };

   template <class First_, class Second_>
   struct IsPair<std::pair<First_, Second_>> {
      enum { value = true };
   };



   template <class T_>
   struct IsComplex {
      enum { value = false };
   };

   template <class Number_>
   struct IsComplex<std::complex<Number_>> {
      enum { value = true };
   };

} // namespace Private

} // namespace ctrl

#endif // TYPEMANIP_H_
