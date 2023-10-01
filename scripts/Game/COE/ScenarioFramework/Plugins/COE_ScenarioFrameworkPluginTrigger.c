//------------------------------------------------------------------------------------------------
// Same as SCR_ScenarioFrameworkPluginTrigger, but automatically assigns it to the player faction
[BaseContainerProps(), SCR_ContainerActionTitle()]
class COE_ScenarioFrameworkPluginTrigger: SCR_ScenarioFrameworkPluginTrigger
{
	//------------------------------------------------------------------------------------------------
	override void Init(SCR_ScenarioFrameworkLayerBase object)
	{
		if (!object)
			return;
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		m_sActivatedByThisFaction = factionManager.GetPlayerFaction().GetFactionKey();

		super.Init(object);
	}
}
