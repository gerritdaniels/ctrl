
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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <ctrl/memberContext.h>
#include <ctrl/classContext.h>
#include <memory>
#include <set>

namespace ctrl {

class Context
{
public:
   Context(ClassContext classContext)
         : m_owningMember(0), m_classContext(classContext),
         m_obligatedSingleRoots(new std::set<const std::type_info*>()) {

   }

   Context(const Context& that)
         : m_owningMember(that.m_owningMember),
         m_classContext(that.m_classContext),
         m_obligatedSingleRoots(that.m_obligatedSingleRoots) {

   }

   Context(const Context& that, MemberContext owningMember)
         : m_owningMember(owningMember),
         m_classContext(that.m_classContext),
         m_obligatedSingleRoots(that.m_obligatedSingleRoots) {

   }

   Context(const Context& that, ClassContext classContext)
         : m_owningMember(that.m_owningMember),
         m_classContext(classContext),
         m_obligatedSingleRoots(that.m_obligatedSingleRoots) {
      for (std::set<const std::type_info*>::const_iterator iter = m_obligatedSingleRoots->begin();
           iter != m_obligatedSingleRoots->end(); ++iter) {
         if (m_classContext.isSingleRootConflict(*iter)) {
            throw Exception("Single root property requested for multiply inherited class: " + m_classContext.getName());
         }
      }
   }

   ~Context() {
      if (m_classContext.singleRootPropertyRequested()) {
         m_obligatedSingleRoots->insert(m_classContext.getSingleRootId());
      }
   }

   const MemberContext& getOwningMember() const {
      return m_owningMember;
   }

   const ClassContext& getClassContext() const {
      return m_classContext;
   }

private:
   MemberContext m_owningMember;
   ClassContext m_classContext;

   std::shared_ptr<std::set<const std::type_info*>> m_obligatedSingleRoots;
};

}

#endif // CONTEXT_H
