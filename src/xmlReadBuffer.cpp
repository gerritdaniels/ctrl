#include <ctrl/buffer/xmlReadBuffer.h>

using namespace rapidxml;
using namespace ctrl::Private;

XmlReadBuffer::XmlReadBuffer(const std::string& data) : m_collectionStart(false) {
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

void XmlReadBuffer::checkNonNull(const rapidxml::xml_node<>* node, const std::string& name) {
   if (node == 0)  {
      throw ctrl::Exception("Corrupt data: no node with name '" + name + "' found (context: " + unwindStack() + ")");
   }
}

void XmlReadBuffer::enterObject() throw(Exception) {

}

void XmlReadBuffer::enterMember(const char* name) throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node(name);
   checkNonNull(node, name);
   m_stack.push(node);
}

void XmlReadBuffer::leaveMember() throw(Exception) {
   m_stack.pop();
}

void XmlReadBuffer::leaveObject() throw(Exception) {

}

void XmlReadBuffer::enterCollection() throw(Exception) {
   m_collectionStart = true;
}

void XmlReadBuffer::enterMap() throw(Exception) {
   m_collectionStart = true;
}

void XmlReadBuffer::nextCollectionElement() throw(Exception) {
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

void XmlReadBuffer::enterKey() throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node("key");
   checkNonNull(node, "key");
   m_stack.push(node);
}

void XmlReadBuffer::leaveKey() throw(Exception) {
   m_stack.pop();
}

void XmlReadBuffer::enterValue() throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node("value");
   checkNonNull(node, "value");
   m_stack.push(node);
}

void XmlReadBuffer::leaveValue() throw(Exception) {
   m_stack.pop();
}

void XmlReadBuffer::leaveCollection() throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlReadBuffer::leaveMap() throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlReadBuffer::readVersion(int& version) throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node("__version");
   checkNonNull(node, "__version");
   version = readValueFromString<int>(node->value());
}

void XmlReadBuffer::readBits(char* data, long int length) throw(Exception) {
   std::string bits(m_stack.top()->value());
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

void XmlReadBuffer::readCollectionSize(std::size_t& size) throw(Exception) {
   xml_node<>* node = m_stack.top()->first_node();
   size = 0;
   while (node != 0) {
      ++size;
      node = node->next_sibling();
   }
}

void XmlReadBuffer::readPointerId(int& id) throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node("__id");
   checkNonNull(node, "__id");
   id = readValueFromString<int>(node->value());
}

void XmlReadBuffer::readTypeId(std::string& val) throw(Exception) {
   xml_node<> *node = m_stack.top()->first_node("__typeId");
   checkNonNull(node, "__typeId");
   val = node->value();
}

void XmlReadBuffer::read(bool& val) throw(Exception) {
   val = readValueFromString<bool>(m_stack.top()->value());
}

void XmlReadBuffer::read(char& val) throw(Exception) {
   val = readValueFromString<char>(m_stack.top()->value());
}

void XmlReadBuffer::read(short& val) throw(Exception) {
   val = readValueFromString<short>(m_stack.top()->value());
}

void XmlReadBuffer::read(int& val) throw(Exception) {
   val = readValueFromString<int>(m_stack.top()->value());
}

void XmlReadBuffer::read(long& val) throw(Exception) {
   val = readValueFromString<long>(m_stack.top()->value());
}

void XmlReadBuffer::read(long long& val) throw(Exception) {
   val = readValueFromString<long long>(m_stack.top()->value());
}

void XmlReadBuffer::read(unsigned char& val) throw(Exception) {
   val = readValueFromString<unsigned char>(m_stack.top()->value());
}

void XmlReadBuffer::read(unsigned short& val) throw(Exception) {
   val = readValueFromString<unsigned short>(m_stack.top()->value());
}

void XmlReadBuffer::read(unsigned int& val) throw(Exception) {
   val = readValueFromString<unsigned int>(m_stack.top()->value());
}

void XmlReadBuffer::read(unsigned long& val) throw(Exception) {
   val = readValueFromString<unsigned long>(m_stack.top()->value());
}

void XmlReadBuffer::read(unsigned long long& val) throw(Exception) {
   val = readValueFromString<unsigned long long>(m_stack.top()->value());
}

void XmlReadBuffer::read(float& val) throw(Exception) {
   val = readValueFromString<float>(m_stack.top()->value());
}

void XmlReadBuffer::read(double& val) throw(Exception) {
   val = readValueFromString<double>(m_stack.top()->value());
}

void XmlReadBuffer::read(std::string& val) throw(Exception) {
   val = m_stack.top()->value();
}

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
