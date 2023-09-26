//------------------------------------------------------------------------------------------------
class KEX_CombatOps_SlotBaseClass : GenericEntityClass
{
};

//------------------------------------------------------------------------------------------------
class KEX_CombatOps_SlotBase : GenericEntity
{
	[Attribute("100", UIWidgets.EditBox, desc: "Radius of slot")]
	protected float m_sSlotRadius;
	
	protected static KEX_CombatOps_FactionPrefabs m_PlayerFactionPrefabs;
	protected static KEX_CombatOps_FactionPrefabs m_EnemyFactionPrefabs;
	protected static ref RandomGenerator m_RNG = RandomGenerator();
	
	//------------------------------------------------------------------------------------------------
	void KEX_CombatOps_SlotBase(IEntitySource src, IEntity parent)
	{
		Activate();
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
	};

	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		if (!m_PlayerFactionPrefabs)
		{
			KEX_GameModeCombatOps gameMode = KEX_GameModeCombatOps.Cast(GetGame().GetGameMode());
			m_PlayerFactionPrefabs = gameMode.GetPlayerFactionPrefabs();
			m_EnemyFactionPrefabs = gameMode.GetEnemyFactionPrefabs();
		};
	};
	
	//------------------------------------------------------------------------------------------------
	void Spawn()
	{
	};
};