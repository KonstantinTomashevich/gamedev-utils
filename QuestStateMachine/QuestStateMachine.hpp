#include "QuestState.hpp"
#include <unordered_map>

class QuestStateMachine
{
public:
    QuestStateMachine ();
    virtual ~QuestStateMachine ();

    void Start (int startStateCode);
    void Update (float timeStep);
    void ResetStates ();

    QuestState *GetCurrentState () const;
    QuestState *GetState (int stateCode) const;

    void AddState (QuestState *state);
    bool RemoveState (QuestState *state);

    class Locked; /// Exception type marker.
    class StateCodesCollision; /// Exception type marker.
    class StateNotExists; /// Exception type marker.

private:
    void ThrowExceptionIfLocked () const;
    void ThrowExceptionIfNotLocked () const;

    QuestState *current_;
    std::unordered_map <int, QuestState *> states_;
};
