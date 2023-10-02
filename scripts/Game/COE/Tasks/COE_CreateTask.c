//------------------------------------------------------------------------------------------------
/*
class COE_CreateTask
{
	//------------------------------------------------------------------------------------------------
	static void ClearArea()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	static void DestroyTarget()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	static void Kill()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	static void DeliverIntel(COE_TaskDeliverIntelConfig config, COE_LocationBase location)
	{
		IEntity mainStructure = location.GetMainStructure();
		if (!mainStructure)
		{
			COE_Utils.PrintError(string.Format("%1 has no main struture", location), "COE_TaskDeliverIntel");
			return;
		};
		
		COE_IntelSlotsComponent slotComponent = COE_IntelSlotsComponent.Cast(mainStructure.FindComponent(COE_IntelSlotsComponent));
		if (!slotComponent)
		{
			COE_Utils.PrintError(string.Format("%1 has no COE_IntelSlotsComponent", mainStructure), "COE_TaskDeliverIntel");
			return;
		};
		
		IEntity intel = slotComponent.SpawnInRandomSlot(config.GetIntelPrefabName());
		IEntity layer = COE_GameTools.SpawnSFTaskPrefab(config.GetSFTaskPrefabName(), mainStructure.GetOrigin(), intel);
	}
	
	//------------------------------------------------------------------------------------------------
	static void Exfil(COE_TaskExfilConfig config, COE_LocationBase location)
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
		
		COE_GameTools.SpawnSFTaskPrefab(config.GetSFTaskPrefabName(), base.GetOrigin());
	}
}
*/