#ifndef REFLECTIONMACROS_H_
#define REFLECTIONMACROS_H_

#include <boost/shared_ptr.hpp>
#include <ctrl/createMemberIndices.h>
#include <ctrl/createBaseClassList.h>
#include <ctrl/getMemberIndex.h>
#include <ctrl/forwardSerialize.h>
#include <ctrl/polymorphicSerializer.h>
#include <ctrl/polymorphicFactory.h>
#include <ctrl/polymorphicFactoryImpl.h>
#include <ctrl/dynamic_reinterpret_cast.h>
#include <ctrl/writeBuffer.h>
#include <ctrl/readBuffer.h>
#include <ctrl/typemanip.h>

#define CTRL_BEGIN_MEMBERS(ConcreteClass_)                                                                              \
private:                                                                                                                \
   typedef ConcreteClass_ __ConcreteClass;                                                                              \
   enum { __startLine = __LINE__ };                                                                                     \
public:                                                                                                                 \
   ConcreteClass_() { }                                                                                                 \
   template <class Dummy_>                                                                                              \
   static void __initialize(Dummy_&, int) { }                                                                           \
                                                                                                                        \
   virtual const char* __className() { return #ConcreteClass_; }                                                        \
   static const char* __staticClassName() { return #ConcreteClass_; }                                                   \
                                                                                                                        \
   virtual void __serialize(ctrl::Private::AbstractWriteBuffer& buffer, int version) const {                            \
      ctrl::Private::serialize(*this, buffer, version);                                                                 \
   }                                                                                                                    \
   static int __registerDeserializer[];                                                                                 \
                                                                                                                        \
   template <int lineNb_, class Dummy_ = ctrl::Private::NullType>                                                       \
   struct __IsMemberPresent {                                                                                           \
      enum { value = false };                                                                                           \
      enum { index = 0 };                                                                                               \
   };                                                                                                                   \
                                                                                                                        \
   template <int index_, class Dummy_ = ctrl::Private::NullType>                                                        \
   struct __Version {                                                                                                   \
      enum { version = 1 };                                                                                             \
   };                                                                                                                   \
                                                                                                                        \
   template <int lineNb_, class Dummy_ = ctrl::Private::NullType>                                                       \
   struct __IsBaseClassPresent {                                                                                        \
      enum { value = false };                                                                                           \
      typedef ctrl::Private::NullType BaseClass;                                                                        \
   };


#define CTRL_MEMBER(access, DataType, name)                                                                             \
access:                                                                                                                 \
   DataType name;                                                                                                       \
private:                                                                                                                \
   typedef DataType __ConcreteClass::* __MemberPtr##name;                                                               \
public:                                                                                                                 \
   template <class Dummy_>                                                                                              \
   struct __IsMemberPresent<__LINE__, Dummy_>  {                                                                        \
      enum { value = true };                                                                                            \
      enum { index = ctrl::Private::GetMemberIndex<__ConcreteClass, __startLine, __LINE__ - 1>::index + 1 };            \
   };                                                                                                                   \
   static __MemberPtr##name __getMemberPtr(                                                                             \
         ctrl::Private::Int2Type< __IsMemberPresent<__LINE__>::index >) {                                               \
      return &__ConcreteClass::name;                                                                                    \
   }\
   static const char* __getMemberName(                                                                             \
         ctrl::Private::Int2Type< __IsMemberPresent<__LINE__>::index >) {                                               \
      return #name;                                                                                    \
   }


#define CTRL_WITH_VERSION(version_)                                                                                     \
public:                                                                                                                 \
   enum {                                                                                                               \
      __previousMember##__LINE__ = ctrl::Private::GetMemberIndex<__ConcreteClass, __startLine, __LINE__>::index         \
   };                                                                                                                   \
   template <class Dummy_>                                                                                              \
   struct __Version<__previousMember##__LINE__, Dummy_> {                                                               \
      enum { version = version_ };                                                                                      \
   };



#define CTRL_BASE_CLASS(BaseClass_)                                                                                     \
public:                                                                                                                 \
   template <class Dummy_>                                                                                              \
   struct __IsBaseClassPresent<__LINE__, Dummy_> {                                                                      \
      enum { value = true };                                                                                            \
      typedef BaseClass_ BaseClass;                                                                                     \
   };


#define CTRL_END_MEMBERS()                                                                                              \
private:                                                                                                                \
   enum { __endLine = __LINE__ };                                                                                       \
public:                                                                                                                 \
   typedef ctrl::Private::CreateMemberIndices<__startLine, __endLine, __ConcreteClass>                                  \
              ::Indices __MemberIndices;                                                                                \
   typedef ctrl::Private::CreateBaseClassList<__startLine, __endLine, __ConcreteClass>                                  \
              ::TList __BaseClasses;


#define CTRL_POLYMORPH_REGISTER_FACTORY(ClassName_)                                                                     \
   ctrl::Private::PolymorphicSerializer::instance().registerDeserialize(                                                \
         #ClassName_, ctrl::Private::PolymorphicFactory(                                                                \
         boost::shared_ptr<ctrl::Private::PolymorphicFactory::Impl>                                                     \
         (new ctrl::Private::PolymorphicFactoryImpl<ClassName_>())))


#define CTRL_ABSTRACT_POLYMORPH(ClassName_)                                                                             \
int ClassName_::__registerDeserializer[] = {                                                                            \
   ctrl::Private::PolymorphicSerializer::instance()                                                                     \
      .registerAbstract(#ClassName_)                                                                                    \
};

#define CTRL_POLYMORPH(ClassName_)                                                                                      \
int ClassName_::__registerDeserializer[] = {                                                                            \
   CTRL_POLYMORPH_REGISTER_FACTORY(ClassName_)                                                                          \
};


#define CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_)                                                                 \
   ctrl::Private::PolymorphicSerializer::instance().registerCast(                                                       \
         #T0_, #T1_, &ctrl::Private::dynamic_reinterpret_cast<T0_, T1_>),                                               \
   ctrl::Private::PolymorphicSerializer::instance().registerCast(                                                       \
         #T1_, #T0_, &ctrl::Private::dynamic_reinterpret_cast<T1_, T0_>)


#define CTRL_POLYMORPH_MULTIPLE_2(Class_, T0_, T1_)                                                                     \
int Class_::__registerDeserializer[] = {                                                                                \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                             \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_)                                                                      \
};

#define CTRL_POLYMORPH_MULTIPLE_3(Class_, T0_, T1_, T2_)                                                                \
int Class_::__registerDeserializer[] = {                                                                                \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                             \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_)                                                                      \
};

#define CTRL_POLYMORPH_MULTIPLE_4(Class_, T0_, T1_, T2_, T3_)                                                           \
int Class_::__registerDeserializer[] = {                                                                                \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                             \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_)                                                                      \
};

#define CTRL_POLYMORPH_MULTIPLE_5(Class_, T0_, T1_, T2_, T3_, T4_)                                                      \
int Class_::__registerDeserializer[] = {                                                                                \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                             \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T4_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T4_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T4_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T4_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T4_)                                                                      \
};

#define CTRL_POLYMORPH_MULTIPLE_6(Class_, T0_, T1_, T2_, T3_, T4_, T5_)                                                 \
int Class_::__registerDeserializer[] = {                                                                                \
   CTRL_POLYMORPH_REGISTER_FACTORY(Class_),                                                                             \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T4_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T5_),                                                                  \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T4_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T5_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T4_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T5_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T4_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T5_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T4_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T5_),                                                                     \
   CTRL_POLYMORPH_MULTIPLE_REGISTER_CAST(T4_, T5_)                                                                      \
};

#endif /* REFLECTIONMACROS_H_ */

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
