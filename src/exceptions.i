// -------------------------------
// Exception handling
// -------------------------------

%include "exception.i"

%{
#include "hdf5/exceptions/h5exception.h"
#include "hdf5/exceptions/h5errorstack.h"
#include <string>
%}

/*
 * Create a custom HDF5Exception in Python.
 */
#ifdef SWIGPYTHON

%{
  static PyObject* pHDF5Exception;
%}

%init %{
  pHDF5Exception = PyErr_NewException(const_cast<char*>("_DAL.HDF5Exception"), PyExc_RuntimeError, NULL);
  Py_INCREF(pHDF5Exception);
  PyModule_AddObject(m, "HDF5Exception", pHDF5Exception); // 'm' is the name of the module object in SWIG
%}

%pythoncode %{
  # marshall the exception from the shadow module
  HDF5Exception = _DAL.HDF5Exception

  # also record the HDF5 error stack when an exception is thrown
  HDF5Exception.__old_init = HDF5Exception.__init__

  def new_init(self, *args, **kwargs):
      self.__old_init(*args, **kwargs)

      # Keep a reference to the HF5ErrorStack until we copied the stack,
      # or SWIG will become confused.

      stackobj = HDF5ErrorStack()
      self.stack = list(stackobj.stack)

  HDF5Exception.__init__ = new_init    
%}

#endif  

/*
 * Catch and marshall all C++ exceptions.
 */
%exception {
  try {
    $action

  // Catch DAL exception classes

  } catch (const DAL::HDF5Exception &e) {
    const std::string msg = std::string(e.what()) + ": " + e.stackSummary();

#ifdef SWIGPYTHON
    PyErr_SetString(pHDF5Exception, const_cast<char*>(msg.c_str()));
    return NULL;
#else
    SWIG_exception(SWIG_RuntimeError, msg.c_str());
#endif

  } catch (const DAL::DALIndexError &e) {
    SWIG_exception(SWIG_IndexError, e.what());
  } catch (const DAL::DALValueError &e) {
    SWIG_exception(SWIG_ValueError, e.what());
  } catch (const DAL::DALException &e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }  

  // Catch standard C++ exception classes (including std::exception)
  SWIG_CATCH_STDEXCEPT
}

/*
 * Marshall and extend HDF5 error stack classes.
 */
%include "hdf5/exceptions/h5errorstack.h"

namespace std {
  %template(VectorStackLine)        vector<DAL::HDF5StackLine>;
}

%extend DAL::HDF5StackLine {
  std::string __repr__() {
    return std::string("\"") + $self->longDesc() + "\"";
  }

  std::string __str__() {
    return $self->shortDesc();
  }
}
