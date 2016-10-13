
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

#include <ctrl/buffer/jsonReadBuffer.h>
#include <ctrl/nlohmann/json.hpp>
#include <ctrl/properties.h>

using namespace nlohmann;
using namespace ctrl;
using namespace ctrl::Private;

JsonReadBuffer::JsonReadBuffer(const std::string& data) : m_collectionStart(false),
         m_mapStart(false),
         m_skipNextFundamental(false),
         m_keyStart(false) {
   m_document = json::parse(data);
   checkNonNull(m_document);
   m_stack.push(&m_document);
}

void JsonReadBuffer::checkNonNull(const json& node, const std::string& name) {
   if (node.is_null())  {
      throw ctrl::Exception("Corrupt data: no node with name '" + name + "' found");
   }
}

void JsonReadBuffer::enterObject(const Context& context) throw(Exception) {

}

void JsonReadBuffer::enterMember(const Context& context, const char* suggested) throw(Exception) {
   std::string name = m_util.preferedMemberName(context.getOwningMember(), suggested);

   if (context.getOwningMember().getProperty<ctrl::AsIdField>()) {
      if (!context.getOwningMember().isFundamental()) {
         throw Exception("Non fundamental type used as id field (context: " + name + ")");
      }
      m_skipNextFundamental = true;
   } else {
      json& node = (*m_stack.top())[name];
      checkNonNull(node, name);
      m_stack.push(&node);
   }
}

void JsonReadBuffer::leaveMember(const Context& context) throw(Exception) {
   if (m_skipNextFundamental) {
      m_skipNextFundamental = false;
   } else {
      m_stack.pop();
   }
}

void JsonReadBuffer::leaveObject(const Context& context) throw(Exception) {

}

void JsonReadBuffer::enterIdField(const Context& context) throw(Exception) {
   MemberContext idFieldContext = context.getClassContext().getRootIdField().getMemberContext();
   std::string name;
   if (context.getClassContext().hasRootProperty<IdFieldName>()) {
      name = context.getClassContext().getRootProperty<IdFieldName>();
   } else {
      name = m_util.preferedMemberName(idFieldContext);
   }
   json& node = (*m_stack.top())[name];
   m_stack.push(&node);
}

bool JsonReadBuffer::isNullId(const Context& context) throw(Exception) {
   return m_stack.top()->is_null();
}

void JsonReadBuffer::leaveIdField(const Context& context) throw(Exception) {
   m_stack.pop();
}

void JsonReadBuffer::enterCollection(const Context& context) throw(Exception) {
   m_collectionStart = true;
}

void JsonReadBuffer::enterMap(const Context& context) throw(Exception) {
   if (!context.getOwningMember().isMapKeyFundamental()) {
      throw Exception("Only fundamental map keys allowed when serializing to JSON");
   }
   m_mapStart = true;
}

void JsonReadBuffer::nextCollectionElement(const Context& context) throw(Exception) {
   if (m_collectionStart || m_mapStart) {
      if (m_mapStart) {
         m_keyValue = m_stack.top()->begin().key();
      }
      m_iterStack.push(m_stack.top()->begin());
      m_stack.push(&(*m_stack.top()->begin()));
      m_collectionStart = false;
      m_mapStart = false;
   } else {
      json* prev = m_stack.top();
      m_stack.pop();
      json::iterator next = ++m_iterStack.top();
      m_iterStack.pop();
      m_iterStack.push(next);
      if (m_stack.top()->is_object()) {
         m_keyValue = next.key();
      }
      m_stack.push(&*next);
   }
}

void JsonReadBuffer::enterKey(const Context& context) throw(Exception) {
   m_keyStart = true;
}

void JsonReadBuffer::leaveKey(const Context& context) throw(Exception) {
   m_keyStart = false;
}

void JsonReadBuffer::enterValue(const Context& context) throw(Exception) {

}

void JsonReadBuffer::leaveValue(const Context& context) throw(Exception) {

}

void JsonReadBuffer::leaveCollection(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
      m_iterStack.pop();
   }
}

void JsonReadBuffer::leaveMap(const Context& context) throw(Exception) {
   if (m_mapStart) {
      m_mapStart = false;
   } else {
      m_stack.pop();
      m_iterStack.pop();
   }
}

void JsonReadBuffer::readVersion(int& version, const Context& context) throw(Exception) {
   version = 1;
}

void JsonReadBuffer::readBits(char* data, long int length, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) {
      std::string bits;
      readNode(m_stack.top(), bits);
      if (bits.length() != length) {
         throw ctrl::Exception("Number of bits doesn't match");
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

void JsonReadBuffer::readCollectionSize(std::size_t& size, const Context& context) throw(Exception) {
   json& node = *m_stack.top();
   size = node.size();
}

void JsonReadBuffer::readTypeId(std::string& val, const Context& context) throw(Exception) {
   std::string field = context.getClassContext().getRootProperty<ctrl::TypeIdFieldName>();
   json& node = (*m_stack.top())[field];
   checkNonNull(node, field);
   val = node.get<std::string>();
}

void JsonReadBuffer::read(bool& val, const Context& context) throw(Exception) {
   readNode(m_stack.top(), val);
}

void JsonReadBuffer::read(char& val, const Context& context) throw(Exception) {
   readNode(m_stack.top(), val);
}

void JsonReadBuffer::read(short& val, const Context& context) throw(Exception) {
   readNode<short>(m_stack.top(), val);
}

void JsonReadBuffer::read(int& val, const Context& context) throw(Exception) {
   readNode<int>(m_stack.top(), val);
}

void JsonReadBuffer::read(long& val, const Context& context) throw(Exception) {
   readNode<long>(m_stack.top(), val);
}

void JsonReadBuffer::read(long long& val, const Context& context) throw(Exception) {
   readNode<long long>(m_stack.top(), val);
}

void JsonReadBuffer::read(unsigned char& val, const Context& context) throw(Exception) {
   readNode<unsigned char>(m_stack.top(), val);
}

void JsonReadBuffer::read(unsigned short& val, const Context& context) throw(Exception) {
   readNode<unsigned short>(m_stack.top(), val);
}

void JsonReadBuffer::read(unsigned int& val, const Context& context) throw(Exception) {
   readNode<unsigned int>(m_stack.top(), val);
}

void JsonReadBuffer::read(unsigned long& val, const Context& context) throw(Exception) {
   readNode<unsigned long>(m_stack.top(), val);
}

void JsonReadBuffer::read(unsigned long long& val, const Context& context) throw(Exception) {
   readNode<unsigned long long>(m_stack.top(), val);
}

void JsonReadBuffer::read(float& val, const Context& context) throw(Exception) {
   readNode<float>(m_stack.top(), val);
}

void JsonReadBuffer::read(double& val, const Context& context) throw(Exception) {
   readNode<double>(m_stack.top(), val);
}

void JsonReadBuffer::read(std::string& val, const Context& context) throw(Exception) {
   readNode(m_stack.top(), val);
}
