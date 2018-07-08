#pragma once

class ReferenceCounted
{
public:
    ReferenceCounted ();
    virtual ~ReferenceCounted () = default;

    unsigned int GetReferencesCount () const;
    void AddReference ();
    void SubtractReference ();

private:
    unsigned int references_;
};
