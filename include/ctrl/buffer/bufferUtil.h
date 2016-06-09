
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

#ifndef BUFFERUTIL_H_
#define BUFFERUTIL_H_

#include <string>
#include <sstream>
#include <ctrl/context.h>
#include <ctrl/properties.h>

namespace ctrl {

   class BufferUtil {
   public:
      std::string preferedMemberName(const MemberContext& context, const char* suggested = 0) {
         std::string alteredName  = context.getProperty<WithName>();
         if (suggested != 0) {
            return suggested;
         } else if (alteredName != "") {
            return alteredName;
         } else {
            return context.getName();
         }
      }

      template <class T>
      std::string toString(const T& val) {
         m_toString.str("");
         m_toString.clear();
         m_toString << val;
         return m_toString.str();
      }

      template <class T>
      T fromString(const std::string& str) {
         m_fromString.str(str);
         m_fromString.clear();
         T val;
         m_fromString >> val;
         return val;
      }

   private:
      std::ostringstream m_toString;
      std::istringstream m_fromString;
   };

} // namespace ctrl

#endif // BUFFERUTIL_H_