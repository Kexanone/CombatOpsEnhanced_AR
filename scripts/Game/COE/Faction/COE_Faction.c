enum COE_EPrefabLabel
{
	PLAYER_MAIN_BASE,
	INSTALLATION_TO_DESTROY,
	SUPPLIES_TO_DESTROY,
	HVT,
	INTEL,
	SMALL_ROAD_SLOT,
	MEDIUM_ROAD_SLOT,
	LARGE_ROAD_SLOT
}

//------------------------------------------------------------------------------------------------
class COE_Faction : SCR_Faction
{
	[Attribute(desc: "Prefabs for player's main base")]
	protected ref array<ResourceName> m_aPlayerMainBasePrefabs;
	
	[Attribute(desc: "Prefabs for installations to be destroyed")]
	protected ref array<ResourceName> m_aInstallationToDestroyPrefabs;
	
	[Attribute(desc: "Prefabs for supplies to be destroyed")]
	protected ref array<ResourceName> m_aSuppliesToDestroyPrefabs;
	
	[Attribute(desc: "Prefabs for high value targets")]
	protected ref array<ResourceName> m_aHVTPrefabs;
	
	[Attribute(desc: "Prefabs for pickable intels")]
	protected ref array<ResourceName> m_aIntelPrefabs;
	
	[Attribute(desc: "Prefabs for small road slots")]
	protected ref array<ResourceName> m_aSmallRoadSlotPrefabs;
	
	[Attribute(desc: "Prefabs for medium road slots")]
	protected ref array<ResourceName> m_aMediumRoadSlotPrefabs;
	
	[Attribute(desc: "Prefabs for large road slots")]
	protected ref array<ResourceName> m_aLargeRoadSlotPrefabs;
	
	[Attribute(desc: "Prefabs for MG nests")]
	protected ref array<ResourceName> m_aMGNestPrefabs;
	
	protected IEntity m_pMainBase;
	
	//------------------------------------------------------------------------------------------------
	void SetPlayerMainBase(IEntity mainBase)
	{
		m_pMainBase = mainBase;
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity GetPlayerMainBase()
	{
		return m_pMainBase;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetRandomPrefabByLabel(COE_EPrefabLabel label)
	{
		array<ResourceName> prefabs = GetPrefabsByLabel(label);
		if (prefabs.IsEmpty())
			return string.Empty;
		
		Math.Randomize(-1);
		return prefabs[Math.RandomInt(0, prefabs.Count())];
	}
	
	
	//------------------------------------------------------------------------------------------------
	array<ResourceName> GetPrefabsByLabel(COE_EPrefabLabel label)
	{
		switch (label)
		{
			case COE_EPrefabLabel.PLAYER_MAIN_BASE: {return m_aPlayerMainBasePrefabs;};
			case COE_EPrefabLabel.INSTALLATION_TO_DESTROY: {return m_aInstallationToDestroyPrefabs;};
			case COE_EPrefabLabel.SUPPLIES_TO_DESTROY: {return m_aSuppliesToDestroyPrefabs;};
			case COE_EPrefabLabel.HVT: {return m_aHVTPrefabs;};
			case COE_EPrefabLabel.INTEL: {return m_aIntelPrefabs;};
			case COE_EPrefabLabel.SMALL_ROAD_SLOT: {return m_aSmallRoadSlotPrefabs;};
			case COE_EPrefabLabel.MEDIUM_ROAD_SLOT: {return m_aMediumRoadSlotPrefabs;};
			case COE_EPrefabLabel.LARGE_ROAD_SLOT: {return m_aLargeRoadSlotPrefabs;};
		};
		
		return {};
	}
};
