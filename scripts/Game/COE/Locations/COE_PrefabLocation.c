//------------------------------------------------------------------------------------------------
class COE_PrefabLocationConfig : COE_CustomLocationConfig
{
	[Attribute(desc: "Prefabs to search for")]
	protected ref array<ResourceName> m_aPrefabsToQuery;
	
	//------------------------------------------------------------------------------------------------
	override protected bool QueryEntitiesCallback(IEntity entity)
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
}
