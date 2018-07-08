#include "ReferenceCounted.hpp"

ReferenceCounted::ReferenceCounted () :
    references_ (0)
{

}

unsigned ReferenceCounted::GetReferencesCount () const
{
    return references_;
}

void ReferenceCounted::AddReference ()
{
    ++references_;
}

void ReferenceCounted::SubtractReference ()
{
    if (references_ > 0)
    {
        --references_;
    }
}
