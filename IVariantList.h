#ifndef GRAPHITE_IVARIANTLIST_H
#define GRAPHITE_IVARIANTLIST_H

#include "Region.h"
#include "Noncopyable.hpp"
#include "IVariant.h"
#include "IHeader.h"

#include <vector>
#include <algorithm>
#include <iostream>

  /* class IVariant; */
  /*
   * An iterface for variant readers. Provides a very sparse
   * framework for a variant reader. This will allow flexibility
   * for all file types and implementations.
   */
  class IVariantList : private Noncopyable
  {
  public:
    typedef std::shared_ptr< IVariantList > SharedPtr;
    IVariantList(){}
    virtual ~IVariantList() {}

    virtual void processOverlappingAlleles() = 0;
    virtual bool getNextVariant(IVariant::SharedPtr& variantPtr) = 0;
    virtual bool peekNextVariant(IVariant::SharedPtr& variantPtr) = 0;
    virtual size_t getCount() = 0;
    virtual void sort() = 0;
    virtual std::vector< IVariant::SharedPtr > getAllVariantPtrs() = 0;
  };


#endif 
