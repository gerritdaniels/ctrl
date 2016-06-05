
#include <ctrl/buffer/binaryReadBuffer.h>

using namespace ctrl::Private;

BinaryReadBuffer::Impl::Impl() { }

BinaryReadBuffer::Impl::~Impl() { }

BinaryReadBuffer::BinaryReadBuffer(BinaryReadBuffer::Impl* pimpl)
   : m_pimpl(pimpl) {

}

void BinaryReadBuffer::enterObject() throw(Exception) {

}

void BinaryReadBuffer::enterMember(const char* name) throw(Exception) {

}

void BinaryReadBuffer::leaveMember() throw(Exception) {

}

void BinaryReadBuffer::leaveObject() throw(Exception) {

}

void BinaryReadBuffer::enterCollection() throw(Exception) {

}

void BinaryReadBuffer::enterMap() throw(Exception) {

}

void BinaryReadBuffer::nextCollectionElement() throw(Exception) {

}

void BinaryReadBuffer::enterKey() throw(Exception) {

}
void BinaryReadBuffer::leaveKey() throw(Exception) {

}

void BinaryReadBuffer::enterValue() throw(Exception) {

}

void BinaryReadBuffer::leaveValue() throw(Exception) {

}

void BinaryReadBuffer::leaveCollection() throw(Exception) {

}

void BinaryReadBuffer::leaveMap() throw(Exception) {

}

void BinaryReadBuffer::readVersion(int& version) throw(Exception) {
   m_pimpl->read(version);
}

void BinaryReadBuffer::readBits(char* data, long int length) throw(Exception) {
   size_t nbChars = length / 8 + (length % 8 == 0 ? 0 : 1);
   m_pimpl->read(data, nbChars);
}

void BinaryReadBuffer::readCollectionSize(std::size_t& size) throw(Exception) {
   m_pimpl->read(size);
}

void BinaryReadBuffer::readPointerId(int& id) throw(Exception) {
   m_pimpl->read(id);
}

void BinaryReadBuffer::readTypeId(std::string& val) throw(Exception) {
   m_pimpl->read(val);
}

void BinaryReadBuffer::read(bool& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(char& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(short& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(int& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(long& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(long long& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(unsigned char& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(unsigned short& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(unsigned int& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(unsigned long& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(unsigned long long& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(float& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(double& val) throw(Exception) {
   m_pimpl->read(val);
}


void BinaryReadBuffer::read(std::string& val) throw(Exception) {
   m_pimpl->read(val);
}

bool BinaryReadBuffer::reachedEnd() const {
   return m_pimpl->reachedEnd();
}


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
