#pragma once 

#include <string>
#include <io.h>
#include "zfsconfig.h"


namespace io_facilities {

  // Class for searching files and directories
  // (!!! not compliant with C++ std::iterator and is thus meant for specific use !!!)
  class ZFSEXPORT search_iterator
  {
  public:
    search_iterator();
    search_iterator(const char * FileSpec);
    ~search_iterator();

    operator bool () const;
    search_iterator & operator ++ ();
    search_iterator & begin(const char * FileSpec);
    search_iterator & next();
    bool end() const;
    std::string Name() const;

  protected:
    bool		m_Valid;
    intptr_t	m_hFiles;
    _finddata_t	m_FindData;
  };

} // namespace io_facilities


