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
	static AIGroup SpawnGroupPrefab(ResourceName name, vector pos = vector.Zero, float rotation = 0)
	{
		vector transform[4];
		Math3D.AnglesToMatrix(Vector(rotation, 0, 0), transform);
		transform[3] = pos;
		return AIGroup.Cast(SpawnPrefab(name, transform));
	}
	
	//------------------------------------------------------------------------------------------------
	static AIWaypoint SpawnWaypointPrefab(ResourceName name, vector pos)
	{
		vector transform[4];
		Math3D.MatrixIdentity4(transform);
		transform[3] = pos;
		return AIWaypoint.Cast(SpawnPrefab(name, transform));
	}
}