#pragma once

#include <memory>
#include "zfsconfig.h"
#include "ziphdr.h"		// Zip file header
#include "zstream.h"	// Zip Stream



// Zip File System Namespace
namespace zip_file_system {

  class ZFSEXPORT filesystem;

  // Input Zip File class
  class ZFSEXPORT izfstream : public izstream
  {
  public:
    izfstream(const filesystem * pFS = 0);
    izfstream(const char * FilePath, const filesystem * pFS = 0);

    virtual ~izfstream();

    void open(const char * FilePath);
    void close();

    bool is_open() const;

    bool Zipped() const;
    const std::string & FilePath() const;
    const std::string & FullFilePath() const;

  private:
    friend class filesystem;
    class impl;
    impl* pimpl;
  };

  ZFSEXPORT std::ostream & operator << (std::ostream & Out, const filesystem & FS);
  
  // Zip File System central class
  class ZFSEXPORT filesystem  
  {
  public:
    filesystem(const char * BasePath = "", const char * FileExt = "zip", bool DefaultFS = true); 
    ~filesystem();

    void MakeDefault() const;
    void Open(izfstream & File, const char * Filename) const;

    friend std::ostream & operator << (std::ostream & Out, const filesystem & FS);

  private:
    class impl;
    impl* pimpl;
  };

}	// namespace zip_file_system

