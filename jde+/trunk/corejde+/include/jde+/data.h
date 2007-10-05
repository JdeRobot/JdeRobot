#ifndef DATA_H
#define DATA_H
#include <string>
#include <iostream>

/*default put function*/
std::ostream& put_default(std::ostream& os, unsigned char* ptr);

class Data_Pimpl;

class Data {
public:
  /* typedef void* (* alloc_f_t)(void*); */
  /*    typedef void (*dealloc_f_t)(void*); */
  /*    typedef void (* copy_f_t)(void* dst, void* src); */
  /*    typedef size_t (*size_f_t)(void*); */
  typedef std::ostream& (* put_f_t)(std::ostream&, unsigned char*);

  /*Constructors*/
  /*
    Initialize a null data object
  */
  Data();

  /*
    Initialize a data object. If 'copy' true allocate
    mem and make a copy of data_ptr. On destructor 
    allocated mem is released.
  */
  Data(const std::string& data_type, const unsigned int data_size,
       const put_f_t user_put_f = put_default,
       unsigned char* const data_ptr = NULL);
       //const bool copy = true);

  /*
    Copy x object
  */
  Data(const Data& x);

  /*Copy sharing memory*/
  Data(Data* const x);

  /*Destructor*/
  ~Data();

  /*Get type of Data*/
  std::string const& get_type() const;

  /*Get size of Data*/
  unsigned int get_size() const;

  /*Get a pointer to Data*/
  unsigned char* get_data_ptr() const;

  /*Copy x Data object*/
  Data& operator=(const Data& x);

  /**
     Write data into ostream.
  */
  std::ostream& put(std::ostream& os) const;

  /**
     Null data
  */
  static const Data Null;
protected:
  Data_Pimpl* pimpl;
};

/**
   Output operator.
*/
std::ostream& operator<<(std::ostream& os, const Data& d);

#endif /*DATA_H*/
