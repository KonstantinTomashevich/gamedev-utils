#include "QuestState.hpp"

void QuestState::OnEnter ()
{
    mustExit_ = false;
}

void QuestState::OnExit ()
{
    Reset ();
}

void QuestState::Reset ()
{
    mustExit_ = false;
}

int QuestState::GetStateCode () const
{
    return stateCode_;
}

const std::string &QuestState::GetInfo () const
{
    return info_;
}

bool QuestState::IsMustExit () const
{
    return mustExit_;
}

int QuestState::GetNextStateCode () const
{
    return nextStateCode_;
}

QuestState::QuestState (int stateCode, const std::string &info) :
    stateCode_ (stateCode),
    info_ (info),
    mustExit_ (false),
    nextStateCode_ (0)
{

}

void QuestState::SetMustExit (bool mustExit, int nextStateCode)
{
    mustExit_ = mustExit;
    nextStateCode_ = nextStateCode;
}
