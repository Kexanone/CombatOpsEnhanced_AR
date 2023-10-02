//------------------------------------------------------------------------------------------------
class COE_TaskKillConfig : COE_TaskBaseConfig
{
	[Attribute(desc: "Label of target prefab", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EPrefabLabel))]
	protected COE_EPrefabLabel m_iTargetPrefabLabel;
	
	//------------------------------------------------------------------------------------------------
	override void Create(COE_LocationBase location = null)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetTargetPrefabName()
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return string.Empty;
		
		COE_Faction faction = factionManager.GetEnemyFaction();
		if (!faction)
			return string.Empty;
		
		return faction.GetRandomPrefabByLabel(m_iTargetPrefabLabel);
	}
}
