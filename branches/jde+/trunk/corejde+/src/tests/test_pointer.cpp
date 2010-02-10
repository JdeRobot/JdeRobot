#include <jde+/pointer.h>
#include <iostream>

class A {
public:
	virtual void print() { std::cout << "Clase A" << std::endl; }
};

class B : public A {
public:
	virtual void print() { std::cout << "Clase B" << std::endl; }
};

int main(){
  {
	Pointer<B> pB( new B() );
	Pointer<A> pA(pB);

	pA->print();
	pB->print();

	pB = Pointer<B>::Pointer(NULL);
	pA = Pointer<A>::Pointer(NULL);
  }
  std::cout << "Fin" << std::endl;
}
