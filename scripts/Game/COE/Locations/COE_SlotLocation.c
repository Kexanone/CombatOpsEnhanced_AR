//------------------------------------------------------------------------------------------------
class COE_SlotLocationConfig : COE_PrefabLocationConfig
{
	[Attribute(desc: "Prefab to spawn on slot")]
	protected ResourceName m_sPrefabToSpawn;

	//------------------------------------------------------------------------------------------------
	override COE_SlotLocation Create()
	{
		IEntity entity = PickEntity();
		if (!entity)
			return null;
		
		COE_SlotLocation location = COE_SlotLocation(entity.GetOrigin(), m_fLocationRadius);
		location.SetMainPrefabName(m_sPrefabToSpawn);
		return location;
	}
}

//------------------------------------------------------------------------------------------------
class COE_SlotLocation : COE_PrefabLocation
{
}
