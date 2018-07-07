#pragma once
#include <string>

class QuestState
{
public:
    virtual ~QuestState () = default;
    virtual void OnEnter ();
    virtual void OnUpdate (float timeStep) = 0;
    virtual void OnExit ();

    int GetStateCode () const;
    const std::string &GetInfo () const;
    bool IsMustExit () const;
    int GetNextStateCode () const;

protected:
    QuestState (int stateCode, const std::string &info);
    void SetMustExit (bool mustExit, int nextStateCode);

private:
    int stateCode_;
    std::string info_;
    bool mustExit_;
    int nextStateCode_;
};
