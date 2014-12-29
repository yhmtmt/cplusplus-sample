#include <iostream>
#include <map>
#include <string.h>

using namespace std;

//////////////////////// This is a sample code. ///////////////////////////
// This sample code examine a new technique related to factory function.
// The techinique could be beneficial if you need to instantiate classes 
// by the name given as a string. (This may be a kind of reflection.)
// The insertion of the instantiation code is abstracted by the registerCls
// method in the base class. 

class Base;

// Definition of pointer of instantiation function associated to the class
typedef Base * (*CreateCls)();

// Template function of the instantiation function
template <class T> Base * createCls()
{
  return reinterpret_cast<Base*>(new T());
}

// comparison function used in the map 
struct cmp { 
  bool operator () (const char *a,const char *b) const 
  {
    return strcmp(a,b) < 0;
  } 
};

typedef map<const char *, CreateCls, cmp> Map;

//////////////////////////////////////////////////////////////// Base class
class Base
{
public:
  // Table associating class name and instantiation function
  static Map  childs;

  // The function for registering child class
  template <class T> static void registerCls(const char * nameCls)
  {
    childs.insert(Map::value_type(nameCls, createCls<T>));
  }

  // fuctory function creates an class instance associated with the given name.
  static Base * create(const char * nameCls)
  {
    return childs[nameCls]();
  }

  // class interface to be over written.
  virtual const char * getClsName() = 0;
};

map<const char *, CreateCls, cmp> Base::childs;

///////////////////////////////// children to be instantiated via the factory
class Foo: public Base
{
public:
  virtual const char * getClsName(){
    return "Foo";
  }
};

class Bar: public Base
{
public:
  virtual const char * getClsName(){
    return "Bar";
  }
};

////////////////////////////////////////////////////// Testing the framework 
int main(int argc, char ** argv)
{
  // registering children to the talbe in the base class.
  Base::registerCls<Foo>("Foo");
  Base::registerCls<Bar>("Bar");

  // instantication test
  Base * pFoo = Base::create("Foo");
  Base * pBar = Base::create("Bar");
  
  // prinitng the fact
  cout << "The class name of pFoo is " << pFoo->getClsName() << endl;
  cout << "The class name of pBar is " << pBar->getClsName() << endl;

  return 1;
}
