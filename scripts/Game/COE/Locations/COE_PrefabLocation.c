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
		
		if (!IsPositionAccepted(pos))
			return true;
		
		m_aEntitiesToPick.Insert(entity);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool PickPosition(out vector pos, out IEntity associatedStructure)
	{
		if (m_aEntitiesToPick.IsEmpty())
			return false;
				
		while (true)
		{
			if (m_aEntitiesToPick.IsEmpty())
				return false;
			
			associatedStructure = m_aEntitiesToPick[Math.RandomInt(0, m_aEntitiesToPick.Count())];
			pos = associatedStructure.GetOrigin();
			
			if (IsPositionAccepted(pos))
				return true;
			
			m_aEntitiesToPick.RemoveItem(associatedStructure);
		}
		
		return false;
	}
}
