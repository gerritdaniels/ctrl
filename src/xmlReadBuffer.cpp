
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

#include <ctrl/buffer/xmlReadBuffer.h>
#include <ctrl/properties.h>

using namespace rapidxml;
using namespace ctrl;
using namespace ctrl::Private;

XmlReadBuffer::XmlReadBuffer(const std::string& data) : m_collectionStart(false),
		m_nextValueIsAttribute(false), m_skipNextFundamental(false) {
   m_document.parse<0>(const_cast<char*>(data.c_str()));
   xml_node<>* node = m_document.first_node("root");
   checkNonNull(node, "root");
   m_stack.push(node);
}

std::string XmlReadBuffer::unwindStack() {
   std::string context;
   while (!m_stack.empty()) {
      context.insert(0, m_stack.top()->name());
      m_stack.pop();
      if (!m_stack.empty()) {
         context.insert(0, ".");
      }
   }
   return context;
}

void XmlReadBuffer::checkNonNull(const rapidxml::xml_base<>* node, const std::string& name) {
   if (node == 0)  {
      throw ctrl::Exception("Corrupt data: no node with name '" + name + "' found (context: " + unwindStack() + ")");
   }
}

void XmlReadBuffer::enterObject(const Context& context) throw(Exception) {

}

void XmlReadBuffer::enterMember(const Context& context, const char* suggested) throw(Exception) {
   std::string name = m_util.preferedMemberName(context.getOwningMember(), suggested);

   if ( context.getOwningMember().getProperty<AsAttribute>() && !context.getOwningMember().isFundamental() &&
         !(context.getOwningMember().isMap() && context.getOwningMember().isMapKeyFundamental()) ) {
      throw Exception("Element is not a fundamental type so it can't be an attribute (context: " + unwindStack() + "." + name + ")");
   }
   if (context.getOwningMember().getProperty<ctrl::AsIdField>()) {
      if (!context.getOwningMember().isFundamental()) {
         throw Exception("Non fundamental type used as id field (context: " + unwindStack() + "." + name + ")");
      }
      m_skipNextFundamental = true;
   } else {
      if (context.getOwningMember().getProperty<AsAttribute>() && context.getOwningMember().isFundamental()) {
         m_nextValueIsAttribute = true;
         m_attributeName = name;
      } else {
         xml_node<> *node = m_stack.top()->first_node(name.c_str());
         checkNonNull(node, name);
         m_stack.push(node);
      }
   }
}

void XmlReadBuffer::leaveMember(const Context& context) throw(Exception) {
   m_skipNextFundamental = false;
   if (context.getOwningMember().getProperty<AsAttribute>()) {
      m_nextValueIsAttribute = false;
   } else {
      m_stack.pop();
   }
}

void XmlReadBuffer::leaveObject(const Context& context) throw(Exception) {

}

void XmlReadBuffer::enterIdField(const Context& context) throw(Exception) {
   MemberContext idFieldContext = context.getClassContext().getRootIdField().getMemberContext();
   std::string name;
   if (context.getClassContext().hasRootProperty<IdFieldName>()) {
      name = context.getClassContext().getRootProperty<IdFieldName>();
   } else {
      name = m_util.preferedMemberName(idFieldContext);
   }
   if (idFieldContext.getProperty<AsAttribute>()
          || context.getClassContext().getRootProperty<IdFieldAsAttribute>()) {
      m_nextValueIsAttribute = true;
      m_attributeName = name;
   } else {
      xml_node<> *node = m_stack.top()->first_node(name.c_str());
      checkNonNull(node, name);
      m_stack.push(node);
   }
}

bool XmlReadBuffer::isNullId(const Context& context) throw(Exception) {
   return m_stack.top()->first_node() == 0 && m_stack.top()->first_attribute() == 0;
}

void XmlReadBuffer::leaveIdField(const Context& context) throw(Exception) {
   MemberContext idFieldContext = context.getClassContext().getRootIdField().getMemberContext();
   if (idFieldContext.getProperty<AsAttribute>()
          || context.getClassContext().getRootProperty<IdFieldAsAttribute>()) {
      m_nextValueIsAttribute = false;
   } else {
      m_stack.pop();
   }
}

void XmlReadBuffer::enterCollection(const Context& context) throw(Exception) {
   m_collectionStart = true;
}

void XmlReadBuffer::enterMap(const Context& context) throw(Exception) {
   m_collectionStart = true;
}

void XmlReadBuffer::nextCollectionElement(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
      xml_node<> *node = m_stack.top()->first_node("item");
      checkNonNull(node, "item");
      m_stack.push(node);
   } else {
      xml_node<>* next = m_stack.top()->next_sibling();
      checkNonNull(next, "item");
      m_stack.pop();
      m_stack.push(next);
   }
}

void XmlReadBuffer::enterKey(const Context& context) throw(Exception) {
   if (context.getOwningMember().getProperty<AsAttribute>()) {
      m_nextValueIsAttribute = true;
      m_attributeName = "key";
   } else {
      xml_node<> *node = m_stack.top()->first_node("key");
      checkNonNull(node, "key");
      m_stack.push(node);
   }
}

void XmlReadBuffer::leaveKey(const Context& context) throw(Exception) {
   if (context.getOwningMember().getProperty<AsAttribute>()) {
      m_nextValueIsAttribute = false;
   } else {
      m_stack.pop();
   }
}

void XmlReadBuffer::enterValue(const Context& context) throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node("value");
   checkNonNull(node, "value");
   m_stack.push(node);
}

void XmlReadBuffer::leaveValue(const Context& context) throw(Exception) {
   m_stack.pop();
}

void XmlReadBuffer::leaveCollection(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlReadBuffer::leaveMap(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlReadBuffer::readVersion(int& version, const Context& context) throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node("__version");
   checkNonNull(node, "__version");
   readNodeOrAttributeValue(node, version);
}

void XmlReadBuffer::readBits(char* data, long int length, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) {
      std::string bits;
      readNodeOrAttributeValue(m_stack.top(), bits);
      if (bits.length() != length) {
         throw ctrl::Exception("Number of bits doesn't match (context: " + unwindStack() + ")");
      }

      size_t nbChars = length / 8 + (length % 8 == 0 ? 0 : 1);
      for (size_t i = 0; i < nbChars; ++i)
         data[i] = 0;

      unsigned char mask = 0x80;
      for (size_t i = 0; i < length; ++i) {
         if (bits.substr(i, 1) == "1")
            data[i / 8] = data[i / 8] | mask;
         if (mask == 0x01)
            mask = 0x80;
         else
            mask = mask >> 1;
      }
   }
}

void XmlReadBuffer::readCollectionSize(std::size_t& size, const Context& context) throw(Exception) {
   xml_node<>* node = m_stack.top()->first_node();
   size = 0;
   while (node != 0) {
      ++size;
      node = node->next_sibling();
   }
}

void XmlReadBuffer::readTypeId(std::string& val, const Context& context) throw(Exception) {
   const char* field = m_document.allocate_string(context.getClassContext().getRootProperty<ctrl::TypeIdFieldName>().c_str());
   if (context.getClassContext().getRootProperty<TypeIdFieldAsAttribute>()) {
      xml_attribute<>* attr = m_stack.top()->first_attribute(field);
      checkNonNull(attr, field);
      val = attr->value();
   } else {
      xml_node<> *node = m_stack.top()->first_node(field);
      checkNonNull(node, field);
      val = node->value();
   }
}

void XmlReadBuffer::read(bool& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue(m_stack.top(), val);
}

void XmlReadBuffer::read(char& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<char>(m_stack.top(), val);
}

void XmlReadBuffer::read(short& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<short>(m_stack.top(), val);
}

void XmlReadBuffer::read(int& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<int>(m_stack.top(), val);
}

void XmlReadBuffer::read(long& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<long>(m_stack.top(), val);
}

void XmlReadBuffer::read(long long& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<long long>(m_stack.top(), val);
}

void XmlReadBuffer::read(unsigned char& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<unsigned char>(m_stack.top(), val);
}

void XmlReadBuffer::read(unsigned short& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<unsigned short>(m_stack.top(), val);
}

void XmlReadBuffer::read(unsigned int& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<unsigned int>(m_stack.top(), val);
}

void XmlReadBuffer::read(unsigned long& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<unsigned long>(m_stack.top(), val);
}

void XmlReadBuffer::read(unsigned long long& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<unsigned long long>(m_stack.top(), val);
}

void XmlReadBuffer::read(float& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<float>(m_stack.top(), val);
}

void XmlReadBuffer::read(double& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue<double>(m_stack.top(), val);
}

void XmlReadBuffer::read(std::string& val, const Context& context) throw(Exception) {
   readNodeOrAttributeValue(m_stack.top(), val);
}
