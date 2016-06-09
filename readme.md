# C++ Template Reflection Library

CTRL adds data member reflection to C++ using template meta-programming techniques and the standard C++ preprocessor. 
This reflection is used to implement serialize and deserialize functions. Both binary and XML formats are supported. 
Since it uses the standard preprocessor no special preprocessor is needed to use CTRL.

## Requirements

This library depends on the [boost](http://www.boost.org/) library. It also depends on 
[rapidxml](http://rapidxml.sourceforge.net/), but it is included in the distribution, so no download is required.

## Instalation

On Linux systems:

```sh
$ cmake

$ cd build

$ make

$ sudo make install
```

## Tutorial

__Important__: each CTRL macro has to be placed on its own line.

### Composition

To create a serializeable class you need to use the following macro's to define the members.

```cpp
class SimpleClass
{
     CTRL_BEGIN_MEMBERS(SimpleClass)
     CTRL_MEMBER(private, int, m_count)
     CTRL_MEMBER(private, std::string, m_name)
     CTRL_END_MEMBERS()
};
```

The CTRL_BEGIN_MEMBERS macro defines a default constructor so you can't define it yourself. You can put these macro's 
anywhere in the class and not every line between CTRL_BEGIN_MEMBERS and CTRL_END_MEMBERS needs to contain a call to 
CTRL_MEMBER. So a CTRL_MEMBER call can take up multiple lines. You can use all the basic types and std::string and 
std::wstring as members. You can also use smart pointers and standard containers, these are explained later on. First 
lets look at the serialize and deserialize functions.

```cpp
int main(void)
{
    SimpleClass obj;
    
    long length = 0;
    char* data = ctrl::toBinary(obj, length);
    SimpleClass* ptr = ctrl::fromBinary<SimpleClass>(data, length);
    delete ptr; 
    delete[] data;
    
    std::string str = ctrl::toXml(obj);
    ptr = ctrl::fromXml<SimpleClass>(str);
    delete ptr; 
}
```

As you can see the toBinary function takes a const reference to the object and a reference to a long as input 
parameters. This long is filled in to give the length of the serialized data. The fromBinary function returns a 
pointer to an on heap allocated object. It's up to you to do the memory management of this object. If the amount of 
data that is consumed by fromBinary doesn't match the length that is passed in, an exception is thrown.

The toXml function takes a const reference to the object and opionally a boolean indicating that pretty printing should 
be used. The fromXml function takes a const string reference as argument and again returns an on heap allocated
object.

You can also use composition.

```cpp
class CompositeClass
{
    CTRL_BEGIN_MEMBERS(CompositeClass)
    CTRL_MEMBER(private, SimpleClass, m_simple)
    CTRL_MEMBER(private, int, m_value)
    CTRL_END_MEMBERS()
};
```

As smart pointers you can use std::auto_ptr, std::unique_ptr, std::shared_ptr, std::weak_ptr, boost::shared_ptr and 
boost::weak_ptr. Of course you can also use raw pointers.

```cpp
class PointerClass
{
    CTRL_BEGIN_MEMBERS(PointerClass)
    CTRL_MEMBER(private, std::auto_ptr<CompositeClass>, m_auto)
    CTRL_MEMBER(private, boost::shared_ptr<SimpleClass>, m_shared)
    CTRL_MEMBER(private, boost::weak_ptr<SimpleClass>, m_weak)
    CTRL_END_MEMBERS()
};
```

All standard containers are supported.

```cpp
class ContainerClass
{
    CTRL_BEGIN_MEMBERS(ContainerClass)
    CTRL_MEMBER(private, std::vector<SimpleClass>, m_vector)
    CTRL_MEMBER(private, std::list<CompositeClass>, m_list)

    typedef std::map<std::string, SimpleClass> SimpleMap;

    CTRL_MEMBER(private, SimpleMap, m_map)
    CTRL_END_MEMBERS()
};
```

As you can see the example for the map is a bit more involving, you need to define a typedef first. This is always the 
case if you want to use types that have multiple template parameters. You can combine all types of members, so you can 
have a vector of shared_ptr.

Furthermore std::pair, std::complex and std::valarray are also supported.

### Inheritance

If you want to use inheritance you need to add an extra macro to define the base class.


```cpp
class DerivedClass : public CompositeClass
{
    CTRL_BEGIN_MEMBERS(DerivedClass)
    CTRL_BASE_CLASS(CompositeClass)
    CTRL_MEMBER(private, std::string, m_text)
    CTRL_END_MEMBERS()
};
```

If you want to use a derived class in a polymorphic way, eg. store pointers to the base class, you need to use the 
macros CTRL_POLYMORPH (for publicly constructable classes) and CTRL_ABSTRACT_POLYMOPRH (for abstract and non publicly 
constructable classes). These have to be put in the *.cpp file.

```cp
CTRL_POLYMORPH(CompositeClass)

CTRL_POLYMORPH(DerivedClass)
```

You can also use multiple inheritance. For this you simply need to add an extra CTRL_BASE_CLASS macro.

```cpp
class MultipleClass : public CompositeClass, public PointerClass
{
    CTRL_BEGIN_MEMBERS(MultipleClass)
    CTRL_BASE_CLASS(CompositeClass)
    CTRL_BASE_CLASS(PointerClass)
    CTRL_MEMBER(private, std::string, m_text)
    CTRL_END_MEMBERS()
};
```

If you want to use multiple inheritance in a polymorphic way, it gets a bit more complicated. You still need to call 
CTRL_POLYMORPH for all the base classes (that can be publicly constructed), but for the derived class you need to 
call a special macro CTRL_POLYMORPH_MULTIPLE_X where X is the number of direct and indirect base classes. The first 
argument of this macro is the derived class, the following arguments are all the base classes in any order.

```cpp
CTRL_POLYMORPH(CompositeClass)

CTRL_POLYMORPH(PointerClass)

CTRL_POLYMORPH_MULTIPLE_2(MultipleClass, CompositeClass, PointerClass)
```

### Initialization

If you need to do some initialization after an object is deserialized, you can do this by adding a static __initialize 
member function to the class.

```cpp
class InitializeClass
{
public:
    static void __initialize(InitializeClass& obj, int version)
    {
        obj.m_sum = obj.m_val0 + obj.m_val1;
    }

    CTRL_BEGIN_MEMBERS(InitializeClass)
    CTRL_MEMBER(private, int, m_val0)
    CTRL_MEMBER(private, int, m_val1)
    CTRL_END_MEMBERS()

private:
    int m_sum;
};
```

This initialize function is called for all the base classes in the correct order.

### Binary serialization features

You can also set the memory alignment and endian type when you call a toBinary or fromBinary function. Using the 
platform memory alignment (the default) gives the best performance but results in larger files. For the smallest file 
use an alignment of 1. For endian support you can use the macro's CTRL_LITTLE_ENDIAN and CTRL_BIG_ENDIAN.

```cpp
int main(void)
{
    SimpleClass obj;
    long length = 0;
    char* data = ctrl::toBinary<4, CTRL_LITTLE_ENDIAN>(obj, length);
    SimpleClass* ptr = ctrl::fromBinary<SimpleClass, 4, CTRL_LITTLE_ENDIAN>(data, length);
    delete ptr; 
    delete[] data; 
}
```

### XML serialization features

You can control the serialization to XML by adding serialization properties to your members. These properties must
always be added after the member declaration. To set class level properties place the macros right after the call to
CTRL_BEGIN_MEMBERS.

```cpp
class CustomNames
{
    CTRL_BEGIN_MEMBERS(CustomNames)
        CTRL_WITH_NAME("custom")
    CTRL_MEMBER(private, int, m_val)
        CTRL_WITH_NAME("val")
    CTRL_MEMBER(private, std::string, m_text)
    CTRL_END_MEMBERS()

private:
    int m_sum;
};
```

#### CTRL_WITH_NAME(_name_)

The with name macro on a member controls the name used for the xml element. On a class it controls the type id used 
when the class is polymorph.

#### CTRL_AS_ATTRIBUTE()

This macro is only relevant for members that are of a fundamental type or of type std::string or wstring. When used the
member gets serialized as xml attribute instead of as an element.

#### CTRL_TYPE_ID_FIELD_NAME(_name_)

This macro has to be used on the root of a single inheritance hierarchy (on the class, not a member), if used with 
multiple inheritance it results in an error. It controls the name of the element that is used to store the type id in 
the case of polymorphic classes.

#### CTRL_TYPE_ID_FIELD_AS_ATTRIBUTE()

This macro is also an inheritance root property. It serializes the type id field as attribute instead of as element.

#### CTRL_TYPE_ID_FIELD_NAME(_name_)

This macro is also an inheritance root property. Sets the name used for the id field of pointer types.

#### CTRL_TYPE_ID_FIELD_AS_ATTRIBUTE()

This macro is also an inheritance root property. It serializes the id field as attribute instead of as element.

#### CTRL_AS_ID_FIELD()

This macro is an inheritance root property, but it has to be set on a member, not the class. When used that member is
used as the id field. It is up to you to ensure that the id's of different objects are unique.

### Versioning

You can also use versioning with CTRL. For this you use the property CTRL_VERSION.

```cpp
class SimpleClass
{
    CTRL_BEGIN_MEMBERS(SimpleClass)
    CTRL_MEMBER(private, int, m_count)
    CTRL_MEMBER(private, std::string, m_name)
    CTRL_MEMBER(private, float, m_factor)
        CTRL_VERSION(2)
    CTRL_END_MEMBERS()
};

int main(void)
{
    SimpleClass obj;
    long length = 0;
    char* data = ctrl::toBinary(obj, length, 1);
    SimpleClass* ptr = ctrl::fromBinary<SimpleClass>(data, length, 1);
    delete ptr; 
    delete[] data; 
}
```

In this example the float doesn't get serialized or deserialized because it has version 2. You can still initialize it 
afterwards with an initialize function. 

## Template meta-programming

The template mechanism in C++ is turing complete. You can use typedefs and enums as variables, with recursive 
instantiation you can create repition structures and with template specialization you can create choice structures.
Lets define some concepts that we will use later on.

### Int2Type

An Int2Type is simply a class that takes an integer as template parameter. It can be used to enumerate types.

```cpp
template <int i_>
class Int2Type { };
```

### TypeList

A TypeList is a template class with two type argements that are typedefed as Head and Tail. Head is used as the current
element in the list, Tail is the remainder of the list and thus another TypeList instantiation. A TypeList is terminated
by a NullType.

```cpp
template <class Head_, class Tail_>
class TypeList {
public:
    typedef Head_ Head;
    typedef Tail_ Tail;
};

class NullType { };
```

A TypeList can be traversed by a recursive template instantiation.

```cpp
class RecursiveCall {
public:
    template <class TypeList_>
    void call(TypeList_ list) {
        // do something with TypeList_::Head
        RecursiveCall::call(TypeList_::Tail());
    }
    
    void call(NullType list) {
        // empty implementation terminates the recursive call
    }
};
```

### Select

A Select is used to choose between two types based on a boolean condition. It takes an integer and two types as template
parameters. The default implementation typedefs the first type as the Result. A specialization that defines the integer
as false typedefs the second.

```cpp
template <int condition_, class TrueType_, class FalseType_>
class Select {
public:
    typedef TrueType_ Result;
};
    
template <class TrueType_, class FalseType_>
class Select<false, TrueType_, FalseType_> {
public:
    typedef FalseType_ Result;
};    
```

## Design

Our goal is to achieve transparent serialization of user defined types. To achieve this we need a way to access the 
members of an object without knowing the number of members and the names of these members. Accessing a member without 
using its name can be done using a pointer to member. For each member we could add a 'getMemberPtr' function to the 
user class. To differenciate between the getMemberPtr functions we could give each a unique Int2Type as parameter.
If we store these Int2Types in a TypeList we can use a recursive template algorithm to traverse them.

Lets try to define preprocessor macros to implement this functionality. We can make some observations. We need a macro 
to add a member. Since this macro will need the name of the user class (to define the pointer to member), it would 
be usefull to pass this name in a separate macro. Finally we also need a macro to add the type list. Lets name these 
macros BEGIN_MEMBERS, ADD_MEMBER and END_MEMBERS.

```cpp
#define BEGIN_MEMBERS(ConcreteClass_)        \
private:                                     \
   typedef ConcreteClass_ ConcreteClass;
   

#define ADD_MEMBER(DataType, name, nb)                      \
private:                                                    \
   DataType name;                                           \
   typedef DataType ConcreteClass::* MemberPtr##name;       \
public:                                                     \
   static MemberPtr##nb getMemberPtr(Int2Type<nb>)          \
   {                                                        \
      return &ConcreteClass::name;                          \
   }
```

The END_MEMBERS macro will need to create the TypeList typedef. If we enforce that the user has to enumerate his 
members starting with zero, a recursive template algorithm taking the number of members could create the TypeList.

```cpp
#define END_MEMBERS(nb)                                              \
public:                                                              \
   typedef CreateMemberIndices<nb>::Result MemberIndices;


template <class TList_, int nb_>
struct CreateMemberIndicesImpl
{
   typedef Typelist<Int2Type<nb_>, TList_> NewTList;
   
   typedef typename CreateMemberIndicesImpl<NewTList, nb_ - 1>::Result Result;
};

template <class TList_>
struct CreateMemberIndicesImpl<TList_, 0>
{
   typedef Typelist<Int2Type<0>, TList_> Result;
};


template <int nb_>
struct CreateMemberIndices
{
   typedef typename CreateMemberIndicesImpl<NullType, nb_ - 1>::Result Result;
};
```

The serialize function will have to use a recursive call to serialize all members.

```cpp
template <class TList_>
class RecursiveSerialization
{
public:
   template <class UserClass_>
   static void serialize(ostream& stream, UserClass_* ptr)
   {
      serialize(ptr->* UserClass_::getMemberPtr(typename TList_::Head()));
      RecursiveSerialization<typename TList_::Tail>::serialize(stream, ptr);
   }
}

template <>
class RecursiveSerialization<NullType>
{
public:
   template <class UserClass_>
   static void serialize(ostream& stream, UserClass_* ptr)
   {

   }
}

template <class UserClass_>
void serialize(ostream& stream, UserClass_* ptr)
{
   RecursiveSerialization<typename UserClass_::MemberIndices>::serialize(stream, ptr);
}
```

In the previous code, when serialize is called for the RecursiveSerialization instantiation, it calls serialize for 
each element in the TypeList.

We can now use these macros as follows.

```cpp
class UserClass
{
   BEGIN_MEMBERS(UserClass)
   ADD_MEMBER(int, m_count, 0)
   ADD_MEMBER(std::string, m_name, 1)
   END_MEMBERS(2)
};
```

This already is quite simple, but it isn't simple enough. In particular, the user has to pass a number to each 
ADD_MEMBER call. Also, each time a new member is added to a user class the call to END_MEMBERS needs to be changed. 
Lets see if we can't simplify this further.

Instead of passing the number to ADD_MEMBER, the macro could use the line number as the unique integer. This means that 
each ADD_MEMBER call must have its own line, but that isn't a problem.

```cpp
#define ADD_MEMBER(DataType, name)                                \
private:                                                          \
   DataType name;                                                 \
   typedef DataType ConcreteClass::* MemberPtr##name;             \
public:                                                           \
   static MemberPtr##name getMemberPtr(Int2Type<__LINE__>)        \
   {                                                              \
      return &ConcreteClass::name;                                \
   }
```

If we want to use this macro we will need another mechanism to create the TypeList. Since all ADD_MEMBER macros are 
enclosed within the BEGIN_MEMBERS and END_MEMBERS calls, we could let them create two enums, startLine and endLine 
respectively, holding the line number. These could then be passed to the template algorithm instanciated by 
END_MEMBERS. The new template algorithm looks something like this.

```cpp
template <class TList_, int startLine_, int endLine_>
struct CreateMemberIndicesImpl
{
   typedef Loki::Typelist<Loki::Int2Type<startLine_>, TList_> NewTList;
   
   typedef typename CreateMemberIndicesImpl< NewTList, 
                                             startLine_ + 1, 
                                             endLine_ >::Result Result;
};

template <class TList_, int endLine_>
struct CreateMemberIndicesImpl<TList_, endLine_, endLine_>
{
   typedef TList_ Result;
};


template <int startLine_, int endLine_>
struct CreateMemberIndices
{
   typedef typename CreateMemberIndicesImpl< Loki::NullType, 
                                             startLine_ + 1, 
                                             endLine_ >::Result Result;
};
```

Using this algorithm, every line between the begin and end members macros must contain an ADD_MEMBER call. This means 
that no single ADD_MEMBER call may occupy two lines. This isn't flexible enough, so we need a mechanism to check line 
numbers before adding them to the TypeList.

Template specialization could do the trick here. If the BEGIN_MEMBERS macro would create a template class 
IsMemberPresent, taking an integer and defining an enum as false; then ADD_MEMBER could specialize it, defining the 
enum as true. But since explicit template specialization isn't allowed inside class bodies, we will need to use partial 
template specialization. Furthermore, if the CreateMemberIndices algorithm wants to access this class it will also need 
the user class type. These are the new macros:

```cpp
#define BEGIN_MEMBERS(ConcreteClass_)                                      \
private:                                                                   \
   typedef ConcreteClass_ ConcreteClass;                                   \
   enum { startLine = __LINE__ };                                          \
public:                                                                    \
   template <int lineNb_, class Dummy_ = NullType>                         \
   struct IsMemberPresent { enum value = false };


#define ADD_MEMBER(DataType, name)                                         \
private:                                                                   \
   DataType name;                                                          \
   typedef DataType ConcreteClass::* MemberPtr##name;                      \
public:                                                                    \
   template <class Dummy_>                                                 \
   struct IsMemberPresent<__LINE__, Dummy_> { enum value = true };         \
   static MemberPtr##name getMemberPtr(Int2Type<__LINE__>)                 \
   {                                                                       \
      return &ConcreteClass::name;                                         \
   }


#define END_MEMBERS()                                                      \
private:                                                                   \
   enum { endLine = __LINE__ };                                            \
public:                                                                    \
   typedef CreateMemberIndices<startLine, endLine, ConcreteClass>::Result  \
           MemberIndices;
```

The following code shows the altered CreateMemberIndices.

```cpp
template <class TList_, int startLine_, int endLine_, class ConcreteClass_>
struct CreateMemberIndicesImpl
{
   enum { isMemberPresent = ConcreteClass_::IsMemberPresent<startLine_>::value };
   
   typedef typename Select< isMemberPresent, 
                            Typelist<Int2Type<startLine_>, TList_>, 
                            TList_ >::Result NewTList;
   
   typedef typename CreateMemberIndicesImpl< NewTList, 
                                             startLine_ + 1, 
                                             endLine_, 
                                             ConcreteClass_ >::Result Result;
};

template <class TList_, int endLine_, class ConcreteClass_>
struct CreateMemberIndicesImpl<TList_, endLine_, endLine_, ConcreteClass_>
{
   typedef TList_ Result;
};


template <int startLine_, int endLine_, class ConcreteClass_>
struct CreateMemberIndices
{
   typedef typename CreateMemberIndicesImpl< Loki::NullType, 
                                             startLine_ + 1, 
                                             endLine_, 
                                             ConcreteClass_ >::Result Result;
};
```

And here is the new user code:

```cpp
class UserClass
{
   BEGIN_MEMBERS(UserClass)
   ADD_MEMBER(int, m_count)
   ADD_MEMBER(std::string, m_name)
   END_MEMBERS()
};
```
