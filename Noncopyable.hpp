#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class Noncopyable{
public:
  Noncopyable(const Noncopyable& noncopyable) = delete;
  Noncopyable& operator=(const Noncopyable& noncopyable) = delete;

  Noncopyable() = default;
};

#endif
