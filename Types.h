
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <limits>
#include <string>
#include <vector>


  typedef uint32_t position;
  static position MAX_POSITION = std::numeric_limits< position >::max();
  enum class AlleleCountType { NinteyFivePercent = 0, NinteyPercent = 1, EightyPercent = 2, SeventyPercent = 3, LowPercent = 4, Ambiguous = 5 };
  static const std::vector< AlleleCountType > AllAlleleCountTypes = { AlleleCountType::NinteyFivePercent, AlleleCountType::NinteyPercent, AlleleCountType::EightyPercent, AlleleCountType::SeventyPercent, AlleleCountType::LowPercent, AlleleCountType::Ambiguous };
  struct AlleleCountTypeHash
  {
        template <typename T>
	std::size_t operator()(T t) const
    {
      return static_cast<std::size_t>(t);
    }
  };
  static std::string AlleleCountTypeToString(AlleleCountType alleleCountType)
  {
    switch (alleleCountType)
      {
      case AlleleCountType::NinteyFivePercent:
	return "NinteyFivePercent";
      case AlleleCountType::NinteyPercent:
	return "NinteyPercent";
      case AlleleCountType::EightyPercent:
	return "EightyPercent";
      case AlleleCountType::SeventyPercent:
	return "SeventyPercent";
      case AlleleCountType::LowPercent:
	return "LowPercent";
      case AlleleCountType::Ambiguous:
	return "Ambiguous";
      }
    return "NONE";
  }

  static std::string AlleleCountTypeToShortString(AlleleCountType alleleCountType)
  {
    switch (alleleCountType)
      {
      case AlleleCountType::NinteyFivePercent:
	return "NFP";
      case AlleleCountType::NinteyPercent:
	return "NP";
      case AlleleCountType::EightyPercent:
	return "EP";
      case AlleleCountType::SeventyPercent:
	return "SP";
      case AlleleCountType::LowPercent:
	return "LP";
      case AlleleCountType::Ambiguous:
	return "AP";
      }
    return "AP";
  }

  static AlleleCountType scoreToAlleleCountType(uint32_t score)
  {
    if (score >= 95)
      {
	return AlleleCountType::NinteyFivePercent;
      }
    else if (score >= 90)
      {
	return AlleleCountType::NinteyPercent;
      }
    else if (score >= 80)
      {
	return AlleleCountType::EightyPercent;
      }
    else if (score >= 70)
      {
	return AlleleCountType::SeventyPercent;
      }
    return AlleleCountType::LowPercent;
  }


#endif //YPES_H
