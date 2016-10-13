
/*
 * Copyright (C) 2010, 2012, 2013, 2015, 2016 by Gerrit Daniels <gerrit.daniels@gmail.com>
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

#ifndef REFLECTIONMACROS_H_
#define REFLECTIONMACROS_H_

#include <boost/shared_ptr.hpp>
#include <ctrl/typemanip.h>
#include <ctrl/createMemberIndices.h>
#include <ctrl/createBaseClassList.h>
#include <ctrl/createPropertyList.h>
#include <ctrl/getMemberIndex.h>
#include <ctrl/forwardSerialize.h>
#include <ctrl/polymorphicSerializer.h>
#include <ctrl/polymorphicFactory.h>
#include <ctrl/polymorphicFactoryImpl.h>
#include <ctrl/dynamic_reinterpret_cast.h>
#include <ctrl/buffer/abstractWriteBuffer.h>
#include <ctrl/buffer/abstractReadBuffer.h>
#include <ctrl/context.h>
#include <ctrl/properties.h>

#define CTRL_BEGIN_MEMBERS(ConcreteClass_)                                                                             \
private:                                                                                                               \
   typedef ConcreteClass_ CTRL_ConcreteClass;                                                                             \
   enum { CTRL_startLine = __LINE__ };                                                                                    \
public:                                                                                                                \
   ConcreteClass_() { }                                                                                                \
   template <class Dummy_>                                                                                             \
   static void initialize(Dummy_&, int) { }                                                                          \
                                                                                                                       \
   virtual ctrl::ClassContext CTRL_dynamicContext() {                                                                     \
      return ctrl::ClassContext(new ctrl::Private::ClassContextImpl<ConcreteClass_>());                                \
   }                                                                                                                   \
                                                                                                                       \
   static ctrl::ClassContext CTRL_staticContext() {                                                                       \
      return ctrl::ClassContext(new ctrl::Private::ClassContextImpl<ConcreteClass_>());                                \
   }                                                                                                                   \
                                                                                                                       \
   virtual std::string CTRL_dynamicName() {                                                                               \
      std::string withName = CTRL_dynamicContext().getProperty<ctrl::WithName>();                                         \
      return withName == "" ? #ConcreteClass_ : withName;                                                              \
   }                                                                                                                   \
                                                                                                                       \
   static std::string CTRL_staticName() {                                                                                 \
      std::string withName = CTRL_staticContext().getProperty<ctrl::WithName>();                                          \
      return withName == "" ? #ConcreteClass_ : withName;                                                              \
   }                                                                                                                   \
                                                                                                                       \
   virtual void CTRL_serialize(ctrl::AbstractWriteBuffer& buffer, int version, const ctrl::Context& context) const {      \
      ctrl::Private::serialize(*this, buffer, version, context);                                                       \
   }                                                                                                                   \
   static int CTRL_registerDeserializer[];                                                                                \
                                                                                                                       \
   template <int lineNb_, class Dummy_ = ctrl::Private::NullType>                                                      \
   struct CTRL_IsMemberPresent {                                                                                          \
      enum { value = false };                                                                                          \
      enum { index = 0 };                                                                                              \
   };                                                                                                                  \
                                                                                                                       \
   template <int index_, class Dummy_ = ctrl::Private::NullType>                                                       \
   struct CTRL_MemberType {                                                                                               \
      typedef ctrl::Private::NullType Type;                                                                            \
   };                                                                                                                  \
                                                                                                                       \
   template <int lineNb_, class Dummy_ = ctrl::Private::NullType>                                                      \
   struct CTRL_IsBaseClassPresent {                                                                                       \
      enum { value = false };                                                                                          \
      typedef ctrl::Private::NullType BaseClass;                                                                       \
   };                                                                                                                  \
                                                                                                                       \
   template <int lineNb_, class Dummy_ = ctrl::Private::NullType>                                                      \
   struct CTRL_IsPropertyPresent {                                                                                        \
      enum { value = false };                                                                                          \
      typedef ctrl::Private::NullType PropertyId;                                                                      \
   };                                                                                                                  \
                                                                                                                       \
   template <class PropertyId_, int index_>                                                                            \
   static void* CTRL_getProperty(PropertyId_, ctrl::Private::Int2Type<index_>) {                                          \
      return 0;                                                                                                        \
   }                                                                                                                   \
                                                                                                                       \
   template <class PropertyId_, int index_, class Dummy_ = ctrl::Private::NullType>                                    \
   struct CTRL_HasProperty {                                                                                              \
      enum { value = false };                                                                                          \
   };


#define CTRL_MEMBER(access, DataType, name)                                                                            \
access:                                                                                                                \
   DataType name;                                                                                                      \
private:                                                                                                               \
   typedef DataType CTRL_ConcreteClass::* CTRL_MemberPtr##name;                                                              \
public:                                                                                                                \
   template <class Dummy_>                                                                                             \
   struct CTRL_IsMemberPresent<__LINE__, Dummy_>  {                                                                       \
      enum { value = true };                                                                                           \
      enum { index = ctrl::Private::GetMemberIndex<CTRL_ConcreteClass, CTRL_startLine, __LINE__ - 1>::index + 1 };           \
   };                                                                                                                  \
                                                                                                                       \
   template <class Dummy_ >                                                                                            \
   struct CTRL_MemberType<CTRL_IsMemberPresent<__LINE__>::index, Dummy_> {                                                   \
      typedef DataType Type;                                                                                           \
   };                                                                                                                  \
   static CTRL_MemberPtr##name CTRL_getMemberPtr(                                                                            \
         ctrl::Private::Int2Type< CTRL_IsMemberPresent<__LINE__>::index >) {                                              \
      return &CTRL_ConcreteClass::name;                                                                                   \
   }                                                                                                                   \
   static const char* CTRL_getMemberName(                                                                                 \
         ctrl::Private::Int2Type< CTRL_IsMemberPresent<__LINE__>::index >) {                                              \
      return #name;                                                                                                    \
   }


#define CTRL_BASE_CLASS(BaseClass_)                                                                                    \
public:                                                                                                                \
   template <class Dummy_>                                                                                             \
   struct CTRL_IsBaseClassPresent<__LINE__, Dummy_> {                                                                     \
      enum { value = true };                                                                                           \
      typedef BaseClass_ BaseClass;                                                                                    \
   };


#define CTRL_END_MEMBERS()                                                                                             \
private:                                                                                                               \
   enum { CTRL_endLine = __LINE__ };                                                                                      \
public:                                                                                                                \
   typedef ctrl::Private::CreateMemberIndices<CTRL_startLine, CTRL_endLine, CTRL_ConcreteClass>                                 \
              ::Indices CTRL_MemberIndices;                                                                               \
   typedef ctrl::Private::CreateBaseClassList<CTRL_startLine, CTRL_endLine, CTRL_ConcreteClass>                                 \
              ::TList CTRL_BaseClasses;                                                                                   \
   typedef ctrl::Private::CreatePropertyList<CTRL_startLine, CTRL_endLine, CTRL_ConcreteClass>                                  \
              ::TList CTRL_KnownProperties;


#define CTRL_POLYMORPH_REGISTER_FACTORY(ClassName_)                                                                    \
   ctrl::Private::PolymorphicSerializer::instance().registerDeserialize(                                               \
         ClassName_::CTRL_staticName(), ctrl::Private::PolymorphicFactory(                                                \
         boost::shared_ptr<ctrl::Private::PolymorphicFactory::Impl>                                                    \
         (new ctrl::Private::PolymorphicFactoryImpl<ClassName_>())))


#define CTRL_ABSTRACT_POLYMORPH(ClassName_)                                                                            \
int ClassName_::CTRL_registerDeserializer[] = {                                                                           \
   ctrl::Private::PolymorphicSerializer::instance()                                                                    \
      .registerAbstract(ClassName_::CTRL_staticName())                                                                    \
};

#define CTRL_POLYMORPH(ClassName_)                                                                                     \
int ClassName_::CTRL_registerDeserializer[] = {                                                                           \
   CTRL_POLYMORPH_REGISTER_FACTORY(ClassName_)                                                                         \
};


#define CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_)                                                                \
   ctrl::Private::PolymorphicSerializer::instance().registerCast(                                                      \
         T0_::CTRL_staticName(), T1_::CTRL_staticName(), &ctrl::Private::dynamic_reinterpret_cast<T0_, T1_>),                \
   ctrl::Private::PolymorphicSerializer::instance().registerCast(                                                      \
         T1_::CTRL_staticName(), T0_::CTRL_staticName(), &ctrl::Private::dynamic_reinterpret_cast<T1_, T0_>)


#define CTRL_POLYMORPH_MULTIPLE_2(Class_, T0_, T1_)                                                                    \
int Class_::CTRL_registerDeserializer[] = {                                                                               \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                            \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_)                                                                     \
};

#define CTRL_POLYMORPH_MULTIPLE_3(Class_, T0_, T1_, T2_)                                                               \
int Class_::CTRL_registerDeserializer[] = {                                                                               \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                            \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_)                                                                     \
};

#define CTRL_POLYMORPH_MULTIPLE_4(Class_, T0_, T1_, T2_, T3_)                                                          \
int Class_::CTRL_registerDeserializer[] = {                                                                               \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                            \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_)                                                                     \
};

#define CTRL_POLYMORPH_MULTIPLE_5(Class_, T0_, T1_, T2_, T3_, T4_)                                                     \
int Class_::CTRL_registerDeserializer[] = {                                                                               \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                            \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T4_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T4_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T4_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T4_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T4_)                                                                     \
};

#define CTRL_POLYMORPH_MULTIPLE_6(Class_, T0_, T1_, T2_, T3_, T4_, T5_)                                                \
int Class_::CTRL_registerDeserializer[] = {                                                                               \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                            \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T4_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T5_),                                                                 \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T4_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T5_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T4_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T5_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T4_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T5_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T4_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T5_),                                                                    \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T4_, T5_)                                                                     \
};

#endif /* REFLECTIONMACROS_H_ */
