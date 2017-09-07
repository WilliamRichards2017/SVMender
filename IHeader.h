#ifndef IHEADER_H
#define IHEADER_H

#include <memory>
#include <vector>
#include <string>

#include "Noncopyable.hpp"

class Sample;
class IHeader : private Noncopyable{
 public:
  typedef std::shared_ptr< IHeader > SharedPtr;
  IHeader() {}
  virtual ~IHeader() {}
  
  virtual std::string getHeader() = 0;
  virtual std::vector< std::string > getSampleNames() = 0;
  virtual int32_t getColumnPosition(const std::string& columnTitle) = 0;
  virtual std::vector< std::string> getColumnNames() = 0;
  virtual void setColumn(const std::string& column) = 0;
  virtual bool isActiveSampleColumnname(const std::string& sampleName) = 0;
  virtual bool isSampleColumnName(const std::string& headerName) = 0;
};

#endif
