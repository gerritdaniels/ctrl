#ifndef XMLREADBUFFER_H
#define XMLREADBUFFER_H

#include <string>
#include <stack>
#include <sstream>
#include <ctrl/buffer/abstractReadBuffer.h>
#include <ctrl/rapidxml/rapidxml.hpp>

namespace ctrl {

namespace Private {

   class XmlReadBuffer : public AbstractReadBuffer {
   public:
      XmlReadBuffer(const std::string& data);

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

      virtual void readVersion(int& version) throw(Exception);
      virtual void readBits(char* data, long length) throw(Exception);
      virtual void readCollectionSize(std::size_t& size) throw(Exception);
      virtual void readPointerId(int& id) throw(Exception);
      virtual void readTypeId(std::string& val) throw(Exception);

      virtual void read(bool& val) throw(Exception);
      virtual void read(char& val) throw(Exception);
      virtual void read(short& val) throw(Exception);
      virtual void read(int& val) throw(Exception);
      virtual void read(long& val) throw(Exception);
      virtual void read(long long& val) throw(Exception);
      virtual void read(unsigned char& val) throw(Exception);
      virtual void read(unsigned short& val) throw(Exception);
      virtual void read(unsigned int& val) throw(Exception);
      virtual void read(unsigned long& val) throw(Exception);
      virtual void read(unsigned long long& val) throw(Exception);
      virtual void read(float& val) throw(Exception);
      virtual void read(double& val) throw(Exception);
      virtual void read(std::string& val) throw(Exception);

   private:
      std::string unwindStack();
      void checkNonNull(const rapidxml::xml_node<>* node, const std::string& name);

      template <class T>
      T readValueFromString(const std::string& str) {
         m_converter.str(str);
         m_converter.clear();
         T val;
         m_converter >> val;
         return val;
      }

      std::istringstream m_converter;

      rapidxml::xml_document<> m_document;
      std::stack<rapidxml::xml_node<>*> m_stack;
      bool m_collectionStart;
   };

} // namespace ctrl

} // namespace Private

#endif // XMLREADBUFFER_H

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
