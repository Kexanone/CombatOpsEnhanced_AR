//------------------------------------------------------------------------------------------------
class COE_TaskExfilConfig : COE_TaskBaseConfig
{
	//------------------------------------------------------------------------------------------------
	override void Create(COE_Location location = null)
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		IEntity base = faction.GetPlayerMainBase();
		if (!base)
			return;
		
		SpawnSFTaskPrefab(base.GetOrigin());
	}
}
