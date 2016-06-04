#ifndef READPOINTERREPOSITORY_H
#define READPOINTERREPOSITORY_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <ctrl/weakPtrWrapper.h>
#include <ctrl/weakPtrWrapperImpl.h>

namespace ctrl {

namespace Private {

   template<template <class> class Shared_, template <class> class Weak_>
   class ReadPointerRepository {
   public:
      ReadPointerRepository() { }

      bool isRegistered(const int& index) const {
          m_ptrs.find(index) != m_ptrs.end();
      }

      Shared_<void> get(const int& index) const {
         return m_ptrs.at(index).first;
      }

      std::string getTypeName(const int& index) const {
         return m_ptrs.at(index).second;
      }

      void add(const int& index, Shared_<void> p, const std::string& className) {
         m_ptrs[index] = std::pair<Shared_<void>, std::string>(p, className);
      }

      bool hasDelayedAssignment(const int& index) const {
         return m_delayed.find(index) != m_delayed.end();
      }

      template <class Element_>
      void delayAssignment(const int& index, Weak_<Element_>& ptr) {
         std::shared_ptr<typename WeakPtrWrapper<Shared_>::Impl> wrapperImpl(new WeakPtrWrapperImpl<Shared_, Weak_, Element_>(ptr));
         m_delayed[index].push_back(WeakPtrWrapper<Shared_>(wrapperImpl));
      }

      void doAssignment(const int& index, Shared_<void> voidPtr, std::string sharedName) {
         std::vector< WeakPtrWrapper<Shared_> >& weakWrappers(m_delayed.at(index));

         for ( typename std::vector< WeakPtrWrapper<Shared_> >::iterator iter = weakWrappers.begin();
               iter != weakWrappers.end(); ++iter )
            iter->assign(voidPtr, sharedName);
      }


   private:
      std::map< int, std::pair<Shared_<void>, std::string> > m_ptrs;
      std::map< int, std::vector<WeakPtrWrapper<Shared_> > > m_delayed;
   };

} // namespace Private

} //namespace ctrl

#endif // READPOINTERREPOSITORY_H

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
