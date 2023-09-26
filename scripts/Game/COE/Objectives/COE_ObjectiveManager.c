class COE_ObjectiveManagerClass : GenericEntityClass
{
}

class COE_ObjectiveManager : GenericEntity
{
	[Attribute(defvalue: "10", desc: "Timeout until next objective is spawned")]
	protected float m_fNextObjectiveTimeout;
	
	[Attribute(desc: "Types of objectives to select from")]
	protected ref array<ref COE_ObjectiveBaseType> m_aObjectiveTypes;
	
	private static COE_ObjectiveManager m_pInstance;
	protected ref COE_ObjectiveBase m_pCurrentAO;
	protected int m_pObjectiveCounter = 0;
	
	static COE_ObjectiveManager GetInstance()
	{
		return m_pInstance;
	}
	
	void COE_ObjectiveManager(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
				
		m_pInstance = this;
		Activate();
		SetEventMask(EntityEvent.INIT);
		SCR_ArrayHelperT<ref COE_ObjectiveBaseType>.Shuffle(m_aObjectiveTypes);
	};
	
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		CreateNextObjective()
	}
	
	void ScheduleNextObjective()
	{
		GetGame().GetCallqueue().CallLater(CreateNextObjective, m_fNextObjectiveTimeout * 1000);
	}
	
	void CreateNextObjective()
	{
		
		COE_GameMode gameMode = COE_GameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
			return;
		
		if (m_pObjectiveCounter < gameMode.GetNumberOfObjectives())
		{
			COE_ObjectiveBaseType objectiveType = m_aObjectiveTypes[m_pObjectiveCounter % m_aObjectiveTypes.Count()];
			m_pCurrentAO = objectiveType.CreateInstance();
			m_pCurrentAO.GetOnObjectiveCompleted().Insert(ScheduleNextObjective)
		}	
		else
		{
			gameMode.EndGameMode(SCR_GameModeEndData.CreateSimple(EGameOverTypes.COMBATPATROL_VICTORY));
		};	
		
		m_pObjectiveCounter++;
	}
}