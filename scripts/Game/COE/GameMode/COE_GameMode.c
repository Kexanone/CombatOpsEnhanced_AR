//------------------------------------------------------------------------------------------------
class COE_GameModeClass : SCR_BaseGameModeClass
{
};

//------------------------------------------------------------------------------------------------
class COE_GameMode : SCR_BaseGameMode
{
	[Attribute("US", UIWidgets.EditBox, desc: "Playable faction name. Randomly chosen if left empty.", category: "Factions")]
	protected string m_sPlayerFactionKey;
	protected string m_sEnemyFactionKey;
	protected bool m_bFactionsAssigned = false;

	[Attribute("3", UIWidgets.EditBox, desc: "Number of objectives required to complete the mission")]
	protected int m_iNumberOfObjectives;
	
	//------------------------------------------------------------------------------------------------
	void COE_GameMode(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		Math.Randomize(-1);
		
		// Read mission header
		COE_MissionHeader header = COE_MissionHeader.Cast(GetGame().GetMissionHeader());
		
		if (header)
		{
			m_iNumberOfObjectives = header.m_iCOE_NumberOfObjectives;
		};
	};
	
	//------------------------------------------------------------------------------------------------
	override protected void OnGameModeStart()
	{
		super.OnGameModeStart();
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		if (!m_bFactionsAssigned)
			AssignFactions();
		
		COE_EntitySpawner carrierSpawner = COE_EntitySpawner.Cast(GetGame().GetWorld().FindEntityByName("CarrierSpawner"));
		if (carrierSpawner)
			carrierSpawner.Spawn();
	}
	
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
	
	int GetNumberOfObjectives()
	{
		return m_iNumberOfObjectives;
	}
};
