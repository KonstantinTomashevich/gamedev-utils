#pragma once

template <class T>
class SharedPointer
{
public:
    SharedPointer () :
        trackingObject_ (nullptr)
    {

    }

    SharedPointer (T *trackingObject) :
        trackingObject_ (trackingObject)
    {
        AddReference ();
    }

    SharedPointer (const SharedPointer <T> &anotherSharedPointer) :
        trackingObject_ (anotherSharedPointer.Get ())
    {
        AddReference ();
    }

    virtual ~SharedPointer ()
    {
        SubtractReference ();
    }

    void Set (T *trackingObject)
    {
        SubtractReference ();
        trackingObject_ = trackingObject;
        AddReference ();
    }

    T *Get () const
    {
        return trackingObject_;
    }

    T *operator -> () const
    {
        return Get ();
    }

    void Detach ()
    {
        trackingObject_ = nullptr;
    }

private:
    T *trackingObject_;

    void AddReference ()
    {
        if (trackingObject_)
        {
            trackingObject_->AddReference ();
        }
    }

    void SubtractReference ()
    {
        if (trackingObject_ != nullptr)
        {
            trackingObject_->SubtractReference ();
            if (trackingObject_->GetReferencesCount () == 0)
            {
                delete trackingObject_;
            }
        }
    }
};
