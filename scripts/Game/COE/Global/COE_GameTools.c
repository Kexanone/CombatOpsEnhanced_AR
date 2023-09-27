//------------------------------------------------------------------------------------------------
class COE_GameTools
{
	//------------------------------------------------------------------------------------------------
	static IEntity SpawnPrefab(ResourceName name, vector transform[4])
	{
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform = transform;
		return GetGame().SpawnEntityPrefab(Resource.Load(name), null, params);
	}
	
	//------------------------------------------------------------------------------------------------
	static IEntity SpawnStructurePrefab(ResourceName name, vector transform[4])
	{
		IEntity entity = SpawnPrefab(name, transform);
		SCR_AIWorld aiWorld = SCR_AIWorld.Cast(GetGame().GetAIWorld());
		if (aiWorld)
			aiWorld.RequestNavmeshRebuildEntity(entity);
		
		return entity;
	}
	
	//------------------------------------------------------------------------------------------------
	static SCR_AIGroup SpawnGroupPrefab(ResourceName name, vector pos, float rotation = 0)
	{
		vector transform[4];
		Math3D.AnglesToMatrix(Vector(rotation, 0, 0), transform);
		transform[3] = pos;
		return SCR_AIGroup.Cast(SpawnPrefab(name, transform));
	}
	
	//------------------------------------------------------------------------------------------------
	static SCR_AIWaypoint SpawnWaypointPrefab(ResourceName name, vector pos)
	{
		vector transform[4];
		Math3D.MatrixIdentity4(transform);
		transform[3] = pos;
		return SCR_AIWaypoint.Cast(SpawnPrefab(name, transform));
	}
	
	//------------------------------------------------------------------------------------------------
	static IEntity SpawnSFTaskPrefab(ResourceName taskPrefabName, vector pos, IEntity targetEntity = null)
	{
		vector transform[4];
		Math3D.MatrixIdentity4(transform);
		transform[3] = pos;
		IEntity layer = SpawnPrefab(taskPrefabName, transform);
		IEntity slot = layer.GetChildren();
		
		if (targetEntity && slot)
		{
			SCR_ScenarioFrameworkLayerBase slotSFComponent = SCR_ScenarioFrameworkLayerBase.Cast(slot.FindComponent(SCR_ScenarioFrameworkLayerBase));
			slotSFComponent.SetEntity(targetEntity);
		};
			
		SCR_ScenarioFrameworkLayerBase layerSFComponent = SCR_ScenarioFrameworkLayerBase.Cast(layer.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (!layerSFComponent)
			return null;
		
		layerSFComponent.Init();
		return layer;
	}
}