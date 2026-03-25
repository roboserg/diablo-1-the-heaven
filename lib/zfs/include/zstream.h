#pragma once

#include <memory>
#include <streambuf>
#include <ios>
#include <istream>
#include <zlib.h>// Zlib dependencies
#include "zfsconfig.h"


// Zip File System Namespace
namespace zip_file_system {

  // Base buffer class
  class ZFSEXPORT zbuffer : public std::streambuf
  {
  public:
    virtual ~zbuffer();

    virtual zbuffer * open(const char * Filename, std::streamoff Offset, std::streamoff Size) = 0;
    virtual zbuffer * close() = 0;

    bool is_open() const;

  protected:
    zbuffer();
    class impl;
    impl* pimpl;
  };

  // Buffer class for stored compression method.
  class ZFSEXPORT zbuffer_stored : public zbuffer
  {
  public:
    virtual ~zbuffer_stored() { }

    virtual zbuffer_stored * open(const char * Filename, std::streamoff Offset, std::streamoff Size);
    virtual zbuffer_stored * close();

    virtual int overflow(int c = EOF);
    virtual int underflow();
    virtual int	sync();
    virtual std::streambuf * setbuf(char * pr, int nLength);
    virtual std::streampos seekoff(std::streamoff, std::ios::seekdir, std::ios::openmode);

    //	Default Implementation is enough
    //	virtual streampos seekpos(streampos, int);
  };

  // Buffer class for deflated compression method.
  class ZFSEXPORT zbuffer_deflated : public zbuffer
  {
  public:
    zbuffer_deflated();
    virtual ~zbuffer_deflated();

    virtual zbuffer_deflated * open(const char * Filename, std::streamoff Offset, std::streamoff Size);
    virtual zbuffer_deflated * close();

    virtual int overflow(int c = EOF);
    virtual int underflow();
    virtual int sync();
    virtual std::streambuf * setbuf(char * pr, int nLength);
    virtual std::streampos seekoff(std::streamoff, std::ios::seekdir, std::ios::openmode);

    //	Default Implementation is enough
    //	virtual streampos seekpos(streampos, int);

  private:
    class impl;
    impl* pimpl;
  };

  // main istream class for reading zipped files
  class ZFSEXPORT izstream : public std::istream
  {
  public:

    izstream();
    virtual ~izstream();

    void open(const char * Filename, std::streamoff Offset, std::streamoff Size, int CompMethod);
    void close();

  private:
    class impl;
    impl* pimpl;

  };

} // namespace zip_file_system;

