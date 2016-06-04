
#include <ctrl/writePointerRepository.h>

using namespace ctrl::Private;

WritePointerRepository::WritePointerRepository()
   : m_nextIndex(1) {
}

bool WritePointerRepository::isRegistered(void* p) const {
   return m_indices.find(p) != m_indices.end();
}

int WritePointerRepository::get(void* p) const {
   return m_indices.at(p);
}

int WritePointerRepository::add(void* p) {
   m_indices[p] = m_nextIndex;
   return m_nextIndex++;
}

int WritePointerRepository::reserveIndex(void* p) {
   m_reserved[p] = m_nextIndex;
   return m_nextIndex++;
}

bool WritePointerRepository::isReserved(void* p) {
   return m_reserved.find(p) != m_reserved.end();
}

int WritePointerRepository::getReservedIndex(void* p) {
   return m_reserved.at(p);
}

void WritePointerRepository::clearReserved(void* p) {
   int index = m_reserved.at(p);
   m_reserved.erase(p);
   m_indices[p] = index;
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
