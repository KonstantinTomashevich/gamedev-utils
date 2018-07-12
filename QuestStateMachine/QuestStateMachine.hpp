#include "QuestState.hpp"
#include <unordered_map>
#include <SharedPointer/SharedPointer.hpp>

class QuestStateMachine
{
public:
    static const int EXIT_CODE = -1;
    
    QuestStateMachine ();
    virtual ~QuestStateMachine () = default;

    void Start (int startStateCode);
    void Resume (int stateCode);

    void Update (float timeStep);
    void ResetStates ();

    QuestState *GetCurrentState () const;
    QuestState *GetState (int stateCode) const;

    void AddState (QuestState *state);
    bool RemoveState (QuestState *state);
    const std::unordered_map <int, SharedPointer <QuestState> > &GetStates () const;

    class Locked; /// Exception type marker.
    class StateCodesCollision; /// Exception type marker.
    class StateNotExists; /// Exception type marker.

private:
    void ThrowExceptionIfLocked () const;
    void ThrowExceptionIfNotLocked () const;

    QuestState *current_;
    std::unordered_map <int, SharedPointer <QuestState> > states_;
};
