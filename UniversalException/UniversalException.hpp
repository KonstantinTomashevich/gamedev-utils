#pragma once
#include <string>

class AnyUniversalException
{
public:
    explicit AnyUniversalException (const std::string &exception) noexcept : exception_ (exception) {}
    AnyUniversalException (const AnyUniversalException &another) noexcept : exception_ (another.exception_) {}
    virtual ~AnyUniversalException () {}
    const std::string &GetException () const noexcept { return exception_; }

private:
    std::string exception_;
};

template <class T>
class UniversalException : public AnyUniversalException
{
public:
    explicit UniversalException (const std::string &exception) noexcept : AnyUniversalException (exception) {}
    UniversalException (const UniversalException <T> &another) noexcept : AnyUniversalException (another) {}
    virtual ~UniversalException () {}
};
