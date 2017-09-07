#ifndef ALLELEMETADATA_H
#define ALLELEMETADATA_H

#include "Noncopyable.hpp"
#include <memory>

class AlleleMetaData : private Noncopyable{
 public: 
  typedef std::shared_ptr< AlleleMetaData > SharedPtr;
 AlleleMetaData(uint16_t paddingPrefix, uint16_t paddingSuffix) :
  m_padding_prefix(paddingPrefix), m_padding_suffix(paddingSuffix){
  }
  ~AlleleMetaData() {}

  uint16_t getPaddingPrefix() {return this->m_padding_prefix;}
  uint16_t getPaddingSuffix() {return this->m_padding_suffix;}

 private:
  uint16_t m_padding_prefix;
  uint16_t m_padding_suffix;
};

#endif
