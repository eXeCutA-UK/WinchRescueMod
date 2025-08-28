\
modded class ModItemRegisterActions
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(ActionWinchConnect);
        actions.Insert(ActionWinchPull);
        actions.Insert(ActionWinchDisconnect);
    }
}
