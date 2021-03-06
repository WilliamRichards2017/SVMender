#ifndef REGION_H
#define REGION_H

#include <string>
#include <memory>

#include "Types.h"
#include "Noncopyable.hpp"
#include "Utility.h"

class Region : private Noncopyable{
 public:
  typedef std::shared_ptr< Region > SharedPtr;

  enum class BASED {ZERO = 0, ONE = 1};
  
  Region(const std::string& regionString, BASED based);
  Region(const std::string& referenceID, position startPosition, position endPosition, BASED based);
  ~Region();

  std::string getRegionString() const {return m_region_string;}
  std::string getReferenceID() const {return this->m_reference_id;}
  position getStartPosition() const {return this->m_start_position;}
  position getEndPosition() const {return this->m_end_position;}
  void setStartPosition(position startPosition) {this->m_start_position = startPosition;}
  void setEndPosition(position endPosition){ this->m_end_position = endPosition;}
  BASED getBased() {return m_based;}
  void setBased(BASED based);

 private:
  std::string m_region_string;
  std::string m_reference_id;
  position m_start_position;
  position m_end_position;
  BASED m_based;
    };


#endif
