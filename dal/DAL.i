// -------------------------------
// Documentation
// -------------------------------

%define DOCSTRING
"DAL implements a Data Access Layer for dal/lofar data."
%enddef

%module(docstring=DOCSTRING) DAL

// first generate signatures using SWIG's knowledge
%feature("autodoc",1);

// -------------------------------
// Type marshalling - scalars
// -------------------------------

%include "dal/size_types.i"

// -------------------------------
// STL/C++ templates
// -------------------------------

%include "std_complex.i"
%include "std_string.i"
%include "dal/vectors.i"

// -------------------------------
// Exception handling
// -------------------------------

%include "dal/hdf5/exceptions/exceptions.i"

// -------------------------------
// DAL classes and templates
// -------------------------------

%{
  #include "dal/lofar/Station.h"
  #include "dal/lofar/CommonTuples.h"
  #include "dal/lofar/BF_File.h"
  #include "dal/lofar/TBB_File.h"

  using namespace DAL;
%}

%include "dal/hdf5/types/h5tuple.i"
%include "dal/hdf5/types/versiontype.i"

%include "dal/hdf5/Node.i"
%include "dal/hdf5/Attribute.i"
%include "dal/hdf5/Group.i"
%include "dal/hdf5/Dataset.i"
%include dal/hdf5/File.h

%include dal/lofar/Station.h
%include "dal/lofar/CommonTuples.i"
%include dal/lofar/CLA_File.h
%include dal/lofar/Coordinates.h
%include dal/lofar/BF_File.h
%include "dal/lofar/TBB_File.i"

// -------------------------------
// Clean up the DAL namespace
// -------------------------------

%pythoncode %{
for obj in locals().keys():
  if (obj.endswith(   ("__typeName", "__castNode", "_size", "_swigregister"))
   or obj.startswith( ("_swig_", "_TupleBase"))
   or obj in          ("_object", "_newclass")):
    del locals()[obj]

del locals()["obj"]
%}

// -------------------------------
// Run doctest if module is executed
// -------------------------------

%pythoncode %{
# doctest engine
def _doctest():
  """ Run all doctests. """

  import doctest
  import sys

  failures, tests = doctest.testmod(optionflags=doctest.ELLIPSIS | doctest.IGNORE_EXCEPTION_DETAIL | doctest.NORMALIZE_WHITESPACE)

  if failures == 0:
    sys.exit(0)
  else:  
    sys.exit(1)

if __name__ == "__main__":
  _doctest()
%}

