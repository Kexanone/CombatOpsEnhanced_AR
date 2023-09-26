//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class KEX_CombatOps_FactionPrefabs : Managed
{
	[Attribute("", UIWidgets.EditBox, desc: "Faction key")]
	string m_sFactionKey;

	[Attribute(desc: "Spawn point prefab")]
	ResourceName m_sSpawnPointPrefab;
	
	[Attribute(desc: "Prefab of the fireplace at the spawn")]
	ResourceName m_sSpawnFireplacePrefab;
	
	[Attribute(desc: "Prefab of the chair at the spawn")]
	ResourceName m_sSpawnChairPrefab;
	
	[Attribute(desc: "Prefab of the tent at the spawn")]
	ResourceName m_sSpawnTentPrefab;
	
	[Attribute(desc: "Prefab of the arsenal box at the spawn")]
	ResourceName m_sArsenalBoxPrefab;

	[Attribute(desc: "Prefab of the vehicle at the spawn")]
	ResourceName m_sSpawnVehiclePrefab;
	
	[Attribute(desc: "Patrol group Prefabs")]
	array<ResourceName> m_PatrolGroupPrefabs;
	
	[Attribute(desc: "Sniper team prefabs")]
	array<ResourceName> m_SniperTeamPrefabs;
	
	[Attribute(desc: "HVT character prefabs")]
	array<ResourceName> m_HVTCharacterPrefabs;
	
	[Attribute(desc: "Unarmed HVT vehicle prefabs")]
	array<ResourceName> m_UnarmedHVTVehiclePrefabs;
	
	[Attribute(desc: "Armed HVT vehicle prefabs")]
	array<ResourceName> m_ArmedHVTVehiclePrefabs;

	[Attribute(desc: "Intel prefabs")]
	array<ResourceName> m_IntelPrefabs;

	[Attribute(desc: "Radar prefabs")]
	array<ResourceName> m_RadarPrefabs;
};

//------------------------------------------------------------------------------------------------
class KEX_GameModeCombatOpsClass : SCR_BaseGameModeClass
{
};

//------------------------------------------------------------------------------------------------
class KEX_GameModeCombatOps : SCR_BaseGameMode
{
	[Attribute(category: "Locations")]
	protected array<string> m_StartLocations;
	
	[Attribute(category: "Locations")]
	protected array<string> m_ObjectiveLocations;
	
	[Attribute(category: "Locations")]
	protected array<string> m_ExfilLocations;
	
	[Attribute("", UIWidgets.EditBox, desc: "Playable faction name. Randomly chosen if left empty.", category: "Factions")]
	protected string m_sPlayerFactionKey;
	protected string m_sEnemyFactionKey;
	protected bool m_bFactionsAssigned = false;
	
	[Attribute(desc: "Configure prefabs associated with each faction", category: "Factions")]
	protected ref array<ref KEX_CombatOps_FactionPrefabs> m_FactionPrefabs;
		
	//------------------------------------------------------------------------------------------------
	void KEX_GameModeCombatOps(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode())
			return;
		
		// Read mission header
		KEX_MissionHeaderCombatOps header = KEX_MissionHeaderCombatOps.Cast(GetGame().GetMissionHeader());
		
		if (header)
		{
			m_sPlayerFactionKey = header.m_sCombatOpsPlayerFactionKey;
		};
	};
	
	//------------------------------------------------------------------------------------------------
	override protected void OnGameModeStart()
	{
		if (!m_bFactionsAssigned)
			AssignFactions();
		
		super.OnGameModeStart();
	};
	
	//------------------------------------------------------------------------------------------------
	//! Assigns player and enemy factions based on m_sPlayerFactionKey
	protected void AssignFactions()
	{
		// Assign factions randomly if not specified
		if (m_sPlayerFactionKey.IsEmpty())
		{
			Math.Randomize(-1);
			if (Math.RandomInt(0, 2))
			{
				m_sPlayerFactionKey = "USSR";
			}
			else
			{
				m_sPlayerFactionKey = "US";
			};
		};
		
		if (m_sPlayerFactionKey == "USSR")
		{
			m_sEnemyFactionKey = "US";
		}
		else
		{
			m_sEnemyFactionKey = "USSR";
		};
		
		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		SCR_Faction playerFaction = SCR_Faction.Cast(factionManager.GetFactionByKey(m_sPlayerFactionKey));
		SCR_Faction enemyFaction = SCR_Faction.Cast(factionManager.GetFactionByKey(m_sEnemyFactionKey));
		playerFaction.SetIsPlayable(true);
		enemyFaction.SetIsPlayable(false);
		m_bFactionsAssigned = true;
	};
	
	KEX_CombatOps_FactionPrefabs GetPlayerFactionPrefabs()
	{
		if (!m_bFactionsAssigned)
			AssignFactions();
		
		foreach (KEX_CombatOps_FactionPrefabs prefabs : m_FactionPrefabs)
		{
			if (prefabs.m_sFactionKey == m_sPlayerFactionKey)
			{
				return prefabs;
			};
		};
		
		return null;
	};

	KEX_CombatOps_FactionPrefabs GetEnemyFactionPrefabs()
	{
		if (!m_bFactionsAssigned)
			AssignFactions();
		
		foreach (KEX_CombatOps_FactionPrefabs prefabs : m_FactionPrefabs)
		{
			if (prefabs.m_sFactionKey == m_sEnemyFactionKey)
			{
				return prefabs;
			};
		};
		
		return null;
	};
};
