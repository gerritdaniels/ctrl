
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

#ifndef IDFIELD_H_
#define IDFIELD_H_

#include <ctrl/typemanip.h>
#include <ctrl/properties.h>
#include <ctrl/derivationRoots.h>
#include <ctrl/memberContext.h>

namespace ctrl {

class AbstractReadBuffer;

class AbstractWriteBuffer;

class Context;

class IdField {
public:
   struct Impl {
      virtual bool isDefault() const = 0;
      virtual void setDefaultValue(void* val) = 0;
      virtual void write(void* p, const std::string& dynamicName, AbstractWriteBuffer& buffer, const Context& context) = 0;
      virtual void read(AbstractReadBuffer& buffer, const Context& context) = 0;
      virtual void assign(void* p, const std::string& dynamicName) = 0;
      virtual bool operator==(const Impl& that) const = 0;
      virtual const void* value() const = 0;
      virtual const std::type_info& rootType() const = 0;
      virtual bool isNull() const = 0;
      virtual MemberContext getMemberContext() const = 0;
      virtual size_t hash() const = 0;
   };

   IdField(Impl* pimpl) : m_pimpl(pimpl) {}

   IdField(const IdField& that) : m_pimpl(that.m_pimpl) {}

   bool isDefault() const {
      return m_pimpl->isDefault();
   }

   void setDefaultValue(int val) {
      m_pimpl->setDefaultValue(reinterpret_cast<void*>(&val));
   }

   void write(void* p, const std::string& dynamicName, AbstractWriteBuffer& buffer, const Context& context) {
      m_pimpl->write(p, dynamicName, buffer, context);
   }

   void read(AbstractReadBuffer& buffer, const Context& context) {
      m_pimpl->read(buffer, context);
   }

   void assign(void* p, const std::string& dynamicName) const {
      m_pimpl->assign(p, dynamicName);
   }

   bool operator==(const IdField& that) const {
      return m_pimpl->operator==(*that.m_pimpl);
   }

   bool isNull() const {
      return m_pimpl->isNull();
   }

   MemberContext getMemberContext() const {
      return m_pimpl->getMemberContext();
   }

   std::size_t hash() const {
      return m_pimpl->hash();
   }

private:
   std::shared_ptr<Impl> m_pimpl;

};

} // namespace ctrl

namespace std {
template <>

class hash<ctrl::IdField> {
 public:
  size_t operator()(const ctrl::IdField& field) const
  {
    return field.hash();
  }
};

}

#endif // IDFIELD_H_
