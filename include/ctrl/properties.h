
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

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>

#define CTRL_DEFINE_PROPERTY(PropertyId_, Type_, defaultValue_)                                                        \
struct PropertyId_ {                                                                                                   \
   typedef Type_ ValueType;                                                                                            \
   static Type_ defaultValue() { return defaultValue_; }                                                               \
};


#define CTRL_PROPERTY(PropertyId_, value_)                                                                             \
public:                                                                                                                \
   template <class Dummy_>                                                                                             \
   struct CTRL_IsPropertyPresent<__LINE__, Dummy_> {                                                                      \
      enum { value = true };                                                                                           \
      typedef PropertyId_ PropertyId;                                                                                  \
   };                                                                                                                  \
   static void* CTRL_getProperty(PropertyId_,                                                                             \
         ctrl::Private::Int2Type<ctrl::Private::GetMemberIndex                                                         \
                  <CTRL_ConcreteClass, CTRL_startLine, __LINE__>::index>) {                                                  \
      return reinterpret_cast<void*>(new PropertyId_::ValueType(value_));                                              \
   }                                                                                                                   \
                                                                                                                       \
   template <class Dummy_>                                                                                             \
   struct CTRL_HasProperty<PropertyId_, ctrl::Private::GetMemberIndex                                                     \
                  <CTRL_ConcreteClass, CTRL_startLine, __LINE__>::index, Dummy_> {                                           \
      enum { value = true };                                                                                           \
   };


namespace ctrl {

CTRL_DEFINE_PROPERTY(AsAttribute, bool, false)
#define CTRL_AS_ATTRIBUTE() CTRL_PROPERTY(ctrl::AsAttribute, true)

CTRL_DEFINE_PROPERTY(WithName, std::string, "")
#define CTRL_WITH_NAME(name_) CTRL_PROPERTY(ctrl::WithName, name_)

CTRL_DEFINE_PROPERTY(WithVersion, int, 1)
#define CTRL_WITH_VERSION(version_) CTRL_PROPERTY(ctrl::WithVersion, version_)

CTRL_DEFINE_PROPERTY(TypeIdFieldName, std::string, "__typeId")
#define CTRL_TYPE_ID_FIELD_NAME(name_) CTRL_PROPERTY(ctrl::TypeIdFieldName, name_)

CTRL_DEFINE_PROPERTY(TypeIdFieldAsAttribute, bool, false)
#define CTRL_TYPE_ID_FIELD_AS_ATTRIBUTE() CTRL_PROPERTY(ctrl::TypeIdFieldAsAttribute, true)

CTRL_DEFINE_PROPERTY(IdFieldName, std::string, "__id")
#define CTRL_ID_FIELD_NAME(name_) CTRL_PROPERTY(ctrl::IdFieldName, name_)

CTRL_DEFINE_PROPERTY(IdFieldAsAttribute, bool, false)
#define CTRL_ID_FIELD_AS_ATTRIBUTE() CTRL_PROPERTY(ctrl::IdFieldAsAttribute, true)

CTRL_DEFINE_PROPERTY(AsIdField, bool, false)
#define CTRL_AS_ID_FIELD() CTRL_PROPERTY(ctrl::AsIdField, true)

} // namespace ctrl


#endif // PROPERTIES_H_
