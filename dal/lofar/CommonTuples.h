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
#ifndef DAL_COMMONTUPLES_H
#define DAL_COMMONTUPLES_H

#include "dal/hdf5/types/h5tuple.h"

namespace DAL {

/*!
 * Coordinate3D<T> is a 3-dimensional coordinate with fields x, y, z.
 */
template<typename T> class Coordinate3D: public TupleBase<T,3> {
public:
  T x, y, z;
};

}

#endif

