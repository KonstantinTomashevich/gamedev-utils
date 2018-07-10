#include "UIResizer.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/ScrollView.h>

UIResizer::UIResizer (Urho3D::Context *context) : LogicComponent (context),
    continuousUpdate_ (false),
    lastScreenSize_ (0, 0),
    scanRootElement_ ("UIRoot")
{
    SubscribeToEvent (EVENT_UI_RESIZER_RECALCULATE_UI_REQUEST, URHO3D_HANDLER (UIResizer, HandleRecalculateUIRequest));
}

UIResizer::~UIResizer ()
{
    UnsubscribeFromAllEvents ();
}

void UIResizer::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <UIResizer> ("Logic");
    URHO3D_ACCESSOR_ATTRIBUTE ("Continuous Update", IsContinuousUpdate, SetContinuousUpdate, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Scan Root Element", GetScanRootElement, SetScanRootElement, Urho3D::String, "UIRoot", Urho3D::AM_DEFAULT);
}

void UIResizer::Update (float timeStep)
{
    Urho3D::Graphics *graphics = context_->GetSubsystem <Urho3D::Graphics> ();
    if (continuousUpdate_ || lastScreenSize_.x_ != graphics->GetWidth () || lastScreenSize_.y_ != graphics->GetHeight ())
    {
        RecalculateUI ();

    }
}

void UIResizer::RecalculateUI ()
{
    Urho3D::Graphics *graphics = context_->GetSubsystem <Urho3D::Graphics> ();
    lastScreenSize_.x_ = graphics->GetWidth ();
    lastScreenSize_.y_ = graphics->GetHeight ();

    Urho3D::UI *ui = context_->GetSubsystem <Urho3D::UI> ();
    Urho3D::UIElement *rootElement = ui->GetRoot ();

    if (scanRootElement_ != "UIRoot")
    {
        rootElement = rootElement->GetChild (scanRootElement_, true);
    }

    if (rootElement != nullptr && rootElement->HasTag ("UIResizer"))
    {
        Urho3D::HashMap <Urho3D::StringHash, int> dependenciesValues;
        dependenciesValues ["SW"] = lastScreenSize_.x_;
        dependenciesValues ["SH"] = lastScreenSize_.y_;

        if (rootElement != ui->GetRoot ())
        {
            ProcessElement (rootElement, dependenciesValues);
        }

        ProcessElementChildren (rootElement, dependenciesValues);
        if (rootElement != ui->GetRoot ())
        {
            ProcessElementLayout (rootElement, dependenciesValues);
        }
    }
}

bool UIResizer::IsContinuousUpdate () const
{
    return continuousUpdate_;
}

void UIResizer::SetContinuousUpdate (bool continuousUpdate)
{
    continuousUpdate_ = continuousUpdate;
}

const Urho3D::IntVector2 &UIResizer::GetLastScreenSize () const
{
    return lastScreenSize_;
}

const Urho3D::String &UIResizer::GetScanRootElement () const
{
    return scanRootElement_;
}

void UIResizer::SetScanRootElement (const Urho3D::String &scanRootElement)
{
    scanRootElement_ = scanRootElement;
}

void UIResizer::ProcessElement (Urho3D::UIElement *element, Urho3D::HashMap <Urho3D::StringHash, int> &dependenciesValues) const
{
    float vWidth = element->GetVar ("VWidth").GetFloat ();
    float vHeight = element->GetVar ("VHeight").GetFloat ();
    float vX = element->GetVar ("VX").GetFloat ();
    float vY = element->GetVar ("VY").GetFloat ();

    element->SetSize (Urho3D::FloorToInt (vWidth * dependenciesValues [element->GetVar ("WDep").GetString ()]),
                      Urho3D::FloorToInt (vHeight * dependenciesValues [element->GetVar ("HDep").GetString ()]));

    element->SetPosition (Urho3D::FloorToInt (vX * dependenciesValues [element->GetVar ("XDep").GetString ()]),
                          Urho3D::FloorToInt (vY * dependenciesValues [element->GetVar ("YDep").GetString ()]));

    Urho3D::Text *asText = dynamic_cast <Urho3D::Text *> (element);
    Urho3D::LineEdit *asLineEdit = dynamic_cast <Urho3D::LineEdit *> (element);

    if (asText != nullptr || asLineEdit != nullptr)
    {
        float vTextSize = element->GetVar ("VTextSize").GetFloat ();
        if (asText != nullptr)
        {
            asText->SetFontSize (vTextSize * dependenciesValues [element->GetVar ("TSDep").GetString ()]);
        }
        else
        {
            asLineEdit->GetTextElement ()->SetFontSize (
                    vTextSize * dependenciesValues [element->GetVar ("TSDep").GetString ()]);
        }
    }
}

void UIResizer::ProcessElementChildren (Urho3D::UIElement *element,
                                        Urho3D::HashMap <Urho3D::StringHash, int> &dependenciesValues) const
{
    Urho3D::PODVector <Urho3D::UIElement *> children;
    element->GetChildrenWithTag (children, "UIResizer");

    int previousPW = dependenciesValues ["PW"];
    int previousPH = dependenciesValues ["PH"];

    for (Urho3D::UIElement *&child : children)
    {
        dependenciesValues ["PW"] = element->GetWidth ();
        dependenciesValues ["PH"] = element->GetHeight ();

        ProcessElement (child, dependenciesValues);
        ProcessElementChildren (child, dependenciesValues);
        ProcessElementLayout (child, dependenciesValues);
    }

    if (dynamic_cast <Urho3D::ScrollView *> (element) != nullptr)
    {
        Urho3D::UIElement *content = dynamic_cast <Urho3D::ScrollView *> (element)->GetContentElement ();
        if (content != nullptr && content->HasTag ("UIResizer"))
        {
            dependenciesValues ["PW"] = element->GetWidth ();
            dependenciesValues ["PH"] = element->GetHeight ();

            ProcessElement (content, dependenciesValues);
            ProcessElementChildren (content, dependenciesValues);
            ProcessElementLayout (content, dependenciesValues);
        }
    }

    dependenciesValues ["PW"] = previousPW;
    dependenciesValues ["PH"] = previousPH;
}

void UIResizer::ProcessElementLayout (Urho3D::UIElement *element,
                                      Urho3D::HashMap <Urho3D::StringHash, int> &dependenciesValues) const
{
    int wDep = dependenciesValues [element->GetVar ("WDep").GetString ()];
    int hDep = dependenciesValues [element->GetVar ("WDep").GetString ()];

    Urho3D::Variant layoutTypeVar = element->GetVar ("LayoutType");
    if (layoutTypeVar.GetString () == "V" || layoutTypeVar.GetString () == "H")
    {
        bool vertical = (layoutTypeVar.GetString () == "V");
        bool updatePrimarySize = element->GetVar ("UpdatePS").GetBool ();
        bool updateSecondarySize = element->GetVar ("UpdateSS").GetBool ();

        int spacing = element->GetLayoutSpacing () * (vertical ? hDep : wDep) / 1000;
        int currentX = element->GetLayoutBorder ().left_ * wDep / 1000;
        int currentY = element->GetLayoutBorder ().top_ * hDep / 1000;

        Urho3D::PODVector <Urho3D::UIElement *> children;
        element->GetChildrenWithTag (children, "UIResizer");

        int maxSecondary = 0;
        for (Urho3D::UIElement *&child : children)
        {
            child->SetPosition (currentX, currentY);
            (vertical ? currentY : currentX) += (vertical ? child->GetHeight () : child->GetWidth ()) + spacing;
            maxSecondary = std::max (maxSecondary, vertical ? child->GetWidth () : child->GetHeight ());
        }

        if (updatePrimarySize)
        {
            if (vertical)
            {
                element->SetHeight (currentY - spacing + element->GetLayoutBorder ().bottom_ * hDep / 1000);
            }
            else
            {
                element->SetWidth (currentX - spacing + element->GetLayoutBorder ().right_ * wDep / 1000);
            }
        }

        if (updateSecondarySize)
        {
            if (vertical)
            {
                element->SetWidth (currentX + maxSecondary + element->GetLayoutBorder ().right_ * wDep / 1000);
            }
            else
            {
                element->SetHeight (currentY + maxSecondary + element->GetLayoutBorder ().top_ * hDep / 1000);
            }
        }
    }
}

void UIResizer::HandleRecalculateUIRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    RecalculateUI ();
}

