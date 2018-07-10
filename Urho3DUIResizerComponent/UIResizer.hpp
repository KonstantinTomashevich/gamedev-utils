#pragma once
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/UI/UIElement.h>

const Urho3D::StringHash EVENT_UI_RESIZER_RECALCULATE_UI_REQUEST ("UIResizerRecalculateUIRequest");
class UIResizer : public Urho3D::LogicComponent
{
URHO3D_OBJECT (UIResizer, LogicComponent)

public:
    explicit UIResizer (Urho3D::Context *context);
    virtual ~UIResizer ();
    static void RegisterObject (Urho3D::Context *context);

    virtual void Update (float timeStep);
    void RecalculateUI ();

    bool IsContinuousUpdate () const;
    void SetContinuousUpdate (bool continuousUpdate);

    const Urho3D::IntVector2 &GetLastScreenSize () const;
    const Urho3D::String &GetScanRootElement () const;
    void SetScanRootElement (const Urho3D::String &scanRootElement);

private:
    void ProcessElement (Urho3D::UIElement *element, Urho3D::HashMap <Urho3D::StringHash, int> &dependenciesValues) const;
    void ProcessElementChildren (Urho3D::UIElement *element,
                                     Urho3D::HashMap <Urho3D::StringHash, int> &dependenciesValues) const;

    void ProcessElementLayout (Urho3D::UIElement *element,
                                   Urho3D::HashMap <Urho3D::StringHash, int> &dependenciesValues) const;
    void HandleRecalculateUIRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    bool continuousUpdate_;
    Urho3D::IntVector2 lastScreenSize_;
    Urho3D::String scanRootElement_;
};
