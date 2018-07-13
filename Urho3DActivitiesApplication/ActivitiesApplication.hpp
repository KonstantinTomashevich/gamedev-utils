#pragma once
#include "Activity.hpp"
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Container/Vector.h>

//@ASBindGen Class ObjectType=Ref
class ActivitiesApplication : public Urho3D::Application
{
URHO3D_OBJECT (ActivitiesApplication, Application)
public:
    explicit ActivitiesApplication (Urho3D::Context *context);
    virtual ~ActivitiesApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();

    //@ASBindGen Function AddRef_arg0
    void SetupActivityNextFrame (Activity *activity);
    //@ASBindGen Function AddRef_arg0
    void StopActivityNextFrame (Activity *activity);
    //@ASBindGen Function OverrideName=get_activitiesCount
    unsigned GetActivitiesCount () const;
    //@ASBindGen Function AddRef_arg-1
    Activity *GetActivityByIndex (unsigned int index);
    //@ASBindGen Function
    void StopAllActivitiesNextFrame ();

    // Exception classes.
    class IndexOutOfBounds;
    class NullActivityPassed;

private:
    void UpdateActivities (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    Urho3D::Vector <Urho3D::SharedPtr <Activity> > currentActivities_;
    Urho3D::Vector <Urho3D::SharedPtr <Activity> > activitiesToSetup_;
    Urho3D::Vector <Urho3D::SharedPtr <Activity> > activitiesToStop_;
};
