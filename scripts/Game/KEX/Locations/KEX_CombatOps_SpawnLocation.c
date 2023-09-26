//------------------------------------------------------------------------------------------------
class KEX_CombatOps_SpawnSlotClass : KEX_CombatOps_SlotBaseClass
{
};

//------------------------------------------------------------------------------------------------
class KEX_CombatOps_SpawnSlot : KEX_CombatOps_SlotBase
{
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
	};
	
	//------------------------------------------------------------------------------------------------
	override void Spawn()
	{
		super.Spawn();
		vector centerPos = m_RNG.GenerateRandomPointInRadius(0, m_sSlotRadius, GetOrigin());
		KEX_Utils.SpawnPrefabOnTerrain(m_PlayerFactionPrefabs.m_sArsenalBoxPrefab, centerPos);
		IEntity fireplace = KEX_Utils.SpawnPrefabOnTerrain(m_PlayerFactionPrefabs.m_sSpawnFireplacePrefab, m_RNG.GenerateRandomPointInRadius(0, 7, centerPos), 2);
		vector fireplacePos = fireplace.GetOrigin();
		vector spawnPos = m_RNG.GenerateRandomPointInRadius(0, 5, fireplacePos);
		float spawnYaw = vector.Direction(spawnPos, fireplacePos).ToYaw();
		KEX_Utils.SpawnPrefabOnTerrain(m_PlayerFactionPrefabs.m_sSpawnPointPrefab, spawnPos, 0.5, 2, spawnYaw);

		for (int i = 0; i < 2; i++)
		{
			KEX_Utils.SpawnPrefabOnTerrain(m_PlayerFactionPrefabs.m_sSpawnChairPrefab, m_RNG.GenerateRandomPointInRadius(1, 2, fireplacePos));
			KEX_Utils.SpawnPrefabOnTerrain(m_PlayerFactionPrefabs.m_sSpawnTentPrefab, m_RNG.GenerateRandomPointInRadius(3, 5, fireplacePos));
		};
		
		for (int i = 0; i < 3; i++)
		{
			spawnPos = m_RNG.GenerateRandomPointInRadius(5, 10, fireplacePos);
			spawnYaw = vector.Direction(fireplacePos, spawnPos).ToYaw();
			KEX_Utils.SpawnPrefabOnTerrain(m_PlayerFactionPrefabs.m_sSpawnVehiclePrefab, spawnPos, 3, 0, spawnYaw);
		};
		
	};
};

class KEX_Utils
{
	// -- TO DO: Use initial entity and broadcast its transfomr (see SCR_EditableEntityComponent.SetTransformBroadcast)
	static IEntity SpawnPrefabOnTerrain(ResourceName prefabName, vector position, float minCylinderRadius = 0, float minCylinderHeight = 0, float yaw = -1, float searchRadius = 20)
	{
		IEntity entity = GetGame().SpawnEntityPrefab(Resource.Load(prefabName));
		vector min, max;
		entity.GetBounds(min, max);
		vector bounds = max - min;
		// Circumscribe cylinder around bounding box
		float cylinderRadius = Math.Sqrt(2) / 2 * Math.Max(bounds[0], bounds[2]);
		cylinderRadius = Math.Max(cylinderRadius, minCylinderRadius);
		float cylinderHeight = Math.Max(bounds[1], minCylinderHeight);
		SCR_EntityHelper.DeleteEntityAndChildren(entity);
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		vector newPos;
		SCR_WorldTools.FindEmptyTerrainPosition(newPos, position, searchRadius);
		params.Transform[3] = newPos;

		SCR_TerrainHelper.SnapAndOrientToTerrain(params.Transform);
		
		if (yaw < 0)
		{
			Math.Randomize(-1);
			yaw = Math.RandomFloat(0, 360);
		};
		
		// Rotate yaw
		float transQuat[4];
		Math3D.MatrixToQuat(params.Transform, transQuat);
		vector rotAngles = {yaw, 0, 0};
		vector rotMat[3];
		Math3D.AnglesToMatrix(rotAngles, rotMat);
		float rotQuat[4];
		Math3D.MatrixToQuat(rotMat, rotQuat);
		Math3D.QuatMultiply(transQuat, transQuat, rotQuat);
		Math3D.QuatToMatrix(transQuat, params.Transform);
		return GetGame().SpawnEntityPrefab(Resource.Load(prefabName), null, params);
	};
};