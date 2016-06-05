
#include <climits>
#include <ctrl/buffer/xmlWriteBuffer.h>
#include <ctrl/rapidxml/rapidxml_print.hpp>

using namespace rapidxml;
using namespace ctrl::Private;

XmlWriteBuffer::XmlWriteBuffer() : m_collectionStart(false) {
   xml_node<> *node = m_document.allocate_node(node_element, "root");
   m_document.append_node(node);
   m_stack.push(node);
}

void XmlWriteBuffer::enterObject() throw(Exception) {

}

void XmlWriteBuffer::enterMember(const char* name) throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, name);
   m_stack.top()->append_node(node);
   m_stack.push(node);
}

void XmlWriteBuffer::leaveMember() throw(Exception) {
   m_stack.pop();
}

void XmlWriteBuffer::leaveObject() throw(Exception) {

}

void XmlWriteBuffer::enterCollection() throw(Exception) {
   m_collectionStart = true;
}

void XmlWriteBuffer::enterMap() throw(Exception) {
   m_collectionStart = true;
}

void XmlWriteBuffer::nextCollectionElement() throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
   xml_node<> *node = m_document.allocate_node(node_element, "item");
   m_stack.top()->append_node(node);
   m_stack.push(node);
}

void XmlWriteBuffer::enterKey() throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, "key");
   m_stack.top()->append_node(node);
   m_stack.push(node);
}
void XmlWriteBuffer::leaveKey() throw(Exception) {
   m_stack.pop();
}

void XmlWriteBuffer::enterValue() throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, "value");
   m_stack.top()->append_node(node);
   m_stack.push(node);
}

void XmlWriteBuffer::leaveValue() throw(Exception) {
   m_stack.pop();
}

void XmlWriteBuffer::leaveCollection() throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlWriteBuffer::leaveMap() throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else {
      m_stack.pop();
   }
}

void XmlWriteBuffer::appendVersion(const int& version) throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, "__version");
   m_stack.top()->append_node(node);
   m_stack.push(node);
   writeValueAsString(version);
   m_stack.pop();
}

void XmlWriteBuffer::appendBits(const char* data, long int length) throw(Exception) {
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
   append(bits);
}

void XmlWriteBuffer::appendCollectionSize(const std::size_t& size) throw(Exception) {

}

void XmlWriteBuffer::appendPointerId(const int& id) throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, "__id");
   m_stack.top()->append_node(node);
   m_stack.push(node);
   writeValueAsString(id);
   m_stack.pop();
}

void XmlWriteBuffer::appendTypeId(const char* val) throw(Exception) {
   xml_node<> *node = m_document.allocate_node(node_element, "__typeId", val);
   m_stack.top()->append_node(node);
}

void XmlWriteBuffer::append(const bool& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const char& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const short& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const int& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const long& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const long long& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const unsigned char& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const unsigned short& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const unsigned int& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const unsigned long& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const unsigned long long& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const float& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const double& val) throw(Exception) {
   writeValueAsString(val);
}


void XmlWriteBuffer::append(const std::string& val) throw(Exception) {
   const char* converted = m_document.allocate_string(val.c_str());
   m_stack.top()->value(converted);
}

std::string XmlWriteBuffer::getOutput() const {
   std::string result;
   print(std::back_inserter(result), m_document, 0);
   return result;
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

