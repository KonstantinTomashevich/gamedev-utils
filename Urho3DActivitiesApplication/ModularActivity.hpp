#pragma once
#include "Activity.hpp"
#include <SharedPointer/SharedPointer.hpp>
#include <Urho3D/Container/HashMap.h>
#include <UniversalException/UniversalException.hpp>

class ModularActivity : public Activity
{
URHO3D_OBJECT (ModularActivity, Activity)
public:
    explicit ModularActivity (Urho3D::Context *context);
    virtual ~ModularActivity ();

    /// Empty implementation.
    virtual void Start ();
    /// Empty implementation.
    virtual void Update (float timeStep);

    /// Clears all modules.
    virtual void Stop ();

    template <class T> bool RegisterModule (T *module)
    {
        Urho3D::StringHash type = T::GetTypeInfoStatic ()->GetType ();
        if (modules_.Contains (type))
        {
            return false;
        }
        else
        {
            modules_ [type] = module;
            return true;
        }
    }

    template <class T> bool DisposeModule ()
    {
        return modules_.Erase (T::GetTypeInfoStatic ()->GetType ());
    }

    template <class T> T *RequestModule () const
    {
        Urho3D::StringHash type = T::GetTypeInfoStatic ()->GetType ();
        Urho3D::SharedPtr <Urho3D::Object> module;
        modules_.TryGetValue (type, module);

        if (module.Null ())
        {
            throw UniversalException <ModularActivity::ModuleNotFound> (("ModularApplication: module " +
                T::GetTypeInfoStatic ()->GetTypeName () + " isn't exists!").CString ());
        }
        else
        {
            return dynamic_cast <T *> (module.Get ());
        }
    }

    // Exception classes.
    class ModuleNotFound;

private:
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::SharedPtr <Urho3D::Object> > modules_;
};
