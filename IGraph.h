#ifndef IGRAPH_H
#define IGRAPH_H

#include "IReference.h"
#include "IVariantList.h"
#include "IVariant.h"
#include "INode.h"
#include "Noncopyable.hpp"

#include <list>
#include <string>
#include <memory>

class IGraph : private Noncopyable {
  
 public:
  typedef std::shared_ptr<IGraph> SharedPtr;
  
 IGraph(IReference::SharedPtr referencePtr, IVariantList::SharedPtr variantListPtr) : m_reference_ptr(referencePtr), m_variant_list_ptr(variantListPtr){}
  
  virtual ~IGraph() {}
  
  virtual position getStartPOsition() = 0;
  virtual position getEndPosition() = 0;
  
 protected:
  virtual void constructGraph() = 0;
  IReference::SharedPtr m_reference_ptr;
  IVariantList::SharedPtr m_variant_list_ptr;

};

#endif
