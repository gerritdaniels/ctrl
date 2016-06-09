
/*
 * Copyright (C) 2010, 2012, 2013, 2015, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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

#ifndef _POLYMORPHICSERIALIZER_H_
#define _POLYMORPHICSERIALIZER_H_

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <ctrl/polymorphicFactory.h>

namespace ctrl {

class Context;

class AbstractReadBuffer;

class IdField;

namespace Private {

   class PolymorphicSerializer {
   private:
      PolymorphicSerializer();

   public:
      ~PolymorphicSerializer();

      static PolymorphicSerializer& instance();

      bool isPolymorph(const std::string& className) const;

      int registerDeserialize(const std::string& className, const PolymorphicFactory& factory);
      int registerAbstract(const std::string& className);
      void* deserialize(const std::string& className, AbstractReadBuffer& buffer, int version,
                        const IdField& idField, const Context& context) const;

      typedef void* (*CastFunction)(void*);
      int registerCast(const std::string& from, const std::string& to, CastFunction func);
      CastFunction getCast(const std::string& from, const std::string& to) const;
      bool hasCast(const std::string& from, const std::string& to) const;
      void* cast(const std::string& from, const std::string& to, void* p) const;

   private:
      std::map<std::string, PolymorphicFactory> m_factories;
      std::vector<std::string> m_abstractBaseClasses;

      typedef std::map< std::string, std::map<std::string, CastFunction> > CastMap;
      CastMap m_casts;
   };

} // namespace Private

} // namespace ctrl

#endif // _POLYMORPHICSERIALIZER_H_
