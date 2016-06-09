
/*
 * Copyright (C) 2010, 2012, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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

#include <ctrl/buffer/binaryWriteBuffer.h>
#include <ctrl/context.h>
#include <ctrl/properties.h>

using namespace ctrl;
using namespace ctrl::Private;

const char* BinaryWriteBuffer::Impl::s_padding = "\0\0\0\0\0\0";

BinaryWriteBuffer::Impl::Impl()
   : m_data(new char[10])
   , m_capacity(10)
   , m_length(0)
   , m_owner(true) {

}

BinaryWriteBuffer::Impl::~Impl() {
   if (m_owner) delete[] m_data;
}

long BinaryWriteBuffer::Impl::length() {
   return m_length;
}

char* BinaryWriteBuffer::Impl::getData() {
   m_owner = false;
   return m_data;
}

void BinaryWriteBuffer::Impl::appendNoPadding(const char* data, long length) {
   long newLength = m_length + length;
   if (newLength > m_capacity)
      realloc(newLength);

   for (long i = m_length; i < newLength; ++i)
      m_data[i] = data[i - m_length];

   m_length += length;
}

void BinaryWriteBuffer::Impl::realloc(long newLength) {
   long newCapacity = m_capacity * 10;
   while(newCapacity < newLength)
      newCapacity *= 10;

   char* newData = new char[newCapacity];
   for (long i = 0; i < m_length; ++i)
      newData[i] = m_data[i];

   delete[] m_data;
   m_data = newData;
   m_capacity = newCapacity;
}

BinaryWriteBuffer::BinaryWriteBuffer(BinaryWriteBuffer::Impl* pimpl)
   : m_pimpl(pimpl), m_skipNextFundamental(false) {

}

long BinaryWriteBuffer::length() {
   return m_pimpl->length();
}

char* BinaryWriteBuffer::getData() {
   return m_pimpl->getData();
}

void BinaryWriteBuffer::enterObject(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::enterMember(const Context& context, const char* suggested) throw(Exception) {
   if (context.getOwningMember().getProperty<ctrl::AsIdField>()) {
      if (!context.getOwningMember().isFundamental()) {
         throw Exception("Non fundamental type used as id field");
      }
      m_skipNextFundamental = true;
   }
}

void BinaryWriteBuffer::leaveMember(const Context& context) throw(Exception) {
   m_skipNextFundamental = false;
}

void BinaryWriteBuffer::leaveObject(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::enterIdField(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::appendNullId(const Context& context) throw(Exception) {
   append((char) 0, context);
}

void BinaryWriteBuffer::appendNonNullId(const Context& context) throw(Exception) {
   append((char) 1, context);
}

void BinaryWriteBuffer::leaveIdField(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::enterCollection(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::enterMap(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::nextCollectionElement(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::enterKey(const Context& context) throw(Exception) {

}
void BinaryWriteBuffer::leaveKey(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::enterValue(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::leaveValue(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::leaveCollection(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::leaveMap(const Context& context) throw(Exception) {

}

void BinaryWriteBuffer::appendVersion(const int& version, const Context& context) throw(Exception) {
   m_pimpl->append(version);
}

void BinaryWriteBuffer::appendBits(const char* data, long int length, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) {
      size_t nbChars = length / 8 + (length % 8 == 0 ? 0 : 1);
      m_pimpl->append(data, nbChars);
   }
}

void BinaryWriteBuffer::appendCollectionSize(const std::size_t& size, const Context& context) throw(Exception) {
   m_pimpl->append(size);
}

void BinaryWriteBuffer::appendTypeId(const std::string& val, const Context& context) throw(Exception) {
   m_pimpl->append(val);
}

void BinaryWriteBuffer::append(const bool& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const char& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const short& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const int& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const long long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const unsigned char& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const unsigned short& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const unsigned int& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const unsigned long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const unsigned long long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const float& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const double& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}


void BinaryWriteBuffer::append(const std::string& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->append(val); }
}
