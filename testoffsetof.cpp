#include <iostream>
#include <stddef.h>

using namespace std;

// PROBLEM: The "offsetof" trick is not allowed for non-POD object.
// SOLUTION: Embedding POD object into non-POD object.
// This sample shows how to get the pointer of non-POD object from its members.
// The trick is in using an embedded POD object includes an anchor object and 
// this pointer of the parent object. 

class CBase{
public:
  // Embedded POD object. 
  struct SBase{
    int anchor; // Anchor object. Any type is OK.
    CBase * ptr;// Initialized as this pointer in CBase().
  } bmem;

  CBase(){
    bmem.anchor = 1;
    bmem.ptr = this;
  };
  ~CBase(){};
  virtual void func() = 0;
};

class CSub1: public CBase{
public:
  float v;
  CSub1(): v(1.){};
  virtual ~CSub1(){};
  virtual void func(){
    cout << "CSub1" << endl;
  }
};

class CSub2: public CBase{
public:
  double v;
  CSub2(): v(1.){};
  virtual ~CSub2(){};
  virtual void func(){
    cout << "CSub2" << endl;
  }
};

int main(int argc, char ** argv)
{
  // Instances of both child classes.
  CSub1 s1;
  CSub2 s2;
  
  // Setting the anchor pointers of the instances. 
  int * panchor1 = &s1.bmem.anchor;
  int * panchor2 = &s2.bmem.anchor;

  // Retrieving CBase pointer using anchor object.
  CBase * pb1 = ((CBase::SBase*) ((char*) panchor1 - offsetof(CBase::SBase, anchor)))->ptr;
  CBase * pb2 = ((CBase::SBase*) ((char*) panchor2 - offsetof(CBase::SBase, anchor)))->ptr;

  // Teting virtual functions.
  pb1->func();
  pb2->func();
  return 0;
}
