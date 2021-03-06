#ifndef IVARIANT_H
#define IVARIANT_H

#include "Types.h"
#include "IAllele.h"
#include "Noncopyable.hpp"
#include "Region.h"
#include "IHeader.h"

#include <vector>
#include <memory>
#include <unordered_set>


  class IAllele;
  class IAlignment;
  class Sample;
  class IVariant : private Noncopyable, public std::enable_shared_from_this< IVariant >
    {
    public:
      typedef std::shared_ptr<IVariant> SharedPtr;
      typedef std::shared_ptr<IVariant> WeakPtr;
      IVariant()
	{
	}
      virtual ~IVariant() {}

      virtual position getPosition() = 0;
      virtual std::string getChrom() const = 0;
      virtual IAllele::SharedPtr getRefAllelePtr() = 0;
      virtual std::vector< IAllele::SharedPtr > getAltAllelePtrs() = 0;
      virtual void processOverlappingAlleles() = 0; // set all allele variantwptrs to be this
      virtual uint32_t getAllelePrefixOverlapMaxCount(IAllele::SharedPtr allelePtr) = 0;
      virtual uint32_t getAlleleSuffixOverlapMaxCount(IAllele::SharedPtr allelePtr) = 0;
      virtual std::string getVariantLine(IHeader::SharedPtr headerPtr) = 0;
      virtual bool shouldSkip() = 0;
      virtual void setSkip(bool) = 0;
      virtual std::vector< Region::SharedPtr > getRegions() = 0;
      virtual bool doesOverlap(IVariant::SharedPtr variantPtr) = 0;
      virtual uint32_t getReferenceSize() = 0;
      virtual void addRegion(Region::SharedPtr regionPtr) = 0;
      virtual uint32_t getVariantSize() = 0;
      virtual bool isStructuralVariant() = 0;
    };

#endif
