
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
#include <ctrl/buffer/jsonWriteBuffer.h>
#include <ctrl/nlohmann/json.hpp>
#include <ctrl/properties.h>

using namespace nlohmann;
using namespace ctrl;
using namespace ctrl::Private;

JsonWriteBuffer::JsonWriteBuffer(int indentation) : m_collectionStart(false),
         m_mapStart(false),
         m_keyStart(false),
         m_skipNextFundamental(false),
         m_indentation(indentation) {
   m_document = json();
   m_stack.push(&m_document);
}

void JsonWriteBuffer::enterObject(const Context& context) throw(Exception) {

}

void JsonWriteBuffer::enterMember(const Context& context, const char* suggested) throw(Exception) {
   std::string name = m_util.preferedMemberName(context.getOwningMember(), suggested);

   if (context.getOwningMember().getProperty<ctrl::AsIdField>()) {
      if (!context.getOwningMember().isFundamental()) {
         throw Exception("Non fundamental type used as id field (context: " + name + ")");
      }
      m_skipNextFundamental = true;
   } else {
      (*m_stack.top())[name] = json();
      m_stack.push(&(*m_stack.top())[name]);
   }
}

void JsonWriteBuffer::leaveMember(const Context& context) throw(Exception) {
   if (context.getOwningMember().getProperty<ctrl::AsIdField>()) {
      m_skipNextFundamental = false;
   } else {
      m_stack.pop();
   }
}

void JsonWriteBuffer::leaveObject(const Context& context) throw(Exception) {

}

void JsonWriteBuffer::enterIdField(const Context& context) throw(Exception) {
   MemberContext idFieldContext = context.getClassContext().getRootIdField().getMemberContext();
   std::string name;
   if (context.getClassContext().hasRootProperty<IdFieldName>()) {
      name = context.getClassContext().getRootProperty<IdFieldName>();
   } else {
      name = m_util.preferedMemberName(idFieldContext);
   }
   (*m_stack.top())[name] = json();
   m_stack.push(&(*m_stack.top())[name]);
}

void JsonWriteBuffer::appendNullId(const Context& context) throw(Exception) {
   (*m_stack.top()) = nullptr;
}

void JsonWriteBuffer::appendNonNullId(const Context& context) throw(Exception) {

}

void JsonWriteBuffer::leaveIdField(const Context& context) throw(Exception) {
   m_stack.pop();
}

void JsonWriteBuffer::enterCollection(const Context& context) throw(Exception) {
   m_collectionStart = true;
}

void JsonWriteBuffer::enterMap(const Context& context) throw(Exception) {
   if (!context.getOwningMember().isMapKeyFundamental()) {
      throw Exception("Only fundamental map keys allowed when serializing to JSON");
   }
   m_mapStart = true;
}

void JsonWriteBuffer::nextCollectionElement(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
   } else if (!m_mapStart) {
      m_stack.pop();
   }
   if (!m_mapStart && !m_stack.top()->is_object()) {
      m_stack.top()->push_back(json());
      m_stack.push(&(*m_stack.top())[m_stack.top()->size() - 1]);
   } else {
      m_mapStart = false;
   }
}

void JsonWriteBuffer::enterKey(const Context& context) throw(Exception) {
   m_keyStart = true;
}

void JsonWriteBuffer::leaveKey(const Context& context) throw(Exception) {
   m_keyStart = false;
}

void JsonWriteBuffer::enterValue(const Context& context) throw(Exception) {

}

void JsonWriteBuffer::leaveValue(const Context& context) throw(Exception) {

}

void JsonWriteBuffer::leaveCollection(const Context& context) throw(Exception) {
   if (m_collectionStart) {
      m_collectionStart = false;
      (*m_stack.top()) = json::array();
   } else {
      m_stack.pop();
   }
}

void JsonWriteBuffer::leaveMap(const Context& context) throw(Exception) {
   if (m_mapStart) {
      m_mapStart = false;
      (*m_stack.top()) = json::object();
   } else {
      m_stack.pop();
   }
}

void JsonWriteBuffer::appendVersion(const int& version, const Context& context) throw(Exception) {

}

void JsonWriteBuffer::appendBits(const char* data, long int length, const Context& context) throw(Exception) {
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

void JsonWriteBuffer::appendCollectionSize(const std::size_t& size, const Context& context) throw(Exception) {

}

void JsonWriteBuffer::appendTypeId(const std::string& val, const Context& context) throw(Exception) {
   std::string field = context.getClassContext().getRootProperty<ctrl::TypeIdFieldName>();
   (*m_stack.top())[field] = val;
}

void JsonWriteBuffer::append(const bool& val, const Context& context) throw(Exception) {
   if (m_keyStart) {
      if (!(*m_stack.top())[val ? "true" : "false"].is_null()) {
         throw Exception("Multimaps with duplicated keys aren't supported when serializing to JSON");
      }
      (*m_stack.top())[val ? "true" : "false"] = json();
      m_stack.push(&(*m_stack.top())[val ? "true" : "false"]);
   } else if (!m_skipNextFundamental) {
      (*m_stack.top()) = val ? "true" : "false";
   }
}


void JsonWriteBuffer::append(const char& val, const Context& context) throw(Exception) {
   if (m_keyStart) {
      if (!(*m_stack.top())[m_util.toString(val)].is_null()) {
         throw Exception("Multimaps with duplicated keys aren't supported when serializing to JSON");
      }
      (*m_stack.top())[m_util.toString(val)] = json();
      m_stack.push(&(*m_stack.top())[m_util.toString(val)]);
   } else if (!m_skipNextFundamental) {
      (*m_stack.top()) = m_util.toString(val);
   }
}


void JsonWriteBuffer::append(const short& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const int& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const long& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const long long& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const unsigned char& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const unsigned short& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const unsigned int& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const unsigned long& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const unsigned long long& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const float& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const double& val, const Context& context) throw(Exception) {
   appendNumber(val);
}


void JsonWriteBuffer::append(const std::string& val, const Context& context) throw(Exception) {
   if (m_keyStart) {
      if (!(*m_stack.top())[val].is_null()) {
         throw Exception("Multimaps with duplicated keys aren't supported when serializing to JSON");
      }
      (*m_stack.top())[val] = json();
      m_stack.push(&(*m_stack.top())[val]);
   } else if (!m_skipNextFundamental) {
      (*m_stack.top()) = val;
   }
}

std::string JsonWriteBuffer::getOutput() const {
   if (m_indentation == 0) {
      return m_document.dump();
   } else {
      return m_document.dump(m_indentation);
   }
}
