
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

#include <climits>
#include <ctrl/buffer/xmlWriteBuffer.h>
#include <ctrl/rapidxml/rapidxml_print.hpp>
#include <ctrl/properties.h>

using namespace rapidxml;
using namespace ctrl;
using namespace ctrl::Private;

XmlWriteBuffer::XmlWriteBuffer(bool prettyPrint) : m_collectionStart(false),
         m_nextValueIsAttribute(false),
         m_skipNextFundamental(false),
         m_prettyPrint(prettyPrint) {
   xml_node<> *node = m_document.allocate_node(node_element, "root");
   m_document.append_node(node);
   m_stack.push(node);
}

std::string XmlWriteBuffer::unwindStack() {
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


void XmlWriteBuffer::enterObject(const Context& context) throw(Exception) {

}

void XmlWriteBuffer::enterMember(const Context& context, const char* suggested) throw(Exception) {
   const char* name = m_document.allocate_string(m_util.preferedMemberName(context.getOwningMember(), suggested).c_str());

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
         xml_node<> *node = m_stack.empty() ? 0 : m_stack.top()->first_node(name);
         if (node == 0) {
            node = m_document.allocate_node(node_element, name);
            m_stack.top()->append_node(node);
         }
         m_stack.push(node);
      }
   }
}

void XmlWriteBuffer::leaveMember(const Context& context) throw(Exception) {
   if (context.getOwningMember().getProperty<ctrl::AsIdField>()) {
      m_skipNextFundamental = false;
   } else {
      if (context.getOwningMember().getProperty<AsAttribute>()) {
         m_nextValueIsAttribute = false;
      } else {
         m_stack.pop();
      }
   }
}

void XmlWriteBuffer::leaveObject(const Context& context) throw(Exception) {

}

void XmlWriteBuffer::enterIdField(const Context& context) throw(Exception) {
   MemberContext idFieldContext = context.getClassContext().getRootIdField().getMemberContext();
   std::string name;
   if (context.getClassContext().hasRootProperty<IdFieldName>()) {
      name = context.getClassContext().getRootProperty<IdFieldName>();
   } else {
      name = m_util.preferedMemberName(idFieldContext);
   }
   if (idFieldContext.getProperty<AsAttribute>() || context.getClassContext().getRootProperty<IdFieldAsAttribute>()) {
      m_nextValueIsAttribute = true;
      m_attributeName = m_document.allocate_string(name.c_str());
   } else {
      xml_node<>* node = m_document.allocate_node(node_element, m_document.allocate_string(name.c_str()));
      m_stack.top()->append_node(node);
      m_stack.push(node);
   }
}

void XmlWriteBuffer::appendNullId(const Context& context) throw(Exception) {

}

void XmlWriteBuffer::appendNonNullId(const Context& context) throw(Exception) {

}

void XmlWriteBuffer::leaveIdField(const Context& context) throw(Exception) {
   if (context.getClassContext().getRootIdField().getMemberContext().getProperty<AsAttribute>()
          || context.getClassContext().getRootProperty<IdFieldAsAttribute>()) {
      m_nextValueIsAttribute = false;
   } else {
      m_stack.pop();
   }
}

void XmlWriteBuffer::enterCollection(const Context& context) throw(Exception) {
   m_collectionStart = true;
}

void XmlWriteBuffer::enterMap(const Context& context) throw(Exception) {
   m_collectionStart = true;
}

void XmlWriteBuffer::nextCollectionElement(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
   xml_node<> *node = m_document.allocate_node(node_element, "item");
   m_stack.top()->append_node(node);
   m_stack.push(node);
}

void XmlWriteBuffer::enterKey(const Context& context) throw(Exception) {
   if (context.getOwningMember().getProperty<AsAttribute>()) {
      m_nextValueIsAttribute = true;
      m_attributeName = "key";
   } else {
      xml_node<> *node = m_document.allocate_node(node_element, "key");
      m_stack.top()->append_node(node);
      m_stack.push(node);
   }
}
void XmlWriteBuffer::leaveKey(const Context& context) throw(Exception) {
   if (context.getOwningMember().getProperty<AsAttribute>()) {
      m_nextValueIsAttribute = false;
   } else {
      m_stack.pop();
   }
}

void XmlWriteBuffer::enterValue(const Context& context) throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, "value");
   m_stack.top()->append_node(node);
   m_stack.push(node);
}

void XmlWriteBuffer::leaveValue(const Context& context) throw(Exception) {
   m_stack.pop();
}

void XmlWriteBuffer::leaveCollection(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlWriteBuffer::leaveMap(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlWriteBuffer::appendVersion(const int& version, const Context& context) throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, "__version");
   m_stack.top()->append_node(node);
   setNodeOrAttributeValue(node, version);
}

void XmlWriteBuffer::appendBits(const char* data, long int length, const Context& context) throw(Exception) {
   std::string bits;
   unsigned char mask = 0x80;
   for (size_t i = 0; i < length; ++i) {
      if ((data[i / 8] & mask) != 0)
         bits.append("1");
      else
         bits.append("0");
      if (mask == 0x01)
         mask = 0x80;
      else
         mask = mask >> 1;
   }
   append(bits, context);
}

void XmlWriteBuffer::appendCollectionSize(const std::size_t& size, const Context& context) throw(Exception) {

}

void XmlWriteBuffer::appendTypeId(const std::string& val, const Context& context) throw(Exception) {
   const char* field = m_document.allocate_string(context.getClassContext().getRootProperty<ctrl::TypeIdFieldName>().c_str());
   if (context.getClassContext().getRootProperty<TypeIdFieldAsAttribute>()) {
      xml_attribute<>* attr = m_document.allocate_attribute(field, m_document.allocate_string(val.c_str()));
      m_stack.top()->append_attribute(attr);
   } else {
      xml_node<> *node = m_document.allocate_node(node_element, field, m_document.allocate_string(val.c_str()));
      m_stack.top()->append_node(node);
   }
}

void XmlWriteBuffer::append(const bool& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val ? "true" : "false");
}


void XmlWriteBuffer::append(const char& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const short& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const int& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const long& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const long long& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const unsigned char& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const unsigned short& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const unsigned int& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const unsigned long& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const unsigned long long& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const float& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const double& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}


void XmlWriteBuffer::append(const std::string& val, const Context& context) throw(Exception) {
   setNodeOrAttributeValue(m_stack.top(), val);
}

std::string XmlWriteBuffer::getOutput() const {
   std::string result;
   print(std::back_inserter(result), m_document, m_prettyPrint ? 0 : rapidxml::print_no_indenting);
   return result;
}
