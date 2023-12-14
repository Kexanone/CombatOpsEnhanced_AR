//------------------------------------------------------------------------------------------------
class COE_LabelledEntitySlotClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class COE_LabelledEntitySlot : GenericEntity
{
	[Attribute(desc: "Label of the entity to be spawned", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EEntityLabel))]
	protected COE_EEntityLabel m_iEntityLabel;
	
	[Attribute(defvalue: "true", desc: "True if it is the entity of the player faction; False if it is of the enemy faction")]
	protected bool m_bIsPlayerFaction;
	
	//------------------------------------------------------------------------------------------------
	void COE_LabelledEntitySlot(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction;
		if (m_bIsPlayerFaction)
		{
			faction = factionManager.GetPlayerFaction();
		}
		else
		{
			faction = factionManager.GetEnemyFaction();
		};
		
		vector transform[4];
		GetWorldTransform(transform);
		COE_GameTools.SpawnStructurePrefab(faction.GetRandomPrefabByLabel(m_iEntityLabel), transform);
	}
}