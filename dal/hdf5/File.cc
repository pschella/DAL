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
#include "File.h"

using namespace std;

namespace DAL {

/*!
 * Open filename with mode and treat versionAttrName as the version attribute name.
 *
 * Note on reopening: HDF5 cannot reopen the same file (same or different mode) until
 * it has closed the previous open. HDF5 can only close once all opened subgroups etc
 * have been closed. As long as this is not the case, these references remain valid
 * and HDF5 keeps the file open. See the HDF5 manual on H5Fclose() for more detail.
 */
File::File( const std::string &filename, enum File::fileMode mode, const std::string &versionAttrName )
:
  Group(open(filename, mode)),
  filename(filename),
  mode(mode),
  versionAttrName(versionAttrName)
{
  // Fill the data that will be propagated to other Nodes
  data.fileVersion = getStoredFileVersion();

  switch (mode) {
    case CREATE:
    case CREATE_EXCL:
    case READWRITE:
      data.canWrite = true;
      break;

    case READ:
    default:
      data.canWrite = false;
  }

  data.fileName = filename;
  data.parentNodePath = "";
}

hid_gc File::open( const std::string &filename, enum File::fileMode mode ) const
{

  switch (mode) {
    case CREATE:
    case CREATE_EXCL:
      {
        hid_gc_noref fapl(H5Pcreate(H5P_FILE_ACCESS), H5Pclose, "Could not create file access property list to create file " + filename);

        /*
         * We want to use 1.8 features, but always be able to read back any created file.
         * We can force min and max versions, but apart from H5F_LIBVER_EARLIEST and H5F_LIBVER_LATEST,
         * there is only H5F_LIBVER_18 introduced in 1.8.6+, but not in 1.9(.108) (pre-1.10).
         * For now, set H5F_LIBVER_LATEST on all 1.8 and 1.9, and guess we may benefit from LIBVER_18 at 1.10+.
         */
#if H5_VERS_MAJOR == 1 && H5_VERS_MINOR <= 9
        if (H5Pset_libver_bounds(fapl, H5F_LIBVER_LATEST, H5F_LIBVER_LATEST) < 0)
#else // >= 1.10
        if (H5Pset_libver_bounds(fapl, H5F_LIBVER_18, H5F_LIBVER_18) < 0)
#endif
          throw DALException("Could not set HDF5 version bounds to create forward compatible file for HDF5 1.8 features");

        unsigned flags;
        if (mode == CREATE)
          flags = H5F_ACC_TRUNC;
        else // mode == CREATE_EXCL
          flags = H5F_ACC_EXCL;

        return hid_gc(H5Fcreate(filename.c_str(), flags, H5P_DEFAULT, fapl), H5Fclose, "Could not create file " + filename);
      }  

    case READ:  
      return hid_gc(H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT), H5Fclose, "Could not open file for read-only access; file " + filename);

    case READWRITE:  
      return hid_gc(H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT), H5Fclose, "Could not open file for read-write access; file " + filename);

    default:
      throw DALValueError("Could not open file: unknown mode argument");
  }
}

void File::flush()
{
  H5Fflush(group(), H5F_SCOPE_LOCAL);
}

VersionType File::getStoredFileVersion()
{
  Attribute<string> versionAttr(*this, versionAttrName);
  if (!versionAttr.exists())
    return VersionType();

  return VersionType(versionAttr.get());  
}

void File::setFileVersion( const VersionType &version )
{
  Attribute<string> versionAttr(*this, versionAttrName);
  if (versionAttr.exists())
    versionAttr.set(version.toString());

  // update propagation data as well
  data.fileVersion = version;
}

}

