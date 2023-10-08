enum COE_EEntityLabel
{
	PLAYER_MAIN_BASE,
	INSTALLATION_TO_DESTROY,
	SUPPLIES_TO_DESTROY,
	HVT,
	INTEL,
	SMALL_ROAD_SLOT,
	MEDIUM_ROAD_SLOT,
	LARGE_ROAD_SLOT,
	SMALL_FLAT_SLOT,
	MEDIUM_FLAT_SLOT,
	LARGE_FLAT_SLOT,
	MG_NEST,
	BUNKER,
	OCCUPIED_APC,
	MARKSMAN,
	RIFLEMAN_MG,
	RIFLEMAN_GL,
	RIFLEMAN_AT,
	RIFLEMAN,
	TEAM,
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
	
	[Attribute(desc: "Prefabs for large flat slots")]
	protected ref array<ResourceName> m_aLargeFlatSlotPrefabs;
	
	[Attribute(desc: "Prefabs for MG nests")]
	protected ref array<ResourceName> m_aMGNestPrefabs;
	
	[Attribute(desc: "Prefabs for occupied APC")]
	protected ref array<ResourceName> m_aOccupiedAPCPrefabs;
	
	[Attribute(desc: "Prefabs for marksman")]
	protected ref array<ResourceName> m_aMarksmanPrefabs;
	
	[Attribute(desc: "Prefabs for rifleman MG")]
	protected ref array<ResourceName> m_aRiflemanMGPrefabs;
	
	[Attribute(desc: "Prefabs for rifleman GL")]
	protected ref array<ResourceName> m_aRiflemanGLPrefabs;

	[Attribute(desc: "Prefabs for rifleman AT")]
	protected ref array<ResourceName> m_aRiflemanATPrefabs;
	
	[Attribute(desc: "Prefabs for rifleman")]
	protected ref array<ResourceName> m_aRiflemanPrefabs;
	
	[Attribute(desc: "Prefabs for team")]
	protected ref array<ResourceName> m_aTeamPrefabs;
		
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
	ResourceName GetRandomPrefabByLabel(COE_EEntityLabel label)
	{
		array<ResourceName> prefabs = GetPrefabsByLabel(label);
		if (prefabs.IsEmpty())
			return string.Empty;
		
		Math.Randomize(-1);
		return prefabs[Math.RandomInt(0, prefabs.Count())];
	}
	
	
	//------------------------------------------------------------------------------------------------
	array<ResourceName> GetPrefabsByLabel(COE_EEntityLabel label)
	{
		switch (label)
		{
			case COE_EEntityLabel.PLAYER_MAIN_BASE: {return m_aPlayerMainBasePrefabs;};
			case COE_EEntityLabel.INSTALLATION_TO_DESTROY: {return m_aInstallationToDestroyPrefabs;};
			case COE_EEntityLabel.SUPPLIES_TO_DESTROY: {return m_aSuppliesToDestroyPrefabs;};
			case COE_EEntityLabel.HVT: {return m_aHVTPrefabs;};
			case COE_EEntityLabel.INTEL: {return m_aIntelPrefabs;};
			case COE_EEntityLabel.SMALL_ROAD_SLOT: {return m_aSmallRoadSlotPrefabs;};
			case COE_EEntityLabel.MEDIUM_ROAD_SLOT: {return m_aMediumRoadSlotPrefabs;};
			case COE_EEntityLabel.LARGE_ROAD_SLOT: {return m_aLargeRoadSlotPrefabs;};
			case COE_EEntityLabel.LARGE_FLAT_SLOT: {return m_aLargeFlatSlotPrefabs;};
			case COE_EEntityLabel.MARKSMAN: {return m_aMarksmanPrefabs;};
			case COE_EEntityLabel.RIFLEMAN: {return m_aRiflemanPrefabs;};
		};
		
		return {};
	}
};
