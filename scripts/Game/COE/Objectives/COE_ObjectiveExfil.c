//------------------------------------------------------------------------------------------------
class COE_ObjectiveExfil : COE_ObjectiveBase
{
	protected IEntity m_pTarget;
	protected SCR_DestructionMultiPhaseComponent m_pTargetDestructionComponent;
	
	//------------------------------------------------------------------------------------------------
	void COE_ObjectiveExfil()
	{
		COE_EntitySpawner baseSpawner = COE_EntitySpawner.Cast(GetGame().GetWorld().FindEntityByName("BaseSpawner"));
		if (!baseSpawner)
			return;
		
		array<IEntity> baseEntities = baseSpawner.GetSpawnedEntities();
		if (baseEntities.IsEmpty() || !baseEntities[0])
			return;
		
		EntitySpawnParams params = new EntitySpawnParams;
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = baseEntities[0].GetOrigin();
		IEntity area = GetGame().SpawnEntityPrefab(Resource.Load("{46AE02182546D90D}Prefabs/Compositions/Tasks/COE_TaskExfil.et"), null, params);
		SCR_ScenarioFrameworkArea areaComponent = SCR_ScenarioFrameworkArea.Cast(area.FindComponent(SCR_ScenarioFrameworkArea));
		areaComponent.Init();
	}
}
