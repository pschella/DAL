%module LDA

// -------------------------------
// Documentation
// -------------------------------

// first generate signatures using SWIG's knowledge
%feature("autodoc",1);

// then append documentation using doxygen's knowledge
%include "doc/docstrings.i"

// -------------------------------
// Exception handling
// -------------------------------

%include "exception.i"

%{
#include "hdf5core/h5exception.h"
%}

%exception {
  try {
    $action
  } catch (const LDA::HDF5Exception &e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

// -------------------------------
// Type marshalling - scalars
// -------------------------------

typedef long ssize_t;
typedef unsigned long size_t;

// -------------------------------
// STL/C++ templates
// -------------------------------

%include "std_complex.i"

%include "std_string.i"

%include "std_vector.i"

namespace std {
  %template(VectorSizeT)        vector<size_t>;
  %template(VectorSSizeT)       vector<ssize_t>;
  %template(VectorUnsigned)     vector<unsigned>;
  %template(VectorString)       vector<string>;
};


// -------------------------------
// Type marshalling - arrays
// -------------------------------

%{
  #define SWIG_FILE_WITH_INIT
%}

%include "external/numpy.i"

%init %{
  import_array();
%}

%define DATASETTYPE( datatype, numpytype, indextype )

// tell numpy which combinations of data types and index types we use
%numpy_typemaps(datatype, numpytype, indextype)

// tell SWIG how we call our dimension and array parameters
%apply (indextype DIM1, datatype* INPLACE_ARRAY1) {(indextype dim1, datatype *outbuffer1)}
%apply (indextype DIM1, indextype DIM2, datatype* INPLACE_ARRAY2) {(indextype dim1, indextype dim2, datatype *outbuffer2)}

%apply (indextype DIM1, datatype* IN_ARRAY1) {(indextype dim1, const datatype *inbuffer1)}
%apply (indextype DIM1, indextype DIM2, datatype* IN_ARRAY2) {(indextype dim1, indextype dim2, const datatype *inbuffer2)}

%enddef

// enumerate all the dataset types that we refer to
DATASETTYPE(short, NPY_INT16, size_t);
DATASETTYPE(float, NPY_FLOAT32, size_t);
DATASETTYPE(std::complex<float>, NPY_COMPLEX64, size_t);

// -------------------------------
// LDA classes and templates
// -------------------------------

%{
  #include "BF_File.h"
  #include "TBB_File.h"

  using namespace LDA;
%}

// rename otherwise unreachable functions
%rename(get_hid_t)  operator hid_t;
%rename(get_hid_gc) operator hid_gc;

// ignore functions that contain raw pointers that
// cannot be marshalled.
%ignore *::getMatrix;
%ignore *::setMatrix;

%include HDF5Node.h
%include HDF5Attribute.h
%include HDF5FileBase.h
%include HDF5GroupBase.h
%include HDF5DatasetBase.h

namespace LDA {
  using namespace std;

  %template(AttributeBool)      Attribute<bool>;
  %template(AttributeUnsigned)  Attribute<unsigned>;
  %template(AttributeDouble)    Attribute<double>;
  %template(AttributeString)    Attribute<string>;

  %template(AttributeVUnsigned) Attribute< vector<unsigned> >;
  %template(AttributeVString)   Attribute< vector<string> >;

  %template(HDF5DatasetBaseShort)        HDF5DatasetBase<short>;
  %template(HDF5DatasetBaseFloat)        HDF5DatasetBase<float>;
  %template(HDF5DatasetBaseComplexFloat) HDF5DatasetBase< std::complex<float> >;
}

%include CommonAttributesFile.h
%include BF_File.h
%include TBB_File.h

// -------------------------------
// Class extensions for bindings
// -------------------------------

%pythoncode %{
  import numpy

  # record the numpy datatypes used in the various datasets
  HDF5DatasetBaseShort.dtype = numpy.int16
  HDF5DatasetBaseFloat.dtype = numpy.float32
  HDF5DatasetBaseComplexFloat.dtype = numpy.complex64

  del numpy
%}

