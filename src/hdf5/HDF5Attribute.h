#ifndef __H5ATTRIBUTE__
#define __H5ATTRIBUTE__

#include <string>
#include <vector>
#include <hdf5.h>
#include "hdf5/types/hid_gc.h"
#include "hdf5/HDF5Node.h"

namespace LDA {

/*!
 * Represents core functionality for an attribute inside a group.
 */
class AttributeBase: public HDF5Node {
public:
  /*!
   * Returns whether this attribute exists in the HDF5 file.
   */
  bool exists() const { return H5Aexists(container, _name.c_str()) > 0; }

  /*!
   * Removes this element from the HDF5 file. Useful for porting, 
   * repairing, or otherwise modifying files on a structural level.
   */
  void remove() const { if (H5Adelete(container, _name.c_str()) < 0) throw HDF5Exception("Could not delete element"); }

protected:
  AttributeBase( const hid_gc &container, const std::string &name ): HDF5Node(name), container(container) {}

  /*!
   * Returns the number of data points in this element (1 for a scalar, >= 0 for a vector)
   */
  size_t size() const;

  const hid_gc container;
};

/*!
 * Represents an attribute containing a scalar or a string.
 */
template<typename T> class Attribute: public AttributeBase {
public:
  Attribute( const hid_gc &container, const std::string &name ): AttributeBase(container, name) {}

  /*!
   * Returns the value of this attribute, retrieved from the HDF5 file. An exception
   * is thrown if the attribute does not already exist.
   */
  T get() const;

  /*!
   * Stores the value of this attribute in the HDF5 file. The attribute is created
   * if it does not already exist.
   */
  void set( const T &value ) const;
};

/*!
 * Represents an attribute containing a vector of scalars or strings.
 */
template<typename T> class Attribute< std::vector<T> >: public AttributeBase {
public:
  Attribute( const hid_gc &container, const std::string &name ): AttributeBase(container, name) {}

  /*!
   * Returns the value of this attribute, retrieved from the HDF5 file. An exception
   * is thrown if the attribute does not already exist.
   */
  std::vector<T> get() const;

  /*!
   * Stores the value of this attribute in the HDF5 file. The attribute is created
   * if it does not already exist.
   */
  void set( const std::vector<T> &value ) const;
};

}

#include "hdf5/HDF5Attribute.tcc"

#endif

