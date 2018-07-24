#include <Urho3D/Engine/Application.h>
#include <Urho3D/AngelScript/ScriptFile.h>

#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/Core/Main.h>
#include <Urho3D/Engine/Engine.h>

#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <Urho3D/DebugNew.h>
#include "UIResizer.hpp"

class EditorLauncher : public Urho3D::Application
{
URHO3D_OBJECT (EditorLauncher, Application)
public:
    explicit EditorLauncher (Urho3D::Context* context);
    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();

private:
    void HandleScriptReloadStarted (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleScriptReloadFinished (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleScriptReloadFailed (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    Urho3D::String scriptFileName_;
    Urho3D::SharedPtr <Urho3D::ScriptFile> scriptFile_;
};

URHO3D_DEFINE_APPLICATION_MAIN (EditorLauncher)
EditorLauncher::EditorLauncher (Urho3D::Context* context) : Urho3D::Application (context)
{

}

void EditorLauncher::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Editor.log";
    engineParameters_ ["WindowTitle"] = "Urho3D Editor";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
    engineParameters_ ["ResourcePaths"] = "Data;CoreData;Urho3DEditorData";
}

void EditorLauncher::Start ()
{
    UIResizer::RegisterObject (context_);
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);

    // Hold a shared pointer to the script file to make sure it is not unloaded during runtime
    scriptFile_ = GetSubsystem <Urho3D::ResourceCache> ()->GetResource <Urho3D::ScriptFile> ("Scripts/Editor.as");

    // If script loading is successful, proceed to main loop
    if (scriptFile_ && scriptFile_->Execute ("void Start ()"))
    {
        // Subscribe to script's reload event to allow live-reload of the application
        SubscribeToEvent (scriptFile_, Urho3D::E_RELOADSTARTED, URHO3D_HANDLER (EditorLauncher, HandleScriptReloadStarted));
        SubscribeToEvent (scriptFile_, Urho3D::E_RELOADFINISHED, URHO3D_HANDLER (EditorLauncher, HandleScriptReloadFinished));
        SubscribeToEvent (scriptFile_, Urho3D::E_RELOADFAILED, URHO3D_HANDLER (EditorLauncher, HandleScriptReloadFailed));
        return;
    }
    else
    {
        ErrorExit ("Error loading editor scripts!");
    }
}

void EditorLauncher::Stop ()
{
    if (scriptFile_)
    {
        // Execute the optional stop function
        if (scriptFile_->GetFunction ("void Stop ()"))
        {
            scriptFile_->Execute ("void Stop ()");
        }
    }
}

void EditorLauncher::HandleScriptReloadStarted (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (scriptFile_->GetFunction("void Stop ()"))
    {
        scriptFile_->Execute("void Stop ()");
    }
}

void EditorLauncher::HandleScriptReloadFinished (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    // Restart the script application after reload
    if (!scriptFile_->Execute ("void Start ()"))
    {
        scriptFile_.Reset ();
        ErrorExit ();
    }
}

void EditorLauncher::HandleScriptReloadFailed (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    scriptFile_.Reset ();
    ErrorExit ();
}
