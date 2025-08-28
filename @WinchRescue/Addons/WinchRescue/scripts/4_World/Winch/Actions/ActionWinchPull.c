\
class ActionWinchPullCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(0.1);
    }
};

class ActionWinchPull : ActionContinuousBase
{
    void ActionWinchPull()
    {
        m_CallbackClass = ActionWinchPullCB;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT; // arm movement
        m_Text = "Pull Vehicle";
    }

    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {
        HandCrankWinch w;
        if (Class.CastTo(w, target.GetObject()))
        {
            return w.IsConnected();
        }
        return false;
    }

    override void OnStartServer( ActionData action_data )
    {
        HandCrankWinch w;
        if (Class.CastTo(w, action_data.m_Target.GetObject()))
        {
            w.ServerStartPull();
        }
    }

    override void OnEndServer( ActionData action_data )
    {
        HandCrankWinch w;
        if (Class.CastTo(w, action_data.m_Target.GetObject()))
        {
            w.ServerStopPull();
        }
    }
};
