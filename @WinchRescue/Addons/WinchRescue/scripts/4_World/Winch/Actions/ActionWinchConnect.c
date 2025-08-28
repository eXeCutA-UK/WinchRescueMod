\
class ActionWinchConnect : ActionInteractBase
{
    void ActionWinchConnect()
    {
        m_Text = "Connect to Vehicle";
    }

    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {
        HandCrankWinch w;
        if (Class.CastTo(w, target.GetObject()))
        {
            return !w.IsConnected();
        }
        return false;
    }

    override void OnExecuteServer( ActionData action_data )
    {
        HandCrankWinch w;
        if (Class.CastTo(w, action_data.m_Target.GetObject()))
        {
            w.ServerConnectToNearestVehicle(action_data.m_Player);
        }
    }
};
