class COE_ObjectiveManagerClass : GenericEntityClass
{
}

class COE_ObjectiveManager : GenericEntity
{
	[Attribute("3", UIWidgets.EditBox, desc: "Number of objectives required to complete the mission")]
	protected int m_iNumberOfObjectives;
	
	[Attribute(defvalue: "15", desc: "Timeout until next objective is spawned")]
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
		
		SCR_ArrayHelperT<ref COE_ObjectiveBaseType>.Shuffle(m_aObjectiveTypes);
		
		/*
		COE_MissionHeader header = COE_MissionHeader.Cast(GetGame().GetMissionHeader());
		if (header)
		{
			PrintFormat("||GOG||%1||%2||", m_iNumberOfObjectives, header.m_iCOE_NumberOfObjectives);
			m_iNumberOfObjectives = header.m_iCOE_NumberOfObjectives;
			PrintFormat("||KEK||%1||%2||", m_iNumberOfObjectives, header.m_iCOE_NumberOfObjectives);
		};
		*/
		
		m_pInstance = this;
		Activate();
		SetEventMask(EntityEvent.INIT);
	};
	
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		ScheduleNextObjective();
	}
	
	void ScheduleNextObjective()
	{
		if (!Replication.IsServer())
			return;
		
		GetGame().GetCallqueue().CallLater(CreateNextObjective, m_fNextObjectiveTimeout * 1000);
	}
	
	void CreateNextObjective()
	{
		if (!Replication.IsServer())
			return;
		
		COE_GameMode gameMode = COE_GameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
			return;
		
		if (m_pObjectiveCounter < m_iNumberOfObjectives)
		{
			COE_ObjectiveBaseType objectiveType = m_aObjectiveTypes[m_pObjectiveCounter % m_aObjectiveTypes.Count()];
			m_pCurrentAO = objectiveType.CreateInstance();
		}	
		else
		{
			m_pCurrentAO = COE_ObjectiveExfil();
		};	
		
		m_pObjectiveCounter++;
	}
}