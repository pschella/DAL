%include dal/hdf5/types/versiontype.h

%extend DAL::VersionType {
  %pythoncode {
    def __repr__(self):
      return "%s('%s')" % (self.__class__.__name__,self.toString())

    __str__ = toString
  }
}
