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
#ifndef DAL_DATASET1D_H
#define DAL_DATASET1D_H

#include "Dataset.h"

namespace DAL {

/*!
 * \class Dataset1D
 *
 * Provides generic functionality for 1 dimensional HDF5 Datasets.
 * Less hassle passing positions and dimensions.
 *
 * See class Dataset for documentation.
 */
template <typename T> class Dataset1D: Dataset {
public:

  /*!
   * See Dataset::Dataset().
   */
  Dataset1D( Group &parent, const std::string &name ): Dataset<T>(parent, name) {}

  /*!
   * Destruct a Dataset1D object.
   */
  virtual ~Dataset1D() {}

  /*!
   * See Dataset::create(...).
   */
  void create( ssize_t len, ssize_t maxlen = 0,
               const std::string &filename = "", enum Endianness endianness = NATIVE );

  /*!
   * Returns 1 (by definition).
   */
  size_t ndims();

  /*!
   * Returns the length of the dataset.
   */
  ssize_t dims();

  /*!
   * Returns the maximum length to which thus dataset can grow;
   * -1 represent unbounded length.
   */
  ssize_t> maxdims();

  /*!
   * See Dataset::resize().
   */
  void resize( ssize_t newlen );

  /*!
   * Retrieves a number of data values from a 1D dataset starting at index `pos`.
   * `outbuffer` must point to a memory block large enough to hold the data values.
   * See also Dataset::get1D(...).
   *
   * pos:                       index of the first data value
   * len, outbuffer:            raw 1D array, the len of which determines the number of data values to retrieve
   *
   * Requires:
   *    pos + len <= dims()
   *    len <= size of outbuffer
   */
  void get( size_t pos, size_t len, T *outbuffer );

  /*!
   * Stores a number of data values from a 1D dataset starting at position `pos`.
   * See also Dataset::set1D(...).
   *
   * pos:                       index of the first data value
   * dim, inbuffer:             raw 1D array, the len of which determines the number of data values to store
   *
   * Requires:
   *    pos + len <= dims()
   *    len <= size of inbuffer
   */
  void set( size_t pos, size_t len, const T *inbuffer );

  /*!
   * See Dataset::getScalar().
   */
  T get( size_t pos );

  /*!
   * See Dataset::setScalar().
   */
  void set( size_t pos, T value );
};

#include "Dataset1D.tcc"

#endif

