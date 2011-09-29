#ifndef __HDF5GROUP__
#define __HDF5GROUP__

#include <string>
#include <hdf5.h>
#include "hdf5core/hid_gc.h"
#include "hdf5core/h5attribute.h"
#include "HDF5Node.h"

class HDF5GroupBase: public HDF5Node {
public:
  HDF5GroupBase( const HDF5GroupBase &other );
  virtual ~HDF5GroupBase();

  virtual void create();
  bool exists() const;
  void remove() const;
  void set( const HDF5GroupBase &other, bool deepcopy );

  Attribute<std::string> groupType();

protected:
  HDF5GroupBase( const hid_gc &parent, const std::string &name );

  const hid_gc parent;
  hid_gc *_group;

  const hid_gc &group() {
    // deferred opening of group, as it may need to be created first
    if (!_group)
      _group = open(parent, _name);

    return *_group;
  }

  virtual hid_gc *open( hid_t parent, const std::string &name ) const;
};

#endif
