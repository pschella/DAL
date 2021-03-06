/* Copyright 2011-2012  ASTRON, Netherlands Institute for Radio Astronomy
 * This file is part of the Data Access Library (DAL).
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DAL_FILE_H
#define DAL_FILE_H

#include <string>
#include <hdf5.h>
#include "dal/hdf5/types/hid_gc.h"
#include "dal/hdf5/Group.h"
#include "dal/hdf5/Attribute.h"

namespace DAL {

class File: public Group {
public:
  enum fileMode { READ = 1, READWRITE = 2, CREATE = 3, CREATE_EXCL = 4 };

  /*!
   * Open or create an HDF5 file called `filename`.
   *
   * Python example:
   * \code
   *    # Create and close a new HDF5 file called "example.h5"
   *    >>> f = File("example.h5", File.CREATE)
   *    >>> del f
   *
   *    # Open (and close) the same file for reading
   *    >>> f = File("example.h5", File.READ)
   *    >>> del f
   *
   *    # Clean up
   *    >>> import os
   *    >>> os.remove("example.h5")
   * \endcode
   */
  File( const std::string &filename, enum fileMode mode, const std::string &versionAttrName );

  /*!
   * Commit any changes to disk.
   */
  void flush();

  virtual bool exists() const { return true; }

  /*!
   * The name of the file.
   */
  const std::string filename;

  /*!
   * The mode in which the file is opened.
   */
  const fileMode mode;

  /*!
   * The name of the attribute containing the file version. Cannot be "".
   */
  const std::string versionAttrName;

  /*!
   * Stores the given version in the HDF5 file.
   *
   * Python example:
   * \code
   *    # Create and close a new HDF5 file called "example.h5"
   *    >>> f = File("example.h5", File.CREATE)
   *
   *    # Set and get the file version
   *    >>> f.setFileVersion(VersionType(1,2,3))
   *
   *    # fileVersion() reports the version
   *    >>> str(f.fileVersion())
   *    '1.2.3'
   *
   *    # Groups and attributes inherit the Version
   *    >>> g = Group(f, "GROUP")
   *    >>> str(g.fileVersion())
   *    '1.2.3'
   *
   *    # Note: changing the version does not affect
   *    # already existing group objects.
   *    >>> f.setFileVersion(VersionType(4,5,6))
   *    >>> str(f.fileVersion())
   *    '4.5.6'
   *    >>> str(g.fileVersion())
   *    '1.2.3'
   *
   *    # Reload other objects to refresh the file info,
   *    # including the newly set version.
   *    >>> g = Group(f, "GROUP")
   *    >>> str(g.fileVersion())
   *    '4.5.6'
   *
   *    # Clean up
   *    >>> import os
   *    >>> os.remove("example.h5")
   * \endcode
   */
  void setFileVersion( const VersionType &version );

protected:
  /*!
   * Return the version as stored in the HDF5 file.
   * A default value is returned if the attribute does not exist.
   */
  VersionType getStoredFileVersion();

private:
  hid_gc open( const std::string &filename, enum fileMode mode ) const;
};

}

#endif

