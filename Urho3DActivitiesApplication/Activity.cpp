#include "Activity.hpp"
#include "ActivitiesApplication.hpp"
#include <UniversalException/UniversalException.hpp>

Activity::Activity (Urho3D::Context *context) :
        Urho3D::Object (context),
        application_ (nullptr)
{
    if (context == nullptr)
    {
        throw UniversalException <Activity::NullContextPassed> ("Activity: context pointer can not be null!");
    }
}

Activity::~Activity ()
{

}

ActivitiesApplication *Activity::GetApplication()
{
    return application_;
}

void Activity::SetApplication (ActivitiesApplication *application)
{
    if (application == nullptr)
    {
        throw UniversalException <Activity::NullApplicationPassed> ("Activity: application pointer can not be null!");
    }
    application_ = application;
}
