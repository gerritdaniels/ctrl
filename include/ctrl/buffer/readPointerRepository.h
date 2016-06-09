
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

#ifndef READPOINTERREPOSITORY_H
#define READPOINTERREPOSITORY_H

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <ctrl/buffer/weakPtrWrapper.h>
#include <ctrl/buffer/weakPtrWrapperImpl.h>
#include <ctrl/idField.h>

namespace ctrl {

namespace Private {

   template<template <class> class Shared_, template <class> class Weak_>
   class ReadPointerRepository {
   public:
      ReadPointerRepository() { }

      bool isRegistered(const IdField& idField) const {
          m_ptrs.find(idField) != m_ptrs.end();
      }

      Shared_<void> get(const IdField& idField) const {
         return m_ptrs.at(idField).first;
      }

      std::string getTypeName(const IdField& idField) const {
         return m_ptrs.at(idField).second;
      }

      void add(const IdField& idField, Shared_<void> p, const std::string& className) {
         m_ptrs[idField] = std::pair<Shared_<void>, std::string>(p, className);
      }

      bool hasDelayedAssignment(const IdField& idField) const {
         return m_delayed.find(idField) != m_delayed.end();
      }

      template <class Element_>
      void delayAssignment(const IdField& idField, Weak_<Element_>& ptr) {
         std::shared_ptr<typename WeakPtrWrapper<Shared_>::Impl> wrapperImpl(new WeakPtrWrapperImpl<Shared_, Weak_, Element_>(ptr));
         m_delayed[idField].push_back(WeakPtrWrapper<Shared_>(wrapperImpl));
      }

      void doAssignment(const IdField& idField, Shared_<void> voidPtr, std::string sharedName) {
         std::vector< WeakPtrWrapper<Shared_> >& weakWrappers(m_delayed.at(idField));

         for ( typename std::vector< WeakPtrWrapper<Shared_> >::iterator iter = weakWrappers.begin();
               iter != weakWrappers.end(); ++iter )
            iter->assign(voidPtr, sharedName);
      }


   private:
      std::unordered_map< IdField, std::pair<Shared_<void>, std::string> > m_ptrs;
      std::unordered_map< IdField, std::vector<WeakPtrWrapper<Shared_>> > m_delayed;
   };

} // namespace Private

} //namespace ctrl

#endif // READPOINTERREPOSITORY_H
