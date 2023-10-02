//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class COE_EntitySpawner_PrefabParams : Managed
{
	[Attribute(desc: "Resource name of the prefab")]
	ResourceName m_sPrefabName;
	
	[Attribute(defvalue: "1", desc: "Number of times it get spawned")]
	int m_iSpawnNumber;
}

//------------------------------------------------------------------------------------------------
class COE_EntitySpawnerClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class COE_EntitySpawner : GenericEntity
{
	[Attribute(desc: "Inner border for spawn positions", category: "Area")]
	protected string m_sInnerBorderName;
	
	[Attribute(desc: "Outer border for spawn positions", category: "Area")]
	protected string m_sOuterBorderName;
	
	[Attribute(desc: "Outer border for spawn positions", category: "Prefabs")]
	protected ref array<ref COE_EntitySpawner_PrefabParams> m_aPrefabParamsList;
	
	protected ref array<IEntity> m_aSpawnedEntities = {};
	
	//------------------------------------------------------------------------------------------------
	void Spawn()
	{
		Math.Randomize(-1);
		
		COE_PolygonAreaPicker innerBorderPicker = COE_PolygonAreaPicker.Cast(GetGame().GetWorld().FindEntityByName(m_sInnerBorderName));
		if (!innerBorderPicker)
			return;
		
		COE_PolygonArea innerBorder = COE_PolygonArea.Cast(innerBorderPicker.GetArea());

		COE_PolygonAreaPicker outerBorderPicker = COE_PolygonAreaPicker.Cast(GetGame().GetWorld().FindEntityByName(m_sOuterBorderName));
		if (!outerBorderPicker)
			return;
		
		COE_PolygonArea outerBorder = COE_PolygonArea.Cast(outerBorderPicker.GetArea());
		
		foreach (COE_EntitySpawner_PrefabParams prefabParams : m_aPrefabParamsList)
		{
			for (int i_spawn = 0; i_spawn < prefabParams.m_iSpawnNumber; i_spawn++)
			{
				EntitySpawnParams params = EntitySpawnParams();
				params.TransformMode = ETransformMode.WORLD;
				Math3D.AnglesToMatrix(Vector(Math.RandomFloat(0, 360), 0, 0), params.Transform);				
				params.Transform[3] = COE_AreaBase.SamplePointInArea(outerBorder, innerBorder);
				IEntity entity = GetGame().SpawnEntityPrefab(Resource.Load(prefabParams.m_sPrefabName), null, params);
				m_aSpawnedEntities.Insert(entity);
			};
		};
	}
	
	array<IEntity> GetSpawnedEntities()
	{
		return m_aSpawnedEntities;
	}
}