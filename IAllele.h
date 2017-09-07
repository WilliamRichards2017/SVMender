#ifndef IALLELE_H
#define IALLELE_H

#include "Types.h"
#include "Noncopyable.hpp"

#include <unordered_map>
#include <iostream>

class VCFReader;
class Sequence;
class IVariant;
class IAlignment;
class Sample;

class IAllele : private Noncopyable, public std::enable_shared_from_this< IAllele >{
 public:
  typedef std::shared_ptr< IAllele > SharedPtr;

 IAllele():
  m_id(-1){}

  virtual ~IAllele() {}
  virtual size_t getLength() = 0;
  virtual const char* getSequence() = 0;
  virtual std::string getSequenceString() = 0;
  virtual void setSequence(const std::string& sequence) = 0;
  
  virtual uint32_t getForwardCount(const std::string& sampleName, AlleleCountType alleleCountType) = 0;
  virtual uint32_t getReverseCount(const std::string& sampleName, AlleleCountType alleleCountType) = 0;
  virtual uint32_t getTotalCount(AlleleCountType alleleCountType) = 0;
  
  virtual void incrementForwardCount(std::shared_ptr< Sample > alignmentPtr, AlleleCountType alleleCountType) = 0;
  virtual void incrementReverseCount(std::shared_ptr< Sample > alignmentPtr, AlleleCountType alleleCountType) =0;
  virtual void incrementCount(bool isReverseStrand, std::shared_ptr< Sample > alignmentPtr, AlleleCountType alleleCountType) = 0;
  
  void setVariantWPtr(std::weak_ptr< IVariant > variantWPtr){m_variant_wptr = variantWPtr;}
  std::weak_ptr< IVariant > getVariantWPtr() {return m_variant_wptr;}

  inline SharedPtr getSharedPtr(){
    return shared_from_this();
  }

  virtual uint32_t getCommonPrefixSize(IAllele::SharedPtr allelePtr) = 0;
  virtual uint32_t getCommonSuffixSize(IAllele::SharedPtr allelePtr) = 0;

  void setID(int32_t id) {m_id = id;}
  int32_t getID() {return m_id;}
 
 protected:
  std::weak_ptr< IVariant > m_variant_wptr;
  int32_t m_id;

};

#endif
