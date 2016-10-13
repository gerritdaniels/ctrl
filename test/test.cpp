
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

#include <iostream>
#include <iomanip>
#include <string>
#include <ctrl/ctrl.h>

void writeBytes(const char* bytes, const long& length) {
   std::cout << std::hex;
   int column = 0;
   for (long i = 0; i < length; ++i) {
      std::cout << std::setw(2) << std::setfill('0') << std::right
                << int((unsigned char)(bytes[i])) << ' ';
      ++column;
      if (column % 4 == 0 && column != 16)
         std::cout << "| ";
      if (column == 16 && i != length - 1) {
         std::cout << std::endl;
         column = 0;
      }
   }
   std::cout << std::endl << std::endl;
}

template <class T>
bool testAndDelete(T* newObj, const T& obj, char* bytes) {
   bool success = false;
   if (*newObj == obj) {
      success = true;
   }
   delete newObj;
   if (bytes != 0) {
      delete[] bytes;
   }
   return success;
}

template <class T>
bool testSerialization(const T& obj, bool multimapJsonException = false) {
   long length;
   char* bytes = ctrl::toBinary(obj, length);
   writeBytes(bytes, length);
   T* newObj = ctrl::fromBinary<T>(bytes, length);
   if (!testAndDelete(newObj, obj, bytes)) {
      return false;
   }

   std::string xml = ctrl::toXml(obj, true);
   std::cout << xml << std::endl;
   newObj = ctrl::fromXml<T>(xml);
   if (!testAndDelete(newObj, obj, 0)) {
      return false;
   }

   bool desiredExceptionCaught = false;
   try {
      std::string json = ctrl::toJson(obj, 4);
      std::cout << json << std::endl;
   } catch (ctrl::Exception ex) {
      if (!multimapJsonException || std::string(ex.what()) != "Multimaps with duplicated keys aren't supported when serializing to JSON") {
         throw ex;
      } else {
         desiredExceptionCaught = true;
      }
   }
   if (multimapJsonException && !desiredExceptionCaught) {
      std::cout << "Multimaps silently ignored" << std::endl;
      return false;
   }

   return true;
}

//******************************************************************************

class SimpleClass {
public:
   SimpleClass(const int& c, const std::string& n)
      : count(c)
      , name(n) {

   }

   bool operator==(const SimpleClass& that) const {
      return count == that.count && name == that.name;
   }

   bool operator!=(const SimpleClass& that) const {
      return count != that.count || name != that.name;
   }

   bool operator<(const SimpleClass& that) const {
      return count < that.count;
   }

   static size_t hash(const SimpleClass& obj) {
      return obj.count;
   }

   std::string toString() {
      return name;
   }

   CTRL_BEGIN_MEMBERS(SimpleClass)
   CTRL_MEMBER(private, int, count)
   CTRL_MEMBER(private, std::string, name)
   CTRL_END_MEMBERS()
};

bool testSimple() {
   std::cout << "testSimple" << std::endl;
   std::cout << "----------" << std::endl;

   SimpleClass obj(3, "Gerrit");

   return testSerialization(obj);
}

//******************************************************************************

class CompositeClass {
public:
   CompositeClass(const SimpleClass& obj, const int& v)
        : object(obj)
        , value(v) {

   }

   bool operator==(const CompositeClass& that) const {
      return object == that.object && value == that.value;
   }

   CTRL_BEGIN_MEMBERS(CompositeClass)
   CTRL_MEMBER(private, SimpleClass, object)
   CTRL_MEMBER(private, int, value)
   CTRL_END_MEMBERS()
};

bool testComposite() {
   std::cout << "testComposite" << std::endl;
   std::cout << "-------------" << std::endl;

   CompositeClass obj(SimpleClass(3, "Alex"), 42);

   return testSerialization(obj);
}

//******************************************************************************

class DerivedClass : public CompositeClass {
public:
   DerivedClass(const SimpleClass& obj, const int& v, const std::string& t)
      : CompositeClass(obj, v)
      , text(t) {

   }

   bool operator==(const DerivedClass& that) const {
      return CompositeClass::operator==(that) && text == that.text;
   }

   CTRL_BEGIN_MEMBERS(DerivedClass)
   CTRL_BASE_CLASS(CompositeClass)
   CTRL_MEMBER(private, std::string, text)
   CTRL_END_MEMBERS()
};

bool testDerived() {
   std::cout << "testDerived" << std::endl;
   std::cout << "-----------" << std::endl;

   DerivedClass obj(SimpleClass(3, "Gerrit Daniels"), 42, "Mostly harmless");

   return testSerialization(obj);
}

//******************************************************************************

class WStringContainer {
public:
   WStringContainer(std::wstring str) : m_str(str) { }

   bool operator==(const WStringContainer& that) const {
      return m_str == that.m_str;
   }

   CTRL_BEGIN_MEMBERS(WStringContainer)
   CTRL_MEMBER(public, std::wstring, m_str)
   CTRL_END_MEMBERS()
};

bool testWString() {
   std::cout << "testWString" << std::endl;
   std::cout << "-----------" << std::endl;
   WStringContainer obj(L"\u0391 - \u03a9");
   std::wcout << obj.m_str << std::endl;

   return testSerialization(obj);
}

//******************************************************************************

class SimplePair {
public:
   SimplePair(char first, SimpleClass second) : m_pair(first, second) { }

   bool operator==(const SimplePair& that) const {
      return m_pair == that.m_pair;
   }

   typedef std::pair<char, SimpleClass> PairType;
   CTRL_BEGIN_MEMBERS(SimplePair)
   CTRL_MEMBER(private, PairType, m_pair)
   CTRL_END_MEMBERS()
};

bool testSimplePair() {
   std::cout << "testSimplePair" << std::endl;
   std::cout << "--------------" << std::endl;
   SimplePair obj('G', SimpleClass(42, "Gerrit"));

   return testSerialization(obj);
}

//******************************************************************************

class Complex {
public:
   Complex(double real, double imag) : m_num(real, imag) { }

   bool operator==(const Complex& that) const {
      return m_num == that.m_num;
   }

   CTRL_BEGIN_MEMBERS(Complex)
   CTRL_MEMBER(private, std::complex<double>, m_num)
   CTRL_END_MEMBERS()
};

bool testComplex() {
   std::cout << "testComplex" << std::endl;
   std::cout << "-----------" << std::endl;
   Complex obj(0.85, 2.33);

   return testSerialization(obj);
}

//******************************************************************************

class Valarray {
public:
   Valarray(double v0, double v1, double v2) : m_values(3) {
      m_values[0] = v0;
      m_values[1] = v1;
      m_values[2] = v2;
   }

   bool operator==(const Valarray& that) const {
      if (m_values.size() != that.m_values.size())
         return false;

      for (size_t i = 0; i < m_values.size(); ++i)
         if (m_values[i] != that.m_values[i])
            return false;

         return true;
   }

   CTRL_BEGIN_MEMBERS(Valarray)
   CTRL_MEMBER(private, std::valarray<double>, m_values)
   CTRL_END_MEMBERS()
};

bool testValarray() {
   std::cout << "testValarray" << std::endl;
   std::cout << "------------" << std::endl;
   Valarray obj(0.85, 2.33, 3.78);

   return testSerialization(obj);
}

//******************************************************************************

class BitsetContainer {
public:
   void set(size_t pos) {
      m_bits.set(pos);
   }

   bool operator==(const BitsetContainer& that) {
      return m_bits == that.m_bits;
   }

   CTRL_BEGIN_MEMBERS(BitsetContainer)
   CTRL_MEMBER(private, std::bitset<10>, m_bits)
   CTRL_END_MEMBERS()
};

bool testBitset() {
   std::cout << "testBitset" << std::endl;
   std::cout << "----------" << std::endl;
   BitsetContainer obj;
   obj.set(0);
   obj.set(1);
   obj.set(2);
   obj.set(4);
   obj.set(8);

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassDeque {
public:
   void add(const SimpleClass& obj) {
      elements.push_back(obj);
   }

   bool operator==(const SimpleClassDeque& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassDeque)
   CTRL_MEMBER(private, std::deque<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testDeque() {
   std::cout << "testDeque" << std::endl;
   std::cout << "---------" << std::endl;

   SimpleClassDeque obj;
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassList {
public:
   void add(const SimpleClass& obj) {
      elements.push_back(obj);
   }

   bool operator==(const SimpleClassList& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassList)
   CTRL_MEMBER(private, std::list<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testList() {
   std::cout << "testList" << std::endl;
   std::cout << "--------" << std::endl;

   SimpleClassList obj;
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleMap {
public:
   void add(const std::string& key, const SimpleClass& obj) {
      m_map[key] = obj;
   }

   bool operator==(const SimpleMap& that) const {
      return m_map == that.m_map;
   }

   typedef std::map<std::string, SimpleClass> MapType;
   CTRL_BEGIN_MEMBERS(SimpleMap)
   CTRL_MEMBER(private, MapType, m_map)
   CTRL_END_MEMBERS()
};

bool testMap() {
   std::cout << "testMap" << std::endl;
   std::cout << "-------" << std::endl;
   SimpleMap obj;
   obj.add("G", SimpleClass(0, "Gerrit"));
   obj.add("A", SimpleClass(1, "Alex"));
   obj.add("K", SimpleClass(2, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleMultimap {
public:
   void add(const std::string& key, const SimpleClass& obj) {
      m_map.insert(std::pair<std::string, SimpleClass>(key, obj));
   }

   bool operator==(const SimpleMultimap& that) const {
      return m_map == that.m_map;
   }

   typedef std::multimap<std::string, SimpleClass> MapType;
   CTRL_BEGIN_MEMBERS(SimpleMultimap)
   CTRL_MEMBER(private, MapType, m_map)
   CTRL_END_MEMBERS()
};

bool testMultimap() {
   std::cout << "testMultimap" << std::endl;
   std::cout << "------------" << std::endl;
   SimpleMultimap obj;
   obj.add("G", SimpleClass(0, "Gerrit"));
   obj.add("A", SimpleClass(1, "Alex"));
   obj.add("K", SimpleClass(2, "Kermit"));
   obj.add("G", SimpleClass(8, "Geert"));

   return testSerialization(obj, true);
}

//******************************************************************************

class SimpleClassQueue {
public:
   void add(const SimpleClass& obj) {
      elements.push(obj);
   }

   bool operator==(const SimpleClassQueue& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassQueue)
   CTRL_MEMBER(private, std::queue<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testQueue() {
   std::cout << "testQueue" << std::endl;
   std::cout << "---------" << std::endl;

   SimpleClassQueue obj;
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassPriorityQueue {
public:
   void add(const SimpleClass& obj) {
      elements.push(obj);
   }

   bool operator==(const SimpleClassPriorityQueue& that) {
      if (elements.size() != that.elements.size())
         return false;

      std::priority_queue<SimpleClass> thisCopy(elements);
      std::priority_queue<SimpleClass> thatCopy(that.elements);

      while (!thisCopy.empty()) {
         if (thisCopy.top() != thatCopy.top())
            return false;
         thisCopy.pop();
         thatCopy.pop();
      }
      return true;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassPriorityQueue)
   CTRL_MEMBER(private, std::priority_queue<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testPriorityQueue() {
   std::cout << "testPriorityQueue" << std::endl;
   std::cout << "-----------------" << std::endl;

   SimpleClassPriorityQueue obj;
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassSet {
public:
   void add(const SimpleClass& obj) {
      elements.insert(obj);
   }

   bool operator==(const SimpleClassSet& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassSet)
   CTRL_MEMBER(private, std::set<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testSet() {
   std::cout << "testSet" << std::endl;
   std::cout << "-------" << std::endl;

   SimpleClassSet obj;
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassMultiset {
public:
   void add(const SimpleClass& obj) {
      elements.insert(obj);
   }

   bool operator==(const SimpleClassMultiset& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassMultiset)
   CTRL_MEMBER(private, std::multiset<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testMultiset() {
   std::cout << "testMultiset" << std::endl;
   std::cout << "------------" << std::endl;

   SimpleClassMultiset obj;
   obj.add(SimpleClass(11, "Kermit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassStack {
public:
   void add(const SimpleClass& obj) {
      elements.push(obj);
   }

   bool operator==(const SimpleClassStack& that) {
      return elements == that.elements;
   }

   void print() {
      std::stack<SimpleClass> copy(elements);
      while (!copy.empty()) {
         std::cout << copy.top().toString() << " ";
         copy.pop();
      }
      std::cout << std::endl;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassStack)
   CTRL_MEMBER(private, std::stack<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testStack() {
   std::cout << "testStack" << std::endl;
   std::cout << "---------" << std::endl;

   SimpleClassStack obj;
   obj.add(SimpleClass(11, "Kermit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(5, "Gerrit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassVector {
public:
   void add(const SimpleClass& obj) {
      elements.push_back(obj);
   }

   bool operator==(const SimpleClassVector& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassVector)
   CTRL_MEMBER(private, std::vector<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testVector() {
   std::cout << "testVector" << std::endl;
   std::cout << "----------" << std::endl;

   SimpleClassVector obj;
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class ArrayContainer {
public:
   typedef std::array<std::string, 3> Elements;

   void add(Elements::size_type i, const std::string& str) {
      m_elements[i] = str;
   }

   bool operator==(const ArrayContainer& that) {
      if (m_elements.size() != that.m_elements.size())
         return false;

      for (Elements::size_type i = 0; i < m_elements.size(); ++i)
         if (m_elements[i] != that.m_elements[i])
            return false;

         return true;
   }

   CTRL_BEGIN_MEMBERS(ArrayContainer)
   CTRL_MEMBER(private, Elements, m_elements)
   CTRL_END_MEMBERS()
};

bool testArray() {
   std::cout << "testArray" << std::endl;
   std::cout << "---------" << std::endl;
   ArrayContainer obj;
   obj.add(0, "Gerrit");
   obj.add(1, "Paul");
   obj.add(2, "Toon");

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassForwardList {
public:
   void add(const SimpleClass& obj) {
      elements.push_front(obj);
   }

   bool operator==(const SimpleClassForwardList& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassForwardList)
   CTRL_MEMBER(private, std::forward_list<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testForwardList() {
   std::cout << "testForwardList" << std::endl;
   std::cout << "---------------" << std::endl;

   SimpleClassForwardList obj;
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleUnorderedMap {
public:
   void add(const std::string& key, const SimpleClass& obj) {
      m_map[key] = obj;
   }

   bool operator==(const SimpleUnorderedMap& that) const {
      return m_map == that.m_map;
   }

   typedef std::unordered_map<std::string, SimpleClass> UnorderedMapType;
   CTRL_BEGIN_MEMBERS(SimpleUnorderedMap)
   CTRL_MEMBER(private, UnorderedMapType, m_map)
   CTRL_END_MEMBERS()
};

bool testUnorderedMap() {
   std::cout << "testUnorderedMap" << std::endl;
   std::cout << "----------------" << std::endl;
   SimpleUnorderedMap obj;
   obj.add("G", SimpleClass(0, "Gerrit"));
   obj.add("A", SimpleClass(1, "Alex"));
   obj.add("K", SimpleClass(2, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleUnorderedMultimap {
public:
   void add(const std::string& key, const SimpleClass& obj) {
      m_map.insert(std::pair<std::string, SimpleClass>(key, obj));
   }

   bool operator==(const SimpleUnorderedMultimap& that) const {
      return m_map == that.m_map;
   }

   typedef std::unordered_multimap<std::string, SimpleClass> MapType;
   CTRL_BEGIN_MEMBERS(SimpleUnorderedMultimap)
   CTRL_MEMBER(private, MapType, m_map)
   CTRL_END_MEMBERS()
};

bool testUnorderedMultimap() {
   std::cout << "testUnorderedMultimap" << std::endl;
   std::cout << "---------------------" << std::endl;
   SimpleUnorderedMultimap obj;
   obj.add("G", SimpleClass(0, "Gerrit"));
   obj.add("A", SimpleClass(1, "Alex"));
   obj.add("K", SimpleClass(2, "Kermit"));
   obj.add("G", SimpleClass(8, "Geert"));

   return testSerialization(obj, true);
}

//******************************************************************************

namespace std {

   template<>
   struct hash<SimpleClass> {
   public:
      size_t operator()(const SimpleClass& obj) const {
         return SimpleClass::hash(obj);
      }
   };

}

class SimpleClassUnorderedSet {
public:
   void add(const SimpleClass& obj) {
      elements.insert(obj);
   }

   bool operator==(const SimpleClassUnorderedSet& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassUnorderedSet)
   CTRL_MEMBER(private, std::unordered_set<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testUnorderedSet() {
   std::cout << "testUnorderedSet" << std::endl;
   std::cout << "----------------" << std::endl;

   SimpleClassUnorderedSet obj;
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassUnorderedMultiset {
public:
   void add(const SimpleClass& obj) {
      elements.insert(obj);
   }

   bool operator==(const SimpleClassUnorderedMultiset& that) {
      return elements == that.elements;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassUnorderedMultiset)
   CTRL_MEMBER(private, std::unordered_multiset<SimpleClass>, elements)
   CTRL_END_MEMBERS()
};

bool testUnorderedMultiset() {
   std::cout << "testUnorderedMultiset" << std::endl;
   std::cout << "---------------------" << std::endl;

   SimpleClassUnorderedMultiset obj;
   obj.add(SimpleClass(11, "Kermit"));
   obj.add(SimpleClass(9, "Alex"));
   obj.add(SimpleClass(5, "Gerrit"));
   obj.add(SimpleClass(11, "Kermit"));

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassPtrList {
public:
   void add(const boost::shared_ptr<SimpleClass>& obj) {
      elements.push_back(obj);
   }

   boost::shared_ptr<SimpleClass> get(int index) { return elements[index]; }

   bool operator==(const SimpleClassPtrList& that) {
      if (elements.size() != that.elements.size())
         return false;
      for (int i = 0; i < elements.size(); ++i) {
         if (elements[i].get() == 0 || that.elements[i].get() == 0) {
            if (elements[i].get() != that.elements[i].get())
               return false;
         }
         else if ( !(*(elements[i]) == *(that.elements[i])) )
            return false;
      }
      return true;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassPtrList)
   CTRL_MEMBER(private, std::vector< boost::shared_ptr<SimpleClass> >, elements)
   CTRL_END_MEMBERS()
};

bool testSharedPtr() {
   std::cout << "testSharedPtr" << std::endl;
   std::cout << "-------------" << std::endl;
   SimpleClassPtrList obj;
   boost::shared_ptr<SimpleClass> ptr(new SimpleClass(0, "Gerrit"));
   obj.add(ptr);
   obj.add(boost::shared_ptr<SimpleClass>(new SimpleClass(1, "Alex")));
   obj.add(ptr);
   obj.add(ptr);
   obj.add(boost::shared_ptr<SimpleClass>(new SimpleClass(2, "Kermit")));

   return testSerialization(obj);
}

//******************************************************************************

class WeakContainer {
public:
   void setPointers(boost::shared_ptr<SimpleClass> ptr) {
      m_delayedSerialization = ptr;
      m_sharedPtr = ptr;
      m_weakPtr = ptr;
   }

   bool operator==(const WeakContainer& that) const {
       return (*m_sharedPtr == *that.m_sharedPtr) && (*m_weakPtr.lock() == *that.m_weakPtr.lock()) &&
              (*m_delayedSerialization.lock() == *that.m_delayedSerialization.lock());
   }

   CTRL_BEGIN_MEMBERS(WeakContainer)
   CTRL_MEMBER(private, boost::weak_ptr<SimpleClass>, m_delayedSerialization)
   CTRL_MEMBER(private, boost::shared_ptr<SimpleClass>, m_sharedPtr)
   CTRL_MEMBER(private, boost::weak_ptr<SimpleClass>, m_weakPtr)
   CTRL_END_MEMBERS()
};

bool testWeakPtr() {
   std::cout << "testWeakPtr" << std::endl;
   std::cout << "-----------" << std::endl;
   WeakContainer obj;
   boost::shared_ptr<SimpleClass> ptr(new SimpleClass(42, "Gerrit"));
   obj.setPointers(ptr);

   return testSerialization(obj);
}

//******************************************************************************

class SimpleClassStdPtrList {
public:
   void add(const std::shared_ptr<SimpleClass>& obj) {
      elements.push_back(obj);
   }

   std::shared_ptr<SimpleClass> get(int index) { return elements[index]; }

   bool operator==(const SimpleClassStdPtrList& that) {
      if (elements.size() != that.elements.size())
         return false;
      for (int i = 0; i < elements.size(); ++i) {
         if (elements[i].get() == 0 || that.elements[i].get() == 0) {
            if (elements[i].get() != that.elements[i].get())
               return false;
         }
         else if ( !(*(elements[i]) == *(that.elements[i])) )
            return false;
      }
      return true;
   }

   CTRL_BEGIN_MEMBERS(SimpleClassStdPtrList)
   CTRL_MEMBER(private, std::vector< std::shared_ptr<SimpleClass> >, elements)
   CTRL_END_MEMBERS()
};

bool testStdSharedPtr() {
   std::cout << "testStdSharedPtr" << std::endl;
   std::cout << "----------------" << std::endl;
   SimpleClassStdPtrList obj;
   std::shared_ptr<SimpleClass> ptr(new SimpleClass(0, "Gerrit"));
   obj.add(ptr);
   obj.add(std::shared_ptr<SimpleClass>(new SimpleClass(1, "Alex")));
   obj.add(ptr);
   obj.add(ptr);
   obj.add(std::shared_ptr<SimpleClass>(new SimpleClass(2, "Kermit")));

   return testSerialization(obj);
}

//******************************************************************************

class StdWeakContainer {
public:
   void setPointers(std::shared_ptr<SimpleClass> ptr) {
      m_delayedSerialization = ptr;
      m_sharedPtr = ptr;
      m_weakPtr = ptr;
   }

   bool operator==(const StdWeakContainer& that) const {
       return (*m_sharedPtr == *that.m_sharedPtr) && (*m_weakPtr.lock() == *that.m_weakPtr.lock()) &&
              (*m_delayedSerialization.lock() == *that.m_delayedSerialization.lock());
   }

   CTRL_BEGIN_MEMBERS(StdWeakContainer)
   CTRL_MEMBER(private, std::weak_ptr<SimpleClass>, m_delayedSerialization)
   CTRL_MEMBER(private, std::shared_ptr<SimpleClass>, m_sharedPtr)
   CTRL_MEMBER(private, std::weak_ptr<SimpleClass>, m_weakPtr)
   CTRL_END_MEMBERS()
};

bool testStdWeakPtr() {
   std::cout << "testStdWeakPtr" << std::endl;
   std::cout << "--------------" << std::endl;
   StdWeakContainer obj;
   std::shared_ptr<SimpleClass> ptr(new SimpleClass(42, "Gerrit"));
   obj.setPointers(ptr);

   return testSerialization(obj);
}


//******************************************************************************

class SomeClass {
public:
   SomeClass(const bool& fl, const std::string& n, const float& f)
      : m_flag(fl)
      , m_text(n)
      , m_float(f) {

   }

   bool operator==(const SomeClass& that) const {
      return m_flag == that.m_flag && m_text == that.m_text && m_float == that.m_float;
   }

   CTRL_BEGIN_MEMBERS(SomeClass)
   CTRL_MEMBER(private, bool, m_flag)
   CTRL_MEMBER(private, std::string, m_text)
   CTRL_MEMBER(private, float, m_float)
   CTRL_END_MEMBERS()
};

class SomeContainer {
public:
   SomeContainer(SomeClass* ptr) : m_autoPtr(ptr) { }

   bool operator==(const SomeContainer& that) const {
      if (m_autoPtr.get() == 0 && that.m_autoPtr.get() == 0)
         return true;
      else if (m_autoPtr.get() != 0 && that.m_autoPtr.get() != 0) {
         if (*m_autoPtr == *that.m_autoPtr)
            return true;
         else
            return false;
      }
      else
         return false;
   }

   CTRL_BEGIN_MEMBERS(SomeContainer)
   CTRL_MEMBER(private, std::auto_ptr<SomeClass>, m_autoPtr)
   CTRL_END_MEMBERS()
};

bool testAutoPointer() {
   std::cout << "testAutoPointer" << std::endl;
   std::cout << "-----------------" << std::endl;
   SomeContainer obj(new SomeClass(true, "Gerrit", 2.25));

   if (!testSerialization(obj)) {
      return false;
   }

   SomeContainer obj2(0);
   return testSerialization(obj2);
}

//******************************************************************************

class UniqueContainer {
public:
   UniqueContainer(SomeClass* ptr) : m_uniquePtr(ptr) { }

   bool operator==(const UniqueContainer& that) const {
      if (m_uniquePtr.get() == 0 && that.m_uniquePtr.get() == 0)
         return true;
      else if (m_uniquePtr.get() != 0 && that.m_uniquePtr.get() != 0) {
         if (*m_uniquePtr == *that.m_uniquePtr)
            return true;
         else
            return false;
      }
      else
         return false;
   }

   CTRL_BEGIN_MEMBERS(UniqueContainer)
   CTRL_MEMBER(private, std::unique_ptr<SomeClass>, m_uniquePtr)
   CTRL_END_MEMBERS()
};

bool testUniquePointer() {
   std::cout << "testUniquePointer" << std::endl;
   std::cout << "-----------------" << std::endl;
   UniqueContainer obj(new SomeClass(true, "Gerrit", 2.25));

   return testSerialization(obj);
}

//******************************************************************************

class UniqueVector {
public:

   void add(SomeClass* ptr) {
      m_ptrs.push_back(std::unique_ptr<SomeClass>(ptr));
   }

   bool operator==(const UniqueVector& that) const {
      if (m_ptrs.size() != that.m_ptrs.size())
         return false;

      for (typename std::vector< std::unique_ptr<SomeClass> >::size_type i = 0; i < m_ptrs.size(); ++i)
         if (! (*(m_ptrs[i]) == *(that.m_ptrs[i])) )
            return false;

      return true;
   }

   CTRL_BEGIN_MEMBERS(UniqueVector)
   CTRL_MEMBER(private, std::vector< std::unique_ptr<SomeClass> >, m_ptrs)
   CTRL_END_MEMBERS()
};

bool testUniquePointerVector() {
   std::cout << "testUniquePointerVector" << std::endl;
   std::cout << "-----------------------" << std::endl;
   UniqueVector obj;
   obj.add(new SomeClass(true, "Gerrit", 2.25));
   obj.add(new SomeClass(false, "Alex", 3.14));
   obj.add(new SomeClass(true, "Kermit", 42.0));

   return testSerialization(obj);
}

//******************************************************************************

class PointedTo {
public:
   PointedTo(const std::string& name) : m_name(name) { }

   bool operator==(const PointedTo& that) const {
      return m_name == that.m_name;
   }

   CTRL_BEGIN_MEMBERS(PointedTo)
   CTRL_MEMBER(private, std::string, m_name)
   CTRL_END_MEMBERS()
};

class RawContainer {
public:
   RawContainer(const std::string name)
   : m_ptr0(new PointedTo(name))
   , m_ptr1(m_ptr0) { }

   ~RawContainer() {
      delete m_ptr0;
   }

   bool operator==(const RawContainer& that) const {
      if (m_ptr0 == 0 && that.m_ptr0 == 0)
         return true;
      if (m_ptr0 == 0)
         return false;
      if (that.m_ptr0 == 0)
         return false;
      return *m_ptr0 == *(that.m_ptr0) && *m_ptr1 == *(that.m_ptr1);
   }

   CTRL_BEGIN_MEMBERS(RawContainer)
   CTRL_MEMBER(private, PointedTo*, m_ptr0)
   CTRL_MEMBER(private, PointedTo*, m_ptr1)
   CTRL_END_MEMBERS()
};

bool testRawPointer() {
   std::cout << "testRawPointer" << std::endl;
   std::cout << "--------------" << std::endl;
   RawContainer obj("Gerrit");

   return testSerialization(obj);
}

//******************************************************************************

class Color {
public:
   Color(int r, int g, int b)
      : m_red(r)
      , m_green(g)
      , m_blue(b) {

   }

   bool operator==(const Color& that) const {
      return m_red == that.m_red && m_green == that.m_green && m_blue == that.m_blue;
   }

   CTRL_BEGIN_MEMBERS(Color)
   CTRL_MEMBER(private, int, m_red)
         CTRL_WITH_NAME("r")
         CTRL_AS_ATTRIBUTE()
   CTRL_MEMBER(private, int, m_green)
         CTRL_WITH_NAME("g")
         CTRL_AS_ATTRIBUTE()
   CTRL_MEMBER(private, int, m_blue)
         CTRL_WITH_NAME("b")
         CTRL_AS_ATTRIBUTE()
   CTRL_END_MEMBERS()
};

class Shape {
protected:
   Shape(Color c)
      : m_fill(c) {

   }

   virtual bool equals(const Shape& that) const {
      return m_fill == that.m_fill;
   }

private:
   friend bool operator==(const Shape&, const Shape&);

public:
   virtual ~Shape() { }

   CTRL_BEGIN_MEMBERS(Shape)
         CTRL_TYPE_ID_FIELD_NAME("shapetype")
         CTRL_TYPE_ID_FIELD_AS_ATTRIBUTE()
         CTRL_ID_FIELD_NAME("id")
         CTRL_ID_FIELD_AS_ATTRIBUTE()
   CTRL_MEMBER(protected, Color, m_fill)
         CTRL_WITH_NAME("fill")
   CTRL_END_MEMBERS()
};

CTRL_ABSTRACT_POLYMORPH(Shape)

class Circle : public Shape {
public:
   Circle(Color c, float x, float y, float r)
      : Shape(c)
      , m_x(x)
      , m_y(y)
      , m_radius(r) {

   }

   virtual ~Circle() { }

protected:
   virtual bool equals(const Shape& that) const {
      const Circle* pThat = dynamic_cast<const Circle*>(&that);
      return Shape::equals(that) && m_x == pThat->m_x && m_y == pThat->m_y &&
             m_radius == pThat->m_radius;
   }

   CTRL_BEGIN_MEMBERS(Circle)
   CTRL_BASE_CLASS(Shape)
   CTRL_MEMBER(private, float, m_x)
         CTRL_WITH_NAME("x")
   CTRL_MEMBER(private, float, m_y)
         CTRL_WITH_NAME("y")
   CTRL_MEMBER(private, float, m_radius)
         CTRL_WITH_NAME("r")
   CTRL_END_MEMBERS()
};

CTRL_POLYMORPH(Circle)

class Rectangle : public Shape {
public:
   Rectangle(Color c, float x, float y, float w, float h)
      : Shape(c)
      , m_x(x)
      , m_y(y)
      , m_width(w)
      , m_height(h) {

   }

   virtual ~Rectangle() { }

protected:
   bool equals(const Shape& that) const {
      const Rectangle* pThat = dynamic_cast<const Rectangle*>(&that);
      return Shape::equals(that) && m_x == pThat->m_x && m_y == pThat->m_y &&
             m_width == pThat->m_width && m_height == pThat->m_height;
   }

   CTRL_BEGIN_MEMBERS(Rectangle)
   CTRL_BASE_CLASS(Shape)
   CTRL_MEMBER(private, float, m_x)
         CTRL_WITH_NAME("x")
   CTRL_MEMBER(private, float, m_y)
         CTRL_WITH_NAME("y")
   CTRL_MEMBER(private, float, m_width)
         CTRL_WITH_NAME("w")
   CTRL_MEMBER(private, float, m_height)
         CTRL_WITH_NAME("h")
   CTRL_END_MEMBERS()
};

CTRL_POLYMORPH(Rectangle)

bool operator==(const Shape& shape1, const Shape& shape2) {
   return typeid(shape1) == typeid(shape2) && shape1.equals(shape2);
}

class Drawing {
public:
   Drawing(Color bg)
      : m_background(bg) {

   }

   void addShape(boost::shared_ptr<Shape> shape) {
      m_shapes.push_back(shape);
   }

   virtual bool operator==(const Drawing& that) {
      if (!(m_background == that.m_background))
         return false;
      if (m_shapes.size() != that.m_shapes.size())
         return false;
      for (int i = 0; i < m_shapes.size(); ++i) {
         if (m_shapes[i].get() == 0 || that.m_shapes[i].get() == 0) {
            if (m_shapes[i].get() != that.m_shapes[i].get())
               return false;
         }
         else if ( !(*(m_shapes[i]) == *(that.m_shapes[i])) || sizeof(*(m_shapes[i])) != sizeof(*(that.m_shapes[i])) )
            return false;
      }
      return true;
   }

   CTRL_BEGIN_MEMBERS(Drawing)
   CTRL_MEMBER(private, Color, m_background)
         CTRL_WITH_NAME("background")
   CTRL_MEMBER(private, std::vector< boost::shared_ptr<Shape> >, m_shapes)
         CTRL_WITH_NAME("shapes")
   CTRL_END_MEMBERS()
};

bool testPolymorph() {
   std::cout << "testPolymorph" << std::endl;
   std::cout << "-------------" << std::endl;

   Drawing drawing(Color(0, 255, 0));
   boost::shared_ptr<Shape> circle(new Circle(Color(255, 255, 0), 1.0, 2.0, 3.0));
   boost::shared_ptr<Shape> rectangle(new Rectangle(Color(127, 127, 127), 2.0, 4.0, 3.0, 3.0));
   drawing.addShape(circle);
   drawing.addShape(rectangle);
   drawing.addShape(circle);

   return testSerialization(drawing);
}

//******************************************************************************

class PureVirtual {
public:
   PureVirtual(int val) : m_val(val) {}
   virtual ~PureVirtual() {}

   bool operator==(const PureVirtual& that) const {
      return m_val == that.m_val;
   }

   virtual void foo() = 0;

   CTRL_BEGIN_MEMBERS(PureVirtual)
   CTRL_MEMBER(protected, int, m_val)
   CTRL_END_MEMBERS()
};

CTRL_ABSTRACT_POLYMORPH(PureVirtual)

class FooImplementer : public PureVirtual {
public:
   FooImplementer(int val, std::string text) : PureVirtual(val), m_text(text) {}
   virtual ~FooImplementer() {}

   bool operator==(const FooImplementer& that) const {
      return PureVirtual::operator==(that) && m_text == that.m_text;
   }

   virtual void foo() { }

   CTRL_BEGIN_MEMBERS(FooImplementer)
   CTRL_BASE_CLASS(PureVirtual)
   CTRL_MEMBER(private, std::string, m_text)
   CTRL_END_MEMBERS()
};

CTRL_POLYMORPH(FooImplementer)

bool testPureVirtualFunction() {
   std::cout << "testPureVirtualFunction" << std::endl;
   std::cout << "-----------------------" << std::endl;
   FooImplementer obj(5, "Gerrit");

   return testSerialization(obj);
}

//******************************************************************************

class Base0 {
public:
   Base0(const int& v) : m_val0(v) {}
   virtual ~Base0() {}

   bool operator==(const Base0& that) const {
      return m_val0 == that.m_val0;
   }

   CTRL_BEGIN_MEMBERS(Base0)
         CTRL_WITH_NAME("B0")
   CTRL_MEMBER(private, int, m_val0)
   CTRL_END_MEMBERS()
};

CTRL_ABSTRACT_POLYMORPH(Base0)

class Base1 {
public:
   Base1(const int& v) : m_val1(v) {}
   virtual ~Base1() {}

   bool operator==(const Base1& that) const {
      return m_val1 == that.m_val1;
   }

   CTRL_BEGIN_MEMBERS(Base1)
         CTRL_WITH_NAME("B1")
   CTRL_MEMBER(private, int, m_val1)
   CTRL_END_MEMBERS()
};

CTRL_ABSTRACT_POLYMORPH(Base1)

class Derived0 : public Base0, public Base1 {
public:
   Derived0(const int& v0, const int& v1, const std::string& str)
      : Base0(v0), Base1(v1), m_str(str) {}

   virtual ~Derived0() {}

   bool operator==(const Derived0& that) {
      return Base0::operator==(that) && Base1::operator==(that) && m_str == that.m_str;
   }

   CTRL_BEGIN_MEMBERS(Derived0)
         CTRL_WITH_NAME("D0")
   CTRL_BASE_CLASS(Base0)
   CTRL_BASE_CLASS(Base1)
   CTRL_MEMBER(private, std::string, m_str)
   CTRL_END_MEMBERS()
};

bool testMultipleInheritance0() {
   std::cout << "testMultipleInheritance0" << std::endl;
   std::cout << "------------------------" << std::endl;
   Derived0 obj(1, 2, "abc");

   return testSerialization(obj);
}

//******************************************************************************

class Derived1 : public Base0, public Base1 {
public:
   Derived1(const int& v0, const int& v1, const std::string& str)
      : Base0(v0), Base1(v1), m_str(str) {}

   virtual ~Derived1() {}

   bool operator==(const Derived1& that) {
      return Base0::operator==(that) && Base1::operator==(that) && m_str == that.m_str;
   }

   CTRL_BEGIN_MEMBERS(Derived1)
         CTRL_WITH_NAME("D1")
   CTRL_BASE_CLASS(Base0)
   CTRL_BASE_CLASS(Base1)
   CTRL_MEMBER(private, std::string, m_str)
   CTRL_END_MEMBERS()
};

CTRL_POLYMORPH_MULTIPLE_2(Derived1, Base0, Base1)

class Container {
public:
   void setDerived(boost::shared_ptr<Derived1> ptr) {
      m_ptr = ptr;
   }

   void setBase0(boost::shared_ptr<Base0> ptr) {
      m_ptr0 = ptr;
   }

   void setBase1(boost::shared_ptr<Base1> ptr) {
      m_ptr1 = ptr;
   }

   bool operator==(const Container& that) {
      return (*m_ptr == *that.m_ptr) && (*m_ptr0 == *that.m_ptr0) && (*m_ptr1 == *that.m_ptr1);
   }

   CTRL_BEGIN_MEMBERS(Container)
   CTRL_MEMBER(private, boost::shared_ptr<Base1>, m_ptr1)
   CTRL_MEMBER(private, boost::shared_ptr<Derived1>, m_ptr)
   CTRL_MEMBER(private, boost::shared_ptr<Base0>, m_ptr0)
   CTRL_END_MEMBERS()
};

bool testMultipleInheritance1() {
   std::cout << "testMultipleInheritance1" << std::endl;
   std::cout << "------------------------" << std::endl;
   Container obj;
   obj.setDerived(boost::shared_ptr<Derived1>(new Derived1(1, 2, "a")));
   obj.setBase0(boost::shared_ptr<Base0>(new Derived1(3, 4, "b")));
   obj.setBase1(boost::shared_ptr<Base1>(new Derived1(5, 6, "c")));

   return testSerialization(obj);
}

//******************************************************************************

bool testMultipleInheritance2() {
   std::cout << "testMultipleInheritance2" << std::endl;
   std::cout << "------------------------" << std::endl;
   Container obj;
   boost::shared_ptr<Derived1> ptr(new Derived1(1, 2, "a"));
   obj.setDerived(ptr);
   obj.setBase0(ptr);
   obj.setBase1(ptr);

   return testSerialization(obj);
}

//******************************************************************************

class InitializeBase {
public:
   InitializeBase(int val0, int val1)
      : m_val0(val0)
      , m_val1(val1) {
      m_sum = val0 + val1;
   }

   static void initialize(InitializeBase& obj, int version) {
      obj.m_sum = obj.m_val0 + obj.m_val1;
   }

   bool operator==(const InitializeBase& that) const {
      return (m_val0 == that.m_val0) && (m_val1 == that.m_val1) && (m_sum == that.m_sum);
   }

   CTRL_BEGIN_MEMBERS(InitializeBase)
   CTRL_MEMBER(protected, int, m_val0)
   CTRL_MEMBER(protected, int, m_val1)
   CTRL_END_MEMBERS()

private:
   int m_sum;
};

class InitializeClass : public InitializeBase {
public:
   InitializeClass(int val0, int val1, int val2)
      : InitializeBase(val0, val1)
      , m_val2(val2) {
      m_product = val0 * val1 * val2;
   }

   static void initialize(InitializeClass& obj, int version) {
      obj.m_product = obj.m_val0 * obj.m_val1 * obj.m_val2;
   }

   bool operator==(const InitializeClass& that) const {
      return InitializeBase::operator==(that) && (m_val2 == that.m_val2) && (m_product == that.m_product);
   }

   CTRL_BEGIN_MEMBERS(InitializeClass)
   CTRL_BASE_CLASS(InitializeBase)
   CTRL_MEMBER(private, int, m_val2)
   CTRL_END_MEMBERS()

private:
   int m_product;
};

bool testInitialize() {
   std::cout << "testInitialize" << std::endl;
   std::cout << "--------------" << std::endl;
   InitializeClass obj(5, 7, 3);

   return testSerialization(obj);
}

//******************************************************************************

class EndianClass {
public:
   EndianClass(char c, short s, int i, long long l, float f, double d)
      : m_c(c), m_s(s), m_i(i), m_l(l), m_f(f), m_d(d) { }

   bool operator==(const EndianClass& that) const {
      return m_c == that.m_c && m_s == that.m_s && m_i == that.m_i && m_l == that.m_l && m_f == that.m_f && m_d == that.m_d;
   }

   CTRL_BEGIN_MEMBERS(EndianClass)
   CTRL_MEMBER(private, char, m_c)
   CTRL_MEMBER(private, short, m_s)
   CTRL_MEMBER(private, int, m_i)
   CTRL_MEMBER(private, long long, m_l)
   CTRL_MEMBER(private, float, m_f)
   CTRL_MEMBER(private, double, m_d)
   CTRL_END_MEMBERS()
};

bool testLittleEndian() {
   std::cout << "testLittleEndian" << std::endl;
   std::cout << "----------------" << std::endl;
   EndianClass obj(0x01, 0x0203, 0x04050607, 0x08090a0b0c0d0e0fLL, 2.0f, 3.0);

   long length;
   char* bytes = ctrl::toBinary<8, CTRL_LITTLE_ENDIAN>(obj, length);
   writeBytes(bytes, length);

   char expected[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                     , 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                     , 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                     , 0x07, 0x06, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00
                     , 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08
                     , 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
                     , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40 };
   for (int i = 0; i < length; ++i) {
      if (*(bytes + i) != expected[i]) {
         return false;
      }
   }

   EndianClass* newObj = ctrl::fromBinary<EndianClass, 8, CTRL_LITTLE_ENDIAN>(bytes, length);

   return testAndDelete(newObj, obj, bytes);
}

//******************************************************************************

bool testBigEndian() {
   std::cout << "testBigEndian" << std::endl;
   std::cout << "-------------" << std::endl;
   EndianClass obj(0x01, 0x0203, 0x04050607, 0x08090a0b0c0d0e0fLL, 2.0f, 3.0);

   long length;
   char* bytes = ctrl::toBinary<8, CTRL_BIG_ENDIAN>(obj, length);
   writeBytes(bytes, length);

   char expected[] = { 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
                     , 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                     , 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                     , 0x04, 0x05, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00
                     , 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
                     , 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                     , 0x40, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
   for (int i = 0; i < length; ++i) {
      if (*(bytes + i) != expected[i]) {
         return false;
      }
   }

   EndianClass* newObj = ctrl::fromBinary<EndianClass, 8, CTRL_BIG_ENDIAN>(bytes, length);

   return testAndDelete(newObj, obj, bytes);
}

//******************************************************************************

class WithVersionedClass {
public:
   WithVersionedClass(int v1, int v2) : m_v1(v1), m_v2(v2) { }

   int getV1() const { return m_v1; }
   int getV2() const { return m_v2; }

   static void initialize(WithVersionedClass& obj, int version) {
      if (version < 2) obj.m_v2 = 0;
   }

   CTRL_BEGIN_MEMBERS(WithVersionedClass)
   CTRL_MEMBER(private, int, m_v1)
   CTRL_MEMBER(private, int, m_v2)
         CTRL_WITH_VERSION(2)
   CTRL_END_MEMBERS()
};

bool testWithVersion() {
   std::cout << "testWithVersion" << std::endl;
   std::cout << "-----------" << std::endl;
   WithVersionedClass obj(1, 2);

   long length;
   char* bytes = ctrl::toBinary(obj, length, 1);
   writeBytes(bytes, length);
   WithVersionedClass* newObj = ctrl::fromBinary<WithVersionedClass>(bytes, length, 1);

   if (newObj->getV1() != 1 || newObj->getV2() != 0) {
      delete newObj;
      delete[] bytes;
      return false;
   } else {
      delete newObj;
      delete[] bytes;
   }

   bytes = ctrl::toBinary(obj, length, 2);
   writeBytes(bytes, length);
   newObj = ctrl::fromBinary<WithVersionedClass>(bytes, length, 2);

   if (newObj->getV1() != 1 || newObj->getV2() != 2) {
      delete newObj;
      delete[] bytes;
      return false;
   } else {
      delete newObj;
      delete[] bytes;
      return true;
   }
}

//******************************************************************************

bool testCorruptData() {
   std::cout << "testCorruptData" << std::endl;
   std::cout << "---------------" << std::endl;

   SimpleClass simple(42, "Gerrit");
   CompositeClass composite(SimpleClass(12, "Koen"), 15);

   long simpleLength;
   char* simpleData = ctrl::toBinary(simple, simpleLength);

   long compositeLength;
   char* compositeData = ctrl::toBinary(composite, compositeLength);

   bool failed = false;
   try {
      CompositeClass* newComposite = ctrl::fromBinary<CompositeClass>(simpleData, simpleLength);
      delete newComposite;
   } catch(const ctrl::Exception& ex) {
      failed = true;
   }
   if (!failed) {
      delete[] simpleData;
      delete[] compositeData;
      return false;
   }

   failed = false;
   try {
      SimpleClass* newSimple = ctrl::fromBinary<SimpleClass>(compositeData, compositeLength);
      delete newSimple;
   } catch(const ctrl::Exception& ex) {
      failed = true;
   }

   delete[] simpleData;
   delete[] compositeData;

   if (!failed) {
      return false;
   }
   return true;
}

//******************************************************************************

class XmlAsAttributeMapElement {
public:

   bool operator==(XmlAsAttributeMapElement that) const {
      return m_foo == that.m_foo && m_bar == that.m_bar;
   }

   XmlAsAttributeMapElement(std::string foo, bool bar) : m_foo(foo), m_bar(bar) {}
   CTRL_BEGIN_MEMBERS(XmlAsAttributeMapElement)
   CTRL_MEMBER(private, std::string, m_foo)
         CTRL_WITH_NAME("foo")
         CTRL_AS_ATTRIBUTE()
   CTRL_MEMBER(private, bool, m_bar)
   CTRL_END_MEMBERS()
};

class XmlWithNameAsAttribute {
public:
   XmlWithNameAsAttribute(std::string name, int count) : m_name(name), m_count(count) {}

   void add(std::string key, std::string foo, bool bar) {
      m_map[key] = XmlAsAttributeMapElement(foo, bar);
   }

   bool operator==(XmlWithNameAsAttribute that) const {
      return m_name == that.m_name && m_count == that.m_count && m_map == that.m_map;
   }

   CTRL_BEGIN_MEMBERS(XmlWithNameAsAttribute)

      CTRL_MEMBER(private, std::string, m_name)
            CTRL_WITH_NAME("name")
            CTRL_AS_ATTRIBUTE()

      CTRL_MEMBER(private, int, m_count)

      typedef std::map<std::string, XmlAsAttributeMapElement> MyMap;
      CTRL_MEMBER(private, MyMap, m_map)
            CTRL_AS_ATTRIBUTE()

   CTRL_END_MEMBERS()
};

bool testXmlWithNameAsAttribute() {
   std::cout << "testXmlWithNameAsAttribute" << std::endl;
   std::cout << "--------------------------" << std::endl;

   XmlWithNameAsAttribute obj("Gerrit", 42);
   obj.add("G", "Gerrit", true);
   obj.add("A", "Alex", false);
   obj.add("P", "Paul", false);

   std::string xml = ctrl::toXml(obj);
   std::cout << xml << std::endl;

   std::string expected = std::string("<root name=\"Gerrit\">") +
                          "<__version>1</__version>" +
                          "<m_count>42</m_count>" +
                          "<m_map>" +
                          "<item key=\"A\">" +
                          "<value foo=\"Alex\">" +
                          "<m_bar>false</m_bar>" +
                          "</value>" +
                          "</item>" +
                          "<item key=\"G\">" +
                          "<value foo=\"Gerrit\">" +
                          "<m_bar>true</m_bar>" +
                          "</value>" +
                          "</item>" +
                          "<item key=\"P\">" +
                          "<value foo=\"Paul\">" +
                          "<m_bar>false</m_bar>" +
                          "</value>" +
                          "</item>" +
                          "</m_map>" +
                          "</root>";

   if (expected != xml) {
      std::cout << "Incorrect XML: ";
      return false;
   }

   XmlWithNameAsAttribute* newObj = ctrl::fromXml<XmlWithNameAsAttribute>(xml);
   return testAndDelete(newObj, obj, 0);
}

//******************************************************************************

namespace typemanip {

   struct T0 {};
   struct T1 {};
   struct T2 {};
   struct T3 {};
   struct T4 {};

   template <class T1_, class T2_>
   bool testSameType(T1_ t1, T2_ t2) {
      if (!ctrl::Private::IsSameType<T1_, T2_>::value) {
         return false;
      } else {
         return true;
      }
   }

   bool testTypeListContains() {
      std::cout << "testTypeListContains" << std::endl;
      std::cout << "--------------------" << std::endl;

      typedef ctrl::Private::TypeList<T0, ctrl::Private::TypeList<T1, ctrl::Private::TypeList<T2, ctrl::Private::NullType>>> TList;
      if (!ctrl::Private::TypeListContains<T1, TList>::value) {
         return false;
      }
      if (ctrl::Private::TypeListContains<T3, TList>::value) {
         return false;
      }
      return true;
   }

   bool testUniqueTypeList() {
      std::cout << "testUniqueTypeList" << std::endl;
      std::cout << "------------------" << std::endl;
      typedef ctrl::Private::TypeList<T0, ctrl::Private::TypeList<T0, ctrl::Private::TypeList<T0, ctrl::Private::NullType>>> AllSameTypeList;
      typedef typename ctrl::Private::UniquifyTypeList<AllSameTypeList>::Result SingleResult;
      if (!testSameType(SingleResult(), T0())) {
         return false;
      }

      typedef ctrl::Private::TypeList<T0, ctrl::Private::TypeList<T1, ctrl::Private::TypeList<T1, ctrl::Private::TypeList<T0,
               ctrl::Private::TypeList<T2, ctrl::Private::NullType>>>>> DuplicateTypeList;

      typedef typename ctrl::Private::UniquifyTypeList<DuplicateTypeList>::Result DuplicatesRemoved;
      typedef ctrl::Private::TypeList<T1, ctrl::Private::TypeList<T0, ctrl::Private::TypeList<T2, ctrl::Private::NullType>>> Expected;
      if (!testSameType(DuplicatesRemoved(), Expected())) {
         return false;
      }
      return true;
   }

} // namespace typemanip

namespace inheritance {

   class Base0 {
      CTRL_BEGIN_MEMBERS(Base0)
         CTRL_TYPE_ID_FIELD_NAME("base0")
      CTRL_MEMBER(private, int, val)
      CTRL_END_MEMBERS()
   };

   class Base1 {
      CTRL_BEGIN_MEMBERS(Base1)
         CTRL_TYPE_ID_FIELD_NAME("base1")
      CTRL_END_MEMBERS()
   };

   class Base2 : public Base0 {
      CTRL_BEGIN_MEMBERS(Base2)
      CTRL_BASE_CLASS(Base0)
      CTRL_END_MEMBERS()
   };

   class Multi1 : public Base1, public Base2 {
      CTRL_BEGIN_MEMBERS(Multi1)
      CTRL_BASE_CLASS(Base1)
      CTRL_BASE_CLASS(Base2)
      CTRL_END_MEMBERS()
   };

   class Multi2 : public Multi1 {
      CTRL_BEGIN_MEMBERS(Multi2)
      CTRL_BASE_CLASS(Multi1)
      CTRL_END_MEMBERS()
   };

   class Single1 : public Base2 {
      CTRL_BEGIN_MEMBERS(Single1)
      CTRL_BASE_CLASS(Base2)
      CTRL_END_MEMBERS()
   };

   class Diamond1 : public virtual Base0 {
      CTRL_BEGIN_MEMBERS(Diamond1)
      CTRL_BASE_CLASS(Base0)
      CTRL_END_MEMBERS()
   };

   class Diamond2 : public virtual Base0 {
      CTRL_BEGIN_MEMBERS(Diamond2)
      CTRL_BASE_CLASS(Base0)
      CTRL_END_MEMBERS()
   };

   class Single2 : public Diamond1, public Diamond2 {
      CTRL_BEGIN_MEMBERS(Single2)
      CTRL_BASE_CLASS(Diamond1)
      CTRL_BASE_CLASS(Diamond2)
      CTRL_END_MEMBERS()
   };

   template <class T1_, class T2_>
   bool singleRootChecker(T1_ concreteClass, T2_ expectedRoots, bool expected) {
      typedef ctrl::Private::DerivationRoots<T1_> DerivationRoots;
      if (!typemanip::testSameType(typename DerivationRoots::Roots(), expectedRoots)) {
         return false;
      } else if (DerivationRoots::isSingleRoot != expected) {
         return false;
      }
      return true;
   }

   bool testDerivationRoots() {
      std::cout << "testDerivationRoots" << std::endl;
      std::cout << "-------------------" << std::endl;
      if (!singleRootChecker(inheritance::Base0(), inheritance::Base0(), true)) return false;
      if (!singleRootChecker(inheritance::Base1(), inheritance::Base1(), true)) return false;
      if (!singleRootChecker(inheritance::Base2(), inheritance::Base0(), true)) return false;
      typedef ctrl::Private::TypeList<inheritance::Base1, ctrl::Private::TypeList<Base0, ctrl::Private::NullType>> Expected;
      if (!singleRootChecker(inheritance::Multi1(), Expected(), false)) return false;
      if (!singleRootChecker(inheritance::Multi2(), Expected(), false)) return false;
      if (!singleRootChecker(inheritance::Single1(), inheritance::Base0(), true)) return false;
      if (!singleRootChecker(inheritance::Single2(), inheritance::Base0(), true)) return false;
      return true;
   }

   CTRL_POLYMORPH(Base0)
   CTRL_POLYMORPH(Base1)
   CTRL_POLYMORPH(Base2)
   CTRL_POLYMORPH(Multi1)
   CTRL_POLYMORPH(Multi2)
   CTRL_POLYMORPH(Single1)
   CTRL_POLYMORPH(Single2)
   CTRL_POLYMORPH(Diamond1)
   CTRL_POLYMORPH(Diamond2)

   class Document {
      CTRL_BEGIN_MEMBERS(Document)
      CTRL_MEMBER(private, std::vector<std::shared_ptr<Base0>>, base0)
      CTRL_MEMBER(private, std::vector<std::shared_ptr<Base1>>, base1)
      CTRL_END_MEMBERS()

   public:
      static Document invalid() {
         Document doc;
         doc.base0.push_back(std::shared_ptr<Base0>(new Base0()));
         doc.base0.push_back(std::shared_ptr<Base0>(new Base2()));
         doc.base1.push_back(std::shared_ptr<Base1>(new Multi1()));
         return doc;
      }

      static Document valid() {
         Document doc;
         doc.base0.push_back(std::shared_ptr<Base0>(new Base0()));
         doc.base0.push_back(std::shared_ptr<Base0>(new Base2()));
         doc.base1.push_back(std::shared_ptr<Base1>(new Base1()));
         return doc;
      }

      static Document diamond() {
         Document doc;
         doc.base0.push_back(std::shared_ptr<Base0>(new Diamond1()));
         doc.base0.push_back(std::shared_ptr<Base0>(new Diamond2()));
         doc.base0.push_back(std::shared_ptr<Base0>(new Single2()));
         return doc;
      }
   };

   bool testSingleRootAssertion(Document doc, bool valid) {
      bool exceptionThrown = false;
      try {
         std::cout << ctrl::toXml(doc, true);
      } catch (...) {
         exceptionThrown = true;
      }
      return exceptionThrown == valid;
   }

   bool testSingleRootAssertion() {
      std::cout << "testSingleRootAssertion" << std::endl;
      std::cout << "-----------------------" << std::endl;
      if (testSingleRootAssertion(Document::valid(), true)) return false;
      if (testSingleRootAssertion(Document::invalid(), false)) return false;
      if (testSingleRootAssertion(Document::diamond(), true)) return false;
      return true;
   }

} // namespace inheritance

namespace idfield {

   class Double {
      CTRL_BEGIN_MEMBERS(Double)
      CTRL_MEMBER(private, int, foo)
            CTRL_AS_ID_FIELD()
      CTRL_MEMBER(private, int, bar)
            CTRL_AS_ID_FIELD()
      CTRL_MEMBER(private, int, baz)
      CTRL_END_MEMBERS()
   };

   class Single {
      CTRL_BEGIN_MEMBERS(Single)
      CTRL_MEMBER(private, int, foo)
      CTRL_MEMBER(private, int, bar)
            CTRL_AS_ID_FIELD()
      CTRL_MEMBER(private, int, baz)
      CTRL_END_MEMBERS()
   };

   class First {
      CTRL_BEGIN_MEMBERS(First)
      CTRL_MEMBER(private, int, foo)
            CTRL_AS_ID_FIELD()
      CTRL_MEMBER(private, int, bar)
      CTRL_MEMBER(private, int, baz)
      CTRL_END_MEMBERS()
   };

   class None {
      CTRL_BEGIN_MEMBERS(None)
      CTRL_MEMBER(private, int, foo)
      CTRL_MEMBER(private, int, bar)
      CTRL_MEMBER(private, int, baz)
      CTRL_END_MEMBERS()
   };

   bool testGetIdField() {
      std::cout << "testGetIdField" << std::endl;
      std::cout << "--------------" << std::endl;

      typedef ctrl::Private::RootHasIdField<Single> SingleId;
      if (SingleId::present != true) return false;
      if (SingleId::error != false) return false;
      if (SingleId::index != 1) return false;

      typedef ctrl::Private::RootHasIdField<First> FirstId;
      if (FirstId::present != true) return false;
      if (FirstId::error != false) return false;
      if (FirstId::index != 0) return false;

      typedef ctrl::Private::RootHasIdField<None> NoneId;
      if (NoneId::present != false) return false;
      if (NoneId::error != false) return false;
      if (NoneId::index != -1) return false;

      typedef ctrl::Private::RootHasIdField<Double> DoubleId;
      if (DoubleId::present != true) return false;
      if (DoubleId::error != true) return false;
      if (DoubleId::index != 1) return false;

      return true;
   }

} // namespace idfield

//******************************************************************************

class Employee {

   CTRL_BEGIN_MEMBERS(Employee)
   CTRL_MEMBER(public, std::string, m_ssid)
         CTRL_AS_ATTRIBUTE()
         CTRL_AS_ID_FIELD()
   CTRL_MEMBER(public, std::string, m_name)
   CTRL_MEMBER(public, int, m_age)
         CTRL_AS_ATTRIBUTE()
   CTRL_END_MEMBERS()

   Employee(std::string ssid, std::string name, int age)
         : m_ssid(ssid), m_name(name), m_age(age) {}

   bool operator==(const Employee& that) {
      return m_ssid == that.m_ssid && m_name == that.m_name && m_age == that.m_age;
   }
};

class Company {
   CTRL_BEGIN_MEMBERS(Company)
   CTRL_MEMBER(public, std::vector<std::shared_ptr<Employee>>, m_employees)
   CTRL_END_MEMBERS();

   Company(int dummy) {
      m_employees.push_back(std::shared_ptr<Employee>(new Employee("12345", "Gerrit", 36)));
      m_employees.push_back(std::shared_ptr<Employee>(new Employee("67890", "Alex", 42)));
      m_employees.push_back(m_employees[0]);
   }

   bool operator==(const Company& that) {
      if (m_employees.size() != that.m_employees.size()) {
         return false;
      }
      for (int i = 0; i < m_employees.size(); ++i) {
         if (!(*m_employees[i] == *that.m_employees[i])) {
            return false;
         }
      }
      return true;
   }
};

bool testCustomIdField() {
   std::cout << "testCustomIdField" << std::endl;
   std::cout << "-----------------" << std::endl;

   Company company(0);

   return testSerialization(company);
}

//******************************************************************************

int main() {
   typedef bool (*TestFunction)();

   std::vector<TestFunction> tests;

   tests.push_back(&testSimple);
   tests.push_back(&testComposite);
   tests.push_back(&testDerived);

   tests.push_back(&testWString);
   tests.push_back(&testSimplePair);
   tests.push_back(&testComplex);
   tests.push_back(&testValarray);

   tests.push_back(&testBitset);
   tests.push_back(&testDeque);
   tests.push_back(&testList);
   tests.push_back(&testMap);
   tests.push_back(&testMultimap);
   tests.push_back(&testQueue);
   tests.push_back(&testPriorityQueue);
   tests.push_back(&testSet);
   tests.push_back(&testMultiset);
   tests.push_back(&testStack);
   tests.push_back(&testVector);
   tests.push_back(&testArray);
   tests.push_back(&testForwardList);
   tests.push_back(&testUnorderedMap);
   tests.push_back(&testUnorderedMultimap);
   tests.push_back(&testUnorderedSet);
   tests.push_back(&testUnorderedMultiset);

   tests.push_back(&testSharedPtr);
   tests.push_back(&testWeakPtr);
   tests.push_back(&testAutoPointer);
   tests.push_back(&testStdSharedPtr);
   tests.push_back(&testStdWeakPtr);
   tests.push_back(&testUniquePointer);
   tests.push_back(&testUniquePointerVector);
   tests.push_back(&testRawPointer);

   tests.push_back(&testPolymorph);
   tests.push_back(&testPureVirtualFunction);
   tests.push_back(&testMultipleInheritance0);
   tests.push_back(&testMultipleInheritance1);
   tests.push_back(&testMultipleInheritance2);

   tests.push_back(&testInitialize);
   tests.push_back(&testLittleEndian);
   tests.push_back(&testBigEndian);
   tests.push_back(&testWithVersion);
   tests.push_back(&testCorruptData);

   tests.push_back(&testXmlWithNameAsAttribute);

   tests.push_back(&typemanip::testTypeListContains);
   tests.push_back(&typemanip::testUniqueTypeList);
   tests.push_back(&inheritance::testDerivationRoots);
   tests.push_back(&inheritance::testSingleRootAssertion);
   tests.push_back(&idfield::testGetIdField);
   tests.push_back(&testCustomIdField);

   for ( typename std::vector<TestFunction>::const_iterator iter = tests.begin();
           iter != tests.end(); ++iter ) {
      bool result = (*iter)();
      if (result) {
         std::cout << "SUCCESS" << std::endl << std::endl;
      } else {
         std::cout << "FAILURE" << std::endl;
         break;
      }
   }
}
