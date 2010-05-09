#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// output formatting helper
void indent(int depth) {
  for(int i = 0; i < depth; ++i)
    printf("  ");
}

template <class T>
void dump_memory(const T * p, size_t items, const char * title, int depth) {
  indent(depth);
  printf("%08x -> %s\n", p, title);
  // Make a format string that prints hex with correct number of leading zeroes
  stringstream sstr;
  sstr << "%0" << sizeof(T) * 2 << "x ";
  string format = sstr.str();
  indent(depth + 1);
  const T * end = p + items;
  for(const T * chunk = p; chunk < end; ++chunk) {
    printf(format.c_str(), *chunk);
  }
  printf("\n");
}

// Pretend all pointers are to int to reduce number of casts
template<class T> 
void dump_vtable(const T& t, int virtuals) {
  int * instance = (int *) &t;
  // In memory, the first bytes of the instance are occupied by the vtable
  int * vtable = (int *) &instance;
  int * __vptr = (int *) *vtable;
  int vtable_size = virtuals * sizeof(int);
  indent(2);
  printf("vtbl (%08x - %08x)", *__vptr, *__vptr + vtable_size - 1);
  printf(" %d entr%s:\n", virtuals, ((virtuals == 1) ? "y" : "ies"));
  for(int i = 0; i < virtuals; ++i) {
    int * fptr = ((int **) __vptr)[i];
    stringstream sstr;
    sstr << "fptr " << i << ":";
    dump_memory(fptr, 1, sstr.str().c_str(), 3);
    dump_memory((unsigned char *) *fptr, 16, "implementation (first 16 bytes):", 4);
  }
}

template<class T>
string object_size(const T& t) {
  stringstream sstr;
  sstr << sizeof(t) << " byte";
  if (sizeof(t) != 1)
    sstr << "s";
  return sstr.str();
}

template<class T> 
void dump_instance(const char * title, const T& t, int virtuals = 0) {
  cout << title << endl;
  stringstream sstr;
  sstr << "instance (" << object_size(t) << "):";
  dump_memory((unsigned char *) &t, sizeof(t), sstr.str().c_str(), 1);
  if (virtuals > 0)
    dump_vtable(t, virtuals);
  cout << endl;
}

class A {
};

class B {
  int i;
};

class C {
  char ch;
  int i;
};

class D {
  int i;
  char ch;
};

class E {
  virtual void f() {}
};

class F {
public:
  F() : fdata(0xffffffff) {}; // Initialize with easy-to-spot data to make dumps clearer
  virtual int f(void) = 0;
private:
  int fdata;
};

class F1 : F {
public:
  F1() : f1data(0xf1f1f1f1) {}; // Initialize with easy-to-spot data to make dumps clearer
  virtual int f(void) { return 0x33333333; }
private:
  int f1data;
};

int main(int argc, char* argv[])
{
  A a;
  dump_instance("An empty class:", a);
  B b;
  dump_instance("A class with an int member:", b);
  C c;
  dump_instance("A class with char and int members:", c);
  D d;
  dump_instance("A class with int and char members:", d);
  E e;
  dump_instance("A class with a virtual member function:", e, 1); // We supply the number of virtual methods
  F1 f1;
  dump_instance("A subclass of an abstract class:", f1, 1);

	return 0;
}
