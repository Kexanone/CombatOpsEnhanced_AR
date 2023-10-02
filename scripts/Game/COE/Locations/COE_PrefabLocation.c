//------------------------------------------------------------------------------------------------
class COE_PrefabLocationConfig : COE_LocationBaseConfig
{
	[Attribute(desc: "Prefabs to search for")]
	protected ref array<ResourceName> m_aPrefabsToQuery;
	protected ref array<IEntity> m_aEntitiesToPick = {};
	
	//------------------------------------------------------------------------------------------------
	override void Init()
	{
		super.Init();
		vector minPos, maxPos;
		m_pSampledArea.GetBoundBox(minPos, maxPos);
		GetGame().GetWorld().QueryEntitiesByAABB(minPos, maxPos, QueryEntitiesCallback);
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool QueryEntitiesCallback(IEntity entity)
	{
		EntityPrefabData data = entity.GetPrefabData();
		if (!data)
			return true;
				
		if (!m_aPrefabsToQuery.IsEmpty() && m_aPrefabsToQuery.Find(data.GetPrefabName()) < 0)
			return true;
		
		vector pos = entity.GetOrigin();
		
		if (!m_pSampledArea.IsPointInArea(pos))
			return true;
		
		foreach (COE_AreaBase area : m_aExcludedAreas)
		{
			if (area.IsPointInArea(pos))
				return true;
		};
		
		m_aEntitiesToPick.Insert(entity);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected IEntity PickEntity()
	{
		if (m_aEntitiesToPick.IsEmpty())
			return null;
		
		return m_aEntitiesToPick[Math.RandomInt(0, m_aEntitiesToPick.Count())];
	}
	
	//------------------------------------------------------------------------------------------------
	override COE_PrefabLocation Create()
	{
		IEntity entity = PickEntity();
		if (!entity)
			return null;
		
		COE_PrefabLocation location = COE_PrefabLocation(entity.GetOrigin(), m_fLocationRadius);
		location.SetMainStructure(entity);
		return location;
	}
}

//------------------------------------------------------------------------------------------------
class COE_PrefabLocation : COE_LocationBase
{
}
