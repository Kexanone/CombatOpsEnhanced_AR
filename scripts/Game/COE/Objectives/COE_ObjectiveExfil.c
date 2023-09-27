//------------------------------------------------------------------------------------------------
class COE_ObjectiveExfil : COE_ObjectiveBase
{
	protected IEntity m_pTarget;
	protected SCR_DestructionMultiPhaseComponent m_pTargetDestructionComponent;
	
	//------------------------------------------------------------------------------------------------
	void COE_ObjectiveExfil()
	{
		COE_EntitySpawner baseSpawner = COE_EntitySpawner.Cast(GetGame().GetWorld().FindEntityByName("CarrierSpawner"));
		if (!baseSpawner)
			return;
		
		array<IEntity> baseEntities = baseSpawner.GetSpawnedEntities();
		if (baseEntities.IsEmpty() || !baseEntities[0])
			return;
		
		SetTaskLayer(COE_GameTools.SpawnSFTaskPrefab("{49B2DF0651711783}Prefabs/Compositions/Tasks/Common/COE_TaskExfil.et", baseEntities[0].GetOrigin()));
	}
}
