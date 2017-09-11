#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <iostream>
#include <string>
#include <memory>

#include "Noncopyable.hpp"

  class Sample : private Noncopyable
  {
  public:
    typedef std::shared_ptr< Sample > SharedPtr;
    Sample(const std::string& sampleName, const std::string& readGroup, const std::string& samplePath);
    Sample() = delete;
    ~Sample();

    std::string getName();
    std::string getReadgroup();
    std::string getPath();

  private:
    std::string m_sample_name;
    std::string m_sample_readgroup;
    std::string m_sample_path;
  };

#endif
