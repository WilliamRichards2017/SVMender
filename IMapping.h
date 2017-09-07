#ifndef I_MAPPING_H
#define I_MAPPING_H

#include "IAlignment.h"
#include "IAllele.h"
#include "MappingAlignmentInfo.h"
#include "Noncopyable.hpp"

#include <memory>
#include <vector>

  class IMapping : private Noncopyable
  {
  public:
    typedef std::shared_ptr< IMapping > SharedPtr;
    typedef std::weak_ptr< IMapping > WeakPtr;
    IMapping()
      {
	static uint32_t s_id = 0;
	this->m_id = s_id++;
      }
    ~IMapping() {}

    /* virtual MappingAlignmentInfo::SharedPtr getMappingAlignmentInfo(IAllele::SharedPtr allelePtr, std::shared_ptr< IAdjudicator > adjudicatorPtr) = 0; */
    virtual int getMappingScore() = 0;
    virtual IAlignment::SharedPtr getAlignmentPtr() = 0;
    virtual std::vector< IAllele::SharedPtr > getAllelePtrs() = 0;
    virtual position getPosition() = 0;
    virtual void incrementAlleleCounts() = 0;
    virtual void setMapped(bool mapped) = 0;
    virtual bool getMapped() = 0;
    virtual void addAlleleCountCallback(std::function< void () > functor) = 0;
    virtual void printMapping() = 0;

    uint32_t m_id;
  };


#endif
