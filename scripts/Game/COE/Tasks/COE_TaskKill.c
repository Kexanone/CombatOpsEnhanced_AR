//------------------------------------------------------------------------------------------------
class COE_TaskKillConfig : COE_TaskBaseConfig
{
	[Attribute(desc: "Label of target prefab", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EEntityLabel))]
	protected COE_EEntityLabel m_iTargetPrefabLabel;
	
	//------------------------------------------------------------------------------------------------
	override void Create(COE_Location location = null)
	{
		IEntity main = location.GetMainStructure();
		COE_AISlotsComponent slotsComponent = COE_AISlotsComponent.Cast(main.FindComponent(COE_AISlotsComponent));
		array<COE_AISlotConfig> slots = slotsComponent.GetSlotConfigs(COE_EEntityLabel.HVT, true);
		
		if (slots.IsEmpty())
		{
			COE_Utils.PrintError(string.Format("Location %1 has no HVT slots", location), "COE_TaskKillConfig");
			return;
		};
		
		Math.Randomize(-1);
		IEntity hvt = slots[Math.RandomInt(0, slots.Count())].Spawn();
		SpawnSFTaskPrefab(location.GetCenter(), hvt);
	}
}
