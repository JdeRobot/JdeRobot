#include <jde+/data.h>
#include <jde+/pointer.h>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

std::ostream& put_default(std::ostream& os, unsigned char* ptr) {
  os << "(default data put_f)";
  return os;
}

struct Data_Pimpl {
  Data_Pimpl(const std::string& type, const unsigned int size, 
	     const Pointer<unsigned char> data_ptr,
	     const Data::put_f_t put_f);
  std::string mytype;
  unsigned int mysize;
  Pointer<unsigned char> mydata_ptr;
  Data::put_f_t myput_f;
};

Data_Pimpl::Data_Pimpl(const std::string& type, const unsigned int size, 
		       const Pointer<unsigned char> data_ptr,
		       const Data::put_f_t put_f)
  :mytype(type),mysize(size),mydata_ptr(data_ptr), myput_f(put_f) {}

Data::Data()
  :pimpl(new Data_Pimpl("null",0,Pointer<unsigned char>::Pointer(NULL),
			put_default)) {}

Data::Data(const std::string& data_type, const unsigned int data_size,
           const put_f_t user_put_f, unsigned char* const data_ptr)
	   //const bool copy)
  :pimpl(new Data_Pimpl(data_type,data_size,
			Pointer<unsigned char>::Pointer(NULL),user_put_f)) {
  //if (copy){
    /*copy Data*/
    pimpl->mydata_ptr = 
      Pointer<unsigned char>::Pointer(new unsigned char[pimpl->mysize],true);

    if (data_ptr != NULL)
      memmove(pimpl->mydata_ptr.get(),data_ptr,pimpl->mysize);
    //}
}

Data::Data(const Data& x)
  :pimpl(new Data_Pimpl(x.pimpl->mytype,x.pimpl->mysize,
			Pointer<unsigned char>::Pointer(NULL),
			x.pimpl->myput_f)){
  /*copy Data*/
  if (x.pimpl->mytype != "null") {
    pimpl->mydata_ptr = 
      Pointer<unsigned char>::Pointer(new unsigned char[pimpl->mysize],true);
    memmove(pimpl->mydata_ptr.get(),x.pimpl->mydata_ptr.get(),pimpl->mysize);
  }
}

Data::Data(Data* const x)
  :pimpl( new Data_Pimpl(x->pimpl->mytype,x->pimpl->mysize,
			 x->pimpl->mydata_ptr,x->pimpl->myput_f)) {}

Data::~Data() {
  delete pimpl;
}

std::string const& Data::get_type() const {
  return pimpl->mytype;
}

unsigned int Data::get_size() const {
  return pimpl->mysize;
}

unsigned char* Data::get_data_ptr() const {
  return pimpl->mydata_ptr.get();
}



Data& Data::operator=(const Data& x) {
  if (this != &x) {
    if ((pimpl->mytype != "null") and
	(pimpl->mytype != x.pimpl->mytype))
      throw std::logic_error("Data: Trying to assign different data types(" 
			     + pimpl->mytype + "!=" + x.pimpl->mytype + ")");

    if (x.pimpl->mytype != "null") {
      pimpl->mytype = x.pimpl->mytype;
      if (pimpl->mysize != x.pimpl->mysize) {/*realloc if
					       different size*/
	pimpl->mysize = x.pimpl->mysize;
	pimpl->mydata_ptr = 
	  Pointer<unsigned char>::Pointer(new unsigned char[pimpl->mysize],true);
      }
      memmove(pimpl->mydata_ptr.get(),x.pimpl->mydata_ptr.get(),pimpl->mysize);
    }
  }
  return *this;
}

std::ostream& Data::put(std::ostream& os) const {
  return pimpl->myput_f(os,pimpl->mydata_ptr.get());
}

const Data Data::Null = Data();
/*call put virtual method*/
std::ostream& operator<<(std::ostream& os, const Data& d) {
  return d.put(os);
}
