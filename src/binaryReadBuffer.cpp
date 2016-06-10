
/*
 * Copyright (C) 2010, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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

#include <ctrl/buffer/binaryReadBuffer.h>
#include <ctrl/exception.h>
#include <ctrl/context.h>

using namespace ctrl;
using namespace ctrl::Private;

BinaryReadBuffer::Impl::Impl() { }

BinaryReadBuffer::Impl::~Impl() { }

BinaryReadBuffer::BinaryReadBuffer(BinaryReadBuffer::Impl* pimpl)
   : m_pimpl(pimpl), m_skipNextFundamental(false) {

}

void BinaryReadBuffer::enterObject(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::enterMember(const Context& context, const char* suggested) throw(Exception) {
   if (context.getOwningMember().getProperty<ctrl::AsIdField>()) {
      if (!context.getOwningMember().isFundamental()) {
         throw ctrl::Exception("Non fundamental type used as id field");
      }
      m_skipNextFundamental = true;
   }
}

void BinaryReadBuffer::leaveMember(const Context& context) throw(Exception) {
   m_skipNextFundamental = false;
}

void BinaryReadBuffer::leaveObject(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::enterIdField(const Context& context) throw(Exception) {

}

bool BinaryReadBuffer::isNullId(const Context& context) throw(Exception) {
   char c;
   read(c, context);
   return c == 0;
}

void BinaryReadBuffer::leaveIdField(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::enterCollection(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::enterMap(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::nextCollectionElement(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::enterKey(const Context& context) throw(Exception) {

}
void BinaryReadBuffer::leaveKey(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::enterValue(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::leaveValue(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::leaveCollection(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::leaveMap(const Context& context) throw(Exception) {

}

void BinaryReadBuffer::readVersion(int& version, const Context& context) throw(Exception) {
   m_pimpl->read(version);
}

void BinaryReadBuffer::readBits(char* data, long int length, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) {
      size_t nbChars = length / 8 + (length % 8 == 0 ? 0 : 1);
      m_pimpl->read(data, nbChars);
   }
}

void BinaryReadBuffer::readCollectionSize(std::size_t& size, const Context& context) throw(Exception) {
   m_pimpl->read(size);
}

void BinaryReadBuffer::readTypeId(std::string& val, const Context& context) throw(Exception) {
   m_pimpl->read(val);
}

void BinaryReadBuffer::read(bool& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(char& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(short& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(int& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(long long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(unsigned char& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(unsigned short& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(unsigned int& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(unsigned long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(unsigned long long& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(float& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(double& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}


void BinaryReadBuffer::read(std::string& val, const Context& context) throw(Exception) {
   if (!m_skipNextFundamental) { m_pimpl->read(val); }
}

bool BinaryReadBuffer::reachedEnd() const {
   return m_pimpl->reachedEnd();
}

