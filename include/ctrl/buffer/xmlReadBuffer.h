
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

#ifndef XMLREADBUFFER_H
#define XMLREADBUFFER_H

#include <string>
#include <stack>
#include <sstream>
#include <ctrl/buffer/abstractReadBuffer.h>
#include <ctrl/buffer/bufferUtil.h>
#include <ctrl/rapidxml/rapidxml.hpp>

namespace ctrl {

namespace Private {

   class XmlReadBuffer : public ctrl::AbstractReadBuffer {
   public:
      XmlReadBuffer(const std::string& data);

      virtual void enterObject(const Context& context) throw(Exception);
      virtual void enterMember(const Context& context, const char* suggested = 0) throw(Exception);
      virtual void leaveMember(const Context& context) throw(Exception);
      virtual void leaveObject(const Context& context) throw(Exception);

      virtual void enterCollection(const Context& context) throw(Exception);
      virtual void nextCollectionElement(const Context& context) throw(Exception);
      virtual void leaveCollection(const Context& context) throw(Exception);

      virtual void enterIdField(const Context& context) throw(Exception);
      virtual bool isNullId(const Context& context) throw(Exception);
      virtual void leaveIdField(const Context& context) throw(Exception);

      virtual void enterMap(const Context& context) throw(Exception);
      virtual void enterKey(const Context& context) throw(Exception);
      virtual void leaveKey(const Context& context) throw(Exception);
      virtual void enterValue(const Context& context) throw(Exception);
      virtual void leaveValue(const Context& context) throw(Exception);
      virtual void leaveMap(const Context& context) throw(Exception);

      virtual void readVersion(int& version, const Context& context) throw(Exception);
      virtual void readBits(char* data, long length, const Context& context) throw(Exception);
      virtual void readCollectionSize(std::size_t& size, const Context& context) throw(Exception);
      virtual void readTypeId(std::string& val, const Context& context) throw(Exception);

      virtual void read(bool& val, const Context& context) throw(Exception);
      virtual void read(char& val, const Context& context) throw(Exception);
      virtual void read(short& val, const Context& context) throw(Exception);
      virtual void read(int& val, const Context& context) throw(Exception);
      virtual void read(long& val, const Context& context) throw(Exception);
      virtual void read(long long& val, const Context& context) throw(Exception);
      virtual void read(unsigned char& val, const Context& context) throw(Exception);
      virtual void read(unsigned short& val, const Context& context) throw(Exception);
      virtual void read(unsigned int& val, const Context& context) throw(Exception);
      virtual void read(unsigned long& val, const Context& context) throw(Exception);
      virtual void read(unsigned long long& val, const Context& context) throw(Exception);
      virtual void read(float& val, const Context& context) throw(Exception);
      virtual void read(double& val, const Context& context) throw(Exception);
      virtual void read(std::string& val, const Context& context) throw(Exception);

   private:
      std::string unwindStack();
      void checkNonNull(const rapidxml::xml_base<>* node, const std::string& name);

      template <class T_>
      void readNodeOrAttributeValue(rapidxml::xml_node<>* node, T_& val) {
         if (!m_skipNextFundamental) {
            if (m_nextValueIsAttribute) {
               val = m_util.fromString<T_>(node->first_attribute(m_attributeName.c_str())->value());
            } else {
               val = m_util.fromString<T_>(node->value());
            }
         }
      }

      void readNodeOrAttributeValue(rapidxml::xml_node<>* node, std::string& val) {
         if (!m_skipNextFundamental) {
            if (m_nextValueIsAttribute) {
               val = node->first_attribute(m_attributeName.c_str())->value();
            } else {
               val =  node->value();
            }
         }
      }

      void readNodeOrAttributeValue(rapidxml::xml_node<>* node, bool& val) {
         if (!m_skipNextFundamental) {
            if (m_nextValueIsAttribute) {
               val = std::string(node->first_attribute(m_attributeName.c_str())->value()) == "true" ? true : false;
            } else {
               val =  std::string(node->value()) == "true" ? true : false;
            }
         }
      }

      BufferUtil m_util;
      rapidxml::xml_document<> m_document;
      std::stack<rapidxml::xml_node<>*> m_stack;
      bool m_collectionStart;
      bool m_nextValueIsAttribute;
      bool m_skipNextFundamental;
      std::string m_attributeName;
   };

} // namespace ctrl

} // namespace Private

#endif // XMLREADBUFFER_H
