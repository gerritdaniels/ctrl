#ifndef WEAKPTRWRAPPER_H_
#define WEAKPTRWRAPPER_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace ctrl {

namespace Private {

   template<template <class> class Shared_>
   class WeakPtrWrapper {
   public:
      class Impl {
      public:
         Impl() {}
         virtual ~Impl() {}
         virtual void assign(const Shared_<void>& ptr, const std::string& sharedName) = 0;
      };

      WeakPtrWrapper(std::shared_ptr<Impl> pimpl) : m_pimpl(pimpl) { }
      WeakPtrWrapper(const WeakPtrWrapper<Shared_>& that) : m_pimpl(that.m_pimpl) { }

      void assign(Shared_<void> ptr, const std::string& sharedName) {
         m_pimpl->assign(ptr, sharedName);
      }

   private:
      std::shared_ptr<Impl> m_pimpl;
   };

} // namespace Private

} // namespace ctrl

#endif // WEAKPTRWRAPPER_H_

/*
 * Copyright (C) 2010, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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
