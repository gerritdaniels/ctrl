
/*
 * Copyright (C) 2010, 2012, 2015, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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

#include <ctrl/polymorphicSerializer.h>
#include <ctrl/buffer/abstractReadBuffer.h>
#include <iostream>
#include <algorithm>

using namespace ctrl;
using namespace ctrl::Private;

PolymorphicSerializer::PolymorphicSerializer() {

}

PolymorphicSerializer::~PolymorphicSerializer() {

}

PolymorphicSerializer& PolymorphicSerializer::instance() {
   static PolymorphicSerializer serializer;
   return serializer;
}

bool PolymorphicSerializer::isPolymorph(const std::string& className) const {
   return m_factories.find(className) != m_factories.end() ||
          std::find(m_abstractBaseClasses.begin(), m_abstractBaseClasses.end(), className) != m_abstractBaseClasses.end();
}

void* PolymorphicSerializer::deserialize(const std::string& className, AbstractReadBuffer& buffer, int version,
                                         const IdField& idField, const Context& context) const {
   return m_factories.at(className).deserialize(buffer, idField, className, version, context);
}

int PolymorphicSerializer::registerDeserialize( const std::string& className
                                              , const PolymorphicFactory& factory ) {
   m_factories[className] = factory;
   return 0;
}

int PolymorphicSerializer::registerAbstract(const std::string& className) {
   m_abstractBaseClasses.push_back(className);
   return 0;
}


int PolymorphicSerializer::registerCast( const std::string& from
                                       , const std::string& to
                                       , PolymorphicSerializer::CastFunction func) {
   m_casts[from][to] = func;
   return 0;
}

PolymorphicSerializer::CastFunction PolymorphicSerializer::getCast(
                                const std::string& from, const std::string& to) const {
   return m_casts.at(from).at(to);
}

bool PolymorphicSerializer::hasCast( const std::string& from
                                   , const std::string& to ) const {
   CastMap::const_iterator iter = m_casts.find(from);
   if (iter == m_casts.end())
      return false;
   return iter->second.find(to) != iter->second.end();
}

void* PolymorphicSerializer::cast(const std::string& from, const std::string& to, void* p) const {
   if (hasCast(from, to)) {
      return getCast(from, to)(p);
   } else {
      return p;
   }
}
