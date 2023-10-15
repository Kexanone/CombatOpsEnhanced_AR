//------------------------------------------------------------------------------------------------
class COE_FactionManagerClass: SCR_FactionManagerClass
{
};

//------------------------------------------------------------------------------------------------
class COE_FactionManager : SCR_FactionManager
{
	[Attribute(desc: "Configurations for opposing factions")]
	protected ref array<ref COE_OpposingFactionsConfig> m_aOpposingFactionsConfigs;
	protected ref COE_Faction m_PlayerFaction;
	protected ref COE_Faction m_EnemyFaction;
	
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		// Read mission header
		COE_MissionHeader header = COE_MissionHeader.Cast(GetGame().GetMissionHeader());
		PrintFormat("|||%1|||", header.m_aCOE_OpposingFactionsConfigs);
		if (header && !header.m_aCOE_OpposingFactionsConfigs.IsEmpty())
		{
			m_aOpposingFactionsConfigs = header.m_aCOE_OpposingFactionsConfigs;
		};
			
		if (m_aOpposingFactionsConfigs.IsEmpty())
			return;
			
		// Select opposing faction randomly from provided configs
		Math.Randomize(-1);
		COE_OpposingFactionsConfig config = m_aOpposingFactionsConfigs[Math.RandomInt(0, m_aOpposingFactionsConfigs.Count())];
		m_PlayerFaction = COE_Faction.Cast(GetFactionByKey(config.m_sPlayerFactionKey));
		m_PlayerFaction.SetIsPlayable(true);
		m_EnemyFaction = COE_Faction.Cast(GetFactionByKey(config.m_sEnemyFactionKey));
	}
	
	COE_Faction GetPlayerFaction()
	{
		return m_PlayerFaction;
	}
	
	COE_Faction GetEnemyFaction()
	{
		return m_EnemyFaction;
	}
}
