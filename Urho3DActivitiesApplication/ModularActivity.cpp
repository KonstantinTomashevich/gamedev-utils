#include "ModularActivity.hpp"

ModularActivity::ModularActivity (Urho3D::Context *context) : Activity (context)
{

}

ModularActivity::~ModularActivity ()
{
    Stop ();
}

void ModularActivity::Start ()
{

}

void ModularActivity::Update (float timeStep)
{

}

void ModularActivity::Stop ()
{
    modules_.Clear ();
}
