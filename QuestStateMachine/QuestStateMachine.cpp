#include "QuestStateMachine.hpp"
#include <UniversalException/UniversalException.hpp>

QuestStateMachine::QuestStateMachine () :
        current_ (nullptr),
        states_ ()
{

}

QuestStateMachine::~QuestStateMachine ()
{
    for (auto &keyValuePair : states_)
    {
        delete keyValuePair.second;
    }
}

void QuestStateMachine::Start (int startStateCode)
{
    ThrowExceptionIfLocked ();
    current_ = GetState (startStateCode);
    current_->OnEnter ();
}

void QuestStateMachine::Update (float timeStep)
{
    ThrowExceptionIfNotLocked ();
    while (current_->IsMustExit ())
    {
        current_->OnExit ();
        if (current_->GetNextStateCode () == EXIT_CODE)
        {
            ResetStates ();
            return;
        }
        
        current_ = GetState (current_->GetNextStateCode ());
        current_->OnEnter ();
    }

    current_->OnUpdate (timeStep);
}

void QuestStateMachine::ResetStates ()
{
    current_ = nullptr;
    for (auto &keyValuePair : states_)
    {
        keyValuePair.second->Reset ();
    }
}

QuestState *QuestStateMachine::GetCurrentState () const
{
    return current_;
}

QuestState *QuestStateMachine::GetState (int stateCode) const
{
    auto iterator = states_.find (stateCode);
    if (iterator == states_.end ())
    {
        throw UniversalException <QuestStateMachine::StateNotExists> (
                std::string ("QuestStateMachine: can not find state with code ") +
                        std::to_string (stateCode) + std::string ("!"));
    }

    return iterator->second;
}

void QuestStateMachine::AddState (QuestState *state)
{
    ThrowExceptionIfLocked ();
    auto iterator = states_.find (state->GetStateCode ());

    if (iterator == states_.end ())
    {
        states_[state->GetStateCode ()] = state;
    }
    else
    {
        throw UniversalException <QuestStateMachine::StateCodesCollision> (
                std::string ("QuestStateMachine: can not add state with code ") +
                        std::to_string (state->GetStateCode ()) + std::string (", because this code is already used!"));
    }
}

bool QuestStateMachine::RemoveState (QuestState *state)
{
    ThrowExceptionIfLocked ();
    auto iterator = states_.find (state->GetStateCode ());

    if (iterator != states_.end ())
    {
        if (iterator->second != state)
        {
            throw UniversalException <QuestStateMachine::StateCodesCollision> (
                    std::string ("QuestStateMachine: state with another memory address exists under state code") +
                            std::to_string (state->GetStateCode ()) + std::string ("!"));
        }
        else
        {
            states_.erase (iterator);
            return true;
        }
    }
    else
    {
        return false;
    }
}

void QuestStateMachine::ThrowExceptionIfLocked () const
{
    if (current_ != nullptr)
    {
        throw UniversalException <QuestStateMachine::Locked> (
                std::string ("QuestStateMachine: machine is locked due to execution, can not add or remove states!"));
    }
}

void QuestStateMachine::ThrowExceptionIfNotLocked () const
{
    if (current_ == nullptr)
    {
        throw UniversalException <QuestStateMachine::Locked> (
                std::string ("QuestStateMachine: machine is not locked, can not execute update!"));
    }
}
