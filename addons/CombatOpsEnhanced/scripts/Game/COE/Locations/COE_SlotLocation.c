//------------------------------------------------------------------------------------------------
class COE_SlotLocationConfig : COE_PrefabLocationConfig
{
	[Attribute(desc: "Label of prefab to spawn", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EEntityLabel))]
	protected COE_EEntityLabel m_iPrefabLabelToSpawn;

	//------------------------------------------------------------------------------------------------
	override COE_Location Create()
	{
		COE_Location location = super.Create();
		SCR_SiteSlotEntity slot = SCR_SiteSlotEntity.Cast(location.GetMainStructure());
		if (!slot)
			return null;
		
		Resource prefabToSpawn = Resource.Load(m_Faction.GetRandomPrefabByLabel(m_iPrefabLabelToSpawn));
		IEntity structure = slot.SpawnEntityInSlot(prefabToSpawn);
		location.SetMainStructure(structure);
		return location;
	}
}
