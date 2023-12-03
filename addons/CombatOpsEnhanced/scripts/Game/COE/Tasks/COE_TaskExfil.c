//------------------------------------------------------------------------------------------------
class COE_TaskExfilConfig : COE_TaskBaseConfig
{
	//------------------------------------------------------------------------------------------------
	override void Create(COE_Location location = null)
	{
		IEntity base = m_PlayerFaction.GetPlayerMainBase();
		if (!base)
			return;
		
		SpawnSFTaskPrefab(base.GetOrigin());
	}
}
