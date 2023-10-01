//------------------------------------------------------------------------------------------------
class COE_GameModeClass : SCR_BaseGameModeClass
{
};

//------------------------------------------------------------------------------------------------
class COE_GameMode : SCR_BaseGameMode
{
	//------------------------------------------------------------------------------------------------
	void COE_GameMode(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		// Read mission header
		COE_MissionHeader header = COE_MissionHeader.Cast(GetGame().GetMissionHeader());
		if (header)
		{
		};
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnGameModeStart()
	{
		super.OnGameModeStart();
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		COE_EntitySpawner carrierSpawner = COE_EntitySpawner.Cast(GetGame().GetWorld().FindEntityByName("CarrierSpawner"));
		if (carrierSpawner)
			carrierSpawner.Spawn();
	}
};
