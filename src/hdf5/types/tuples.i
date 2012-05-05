// SWIG customisations for the TupleBase classes

%{
  #include <sstream>
  #include <vector>
%}

// make DAL::TupleBase more pythonic
%ignore DAL::TupleBase::operator[];
%ignore DAL::TupleBase::begin;
%ignore DAL::TupleBase::end;
%ignore DAL::TupleUntemplated;

// make DAL::TupleBase more pythonic
%extend DAL::TupleBase {
  T __getitem__(ssize_t index) {
    if (index < 0)
      index += $self->size();

    if (index >= $self->size())
      throw DAL::DALIndexError("list index out of range");

    return $self->operator[](index);
  }

  void __setitem__(ssize_t index, const T& value) {
    if (index < 0)
      index += $self->size();

    if (index >= $self->size())
      throw DAL::DALIndexError("list index out of range");

    $self->operator[](index) = value;
  }

  size_t __len__() {
    return $self->size();
  }

  std::string __str__() {
    std::ostringstream oss(std::ostringstream::out);

    oss << "(";

    for (size_t i = 0; i < $self->size(); i++) {
      oss << $self->operator[](i);

      if (i != $self->size() - 1)
        oss << ", ";
    }

    oss << ")";

    return oss.str();
  }
}

%include hdf5/types/h5tuple.h

namespace DAL {
  // TupleBases need to be instantiated to force that they will be %extended
  %template(_TupleBaseUnsigned2)     TupleBase<unsigned,2>;
  %template(_TupleBaseUnsigned3)     TupleBase<unsigned,3>;
  %template(_TupleBaseUnsigned4)     TupleBase<unsigned,4>;
  %template(_TupleBaseFloat2)        TupleBase<float,2>;
  %template(_TupleBaseFloat3)        TupleBase<float,3>;
  %template(_TupleBaseFloat4)        TupleBase<float,4>;
  %template(_TupleBaseDouble2)       TupleBase<double,2>;
  %template(_TupleBaseDouble3)       TupleBase<double,3>;
  %template(_TupleBaseDouble4)       TupleBase<double,4>;

  %template(TupleUnsigned2)     Tuple<unsigned,2>;
  %template(TupleUnsigned3)     Tuple<unsigned,3>;
  %template(TupleUnsigned4)     Tuple<unsigned,4>;
  %template(TupleDouble2)       Tuple<double,2>;
  %template(TupleDouble3)       Tuple<double,3>;
  %template(TupleDouble4)       Tuple<double,4>;
}

// std::vector variants
namespace std {
  %template(VectorTupleUnsigned2)     vector< DAL::Tuple<unsigned,2> >;
  %template(VectorTupleUnsigned3)     vector< DAL::Tuple<unsigned,3> >;
  %template(VectorTupleUnsigned4)     vector< DAL::Tuple<unsigned,4> >;
  %template(VectorTupleDouble2)       vector< DAL::Tuple<double,2> >;
  %template(VectorTupleDouble3)       vector< DAL::Tuple<double,3> >;
  %template(VectorTupleDouble4)       vector< DAL::Tuple<double,4> >;
}

%pythoncode %{

# SWIG generates __repr__ members for all classes, so we need to explicitly override them all

# Add __repr__ functions to all tuples

def tuple_repr(self):
  return "(" + ", ".join([x.__repr__() for x in self]) + ")"

g = globals().copy()

for name, obj in g.iteritems():
  if name.startswith("Tuple") and type(obj) == type:
    obj.__repr__ = tuple_repr
    obj.__str__  = tuple_repr

%}  