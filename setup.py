#!/usr/bin/env python

"""
setup.py file from SWIG example, adjusted
"""

from distutils.core import setup, Extension
from distutils.command.build_py import build_py 
from distutils.command.install import install 
import os

# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
  numpy_include = numpy.get_include()
except AttributeError:
  numpy_include = numpy.get_numpy_include()

# Derive the path to the right HDF5 headers.
lofarsoft = os.getenv("LOFARSOFT") 

if lofarsoft is not None:
  # "use LUS"
  hdf5_include = "%s/release/include/hdf5" % (lofarsoft,)
  hdf5_lib     = "%s/release/lib/hdf5"     % (lofarsoft,)
else:
  # lhn001
  hdf5_include = "/opt/cep/hdf5/include"
  hdf5_lib     = "/opt/cep/hdf5/lib"

LDA_module = Extension('_LDA',
                           sources=['LDA.i', 'CommonAttributesFile.cc', 'BF_File.cc', 'TBB_File.cc', 'HDF5FileBase.cc', 'HDF5GroupBase.cc'],
                           include_dirs=[hdf5_include, numpy_include],
                           library_dirs=[hdf5_lib],
                           extra_compile_args=['-include','%s/hdf5.h' % hdf5_include], # force right hdf5.h instead of from /usr/include
                           libraries=['hdf5'],
                           swig_opts=['-c++','-Wall'],
                           language='c++',
                           )

dist = setup (name = 'LDA',
       version = '1.0',
       author       = "dr. Jan David Mol, ASTRON, the Netherlands",
       author_email = "mol@astron.nl",
       description = """Bindings of LDA""",
       ext_modules = [LDA_module],
       py_modules = ["LDA"],
       )

# Because while SWIG support is 'native' to python, it refuses to
# run its build process in the correct order, causing .py files
# generated by SWIG to be ignored. The code below reruns build_py
# again to fix this.
if dist.have_run.get("build_ext", False):
  if dist.have_run.get("build_py", False):
    build_py = build_py(dist) 
    build_py.ensure_finalized()
    build_py.run()

  # Reinstall if needed
  if dist.have_run.get("install", False):
    install = install(dist) 
    install.ensure_finalized()
    install.run()
