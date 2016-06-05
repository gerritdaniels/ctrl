#ifndef XMLWRITEBUFFER_H
#define XMLWRITEBUFFER_H

#include <ctrl/buffer/abstractWriteBuffer.h>
#include <ctrl/rapidxml/rapidxml.hpp>
#include <stack>
#include <sstream>
#include <string>

namespace ctrl {

namespace Private {

   class XmlWriteBuffer : public AbstractWriteBuffer {
   public:
      XmlWriteBuffer();

      virtual void enterObject() throw(Exception);
      virtual void enterMember(const char* name) throw(Exception);
      virtual void leaveMember() throw(Exception);
      virtual void leaveObject() throw(Exception);

      virtual void enterCollection() throw(Exception);
      virtual void nextCollectionElement() throw(Exception);
      virtual void leaveCollection() throw(Exception);

      virtual void enterMap() throw(Exception);
      virtual void enterKey() throw(Exception);
      virtual void leaveKey() throw(Exception);
      virtual void enterValue() throw(Exception);
      virtual void leaveValue() throw(Exception);
      virtual void leaveMap() throw(Exception);

      virtual void appendVersion(const int& version) throw(Exception);
      virtual void appendBits(const char* data, long length) throw(Exception);
      virtual void appendCollectionSize(const std::size_t& size) throw(Exception);
      virtual void appendPointerId(const int& id) throw(Exception);
      virtual void appendTypeId(const char* val) throw(Exception);

      virtual void append(const bool& val) throw(Exception);
      virtual void append(const char& val) throw(Exception);
      virtual void append(const short& val) throw(Exception);
      virtual void append(const int& val) throw(Exception);
      virtual void append(const long& val) throw(Exception);
      virtual void append(const long long& val) throw(Exception);
      virtual void append(const unsigned char& val) throw(Exception);
      virtual void append(const unsigned short& val) throw(Exception);
      virtual void append(const unsigned int& val) throw(Exception);
      virtual void append(const unsigned long& val) throw(Exception);
      virtual void append(const unsigned long long& val) throw(Exception);
      virtual void append(const float& val) throw(Exception);
      virtual void append(const double& val) throw(Exception);
      virtual void append(const std::string& val) throw(Exception);

      std::string getOutput() const;

   private:
      template <class T>
      void writeValueAsString(const T& val) {
         m_converter << val;
         const char* converted = m_document.allocate_string(m_converter.str().c_str());
         m_stack.top()->value(converted);
         m_converter.str("");
         m_converter.clear();
      }

      rapidxml::xml_document<> m_document;
      std::stack<rapidxml::xml_node<>*> m_stack;
      std::ostringstream m_converter;

      bool m_collectionStart;
   };

} // namespace Private

} // namespace ctrl

#endif // XMLWRITEBUFFER_H

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
