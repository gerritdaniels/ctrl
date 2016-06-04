#include <ctrl/writeBuffer.h>

using namespace ctrl::Private;

const char* WriteBuffer::Impl::s_padding = "\0\0\0\0\0\0";

WriteBuffer::Impl::Impl()
   : m_data(new char[10])
   , m_capacity(10)
   , m_length(0)
   , m_owner(true) {

}

WriteBuffer::Impl::~Impl() {
   if (m_owner) delete[] m_data;
}

long WriteBuffer::Impl::length() {
   return m_length;
}

char* WriteBuffer::Impl::getData() {
   m_owner = false;
   return m_data;
}

void WriteBuffer::Impl::appendNoPadding(const char* data, long length) {
   long newLength = m_length + length;
   if (newLength > m_capacity)
      realloc(newLength);

   for (long i = m_length; i < newLength; ++i)
      m_data[i] = data[i - m_length];

   m_length += length;
}

void WriteBuffer::Impl::realloc(long newLength) {
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

WriteBuffer::WriteBuffer(WriteBuffer::Impl* pimpl)
   : m_pimpl(pimpl) {

}

long WriteBuffer::length() {
   return m_pimpl->length();
}

char* WriteBuffer::getData() {
   return m_pimpl->getData();
}

void WriteBuffer::append(const char* data, long length) {
   m_pimpl->append(data, length);
}

WritePointerRepository& WriteBuffer::getPointerRepository() {
   return m_pointerRepository;
}

/*
 * Copyright (C) 2010, 2012 by Gerrit Daniels <gerrit.daniels@gmail.com>
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
