class COE_ObjectiveManagerClass : GenericEntityClass
{
}

class COE_ObjectiveManager : GenericEntity
{
	[Attribute("3", UIWidgets.EditBox, desc: "Number of objectives required to complete the mission")]
	protected int m_iNumberOfObjectives;
	
	[Attribute(defvalue: "15", desc: "Timeout until next objective is spawned")]
	protected float m_fNextObjectiveTimeout;
	
	[Attribute(desc: "Configs for objective locations")]
	ref array<ref COE_LocationBaseConfig> m_aLocationConfigs;
	
	[Attribute(desc: "Configs for objective tasks")]
	ref array<ref COE_TaskBaseConfig> m_aTaskConfigs;
	
	[Attribute(desc: "Config for exfil")]
	protected ref COE_TaskBaseConfig m_aExfilConfig;
	
	private static COE_ObjectiveManager m_pInstance;
	protected ref COE_LocationBase m_pCurrentLocation;
	protected int m_pObjectiveCounter = 0;
	
	static COE_ObjectiveManager GetInstance()
	{
		return m_pInstance;
	}
	
	void COE_ObjectiveManager(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		SCR_ArrayHelperT<ref COE_TaskBaseConfig>.Shuffle(m_aTaskConfigs);
		
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
	};
	
	void OnGameStart()
	{
		foreach (COE_LocationBaseConfig config : m_aLocationConfigs)
		{
			config.Init();
		};
		
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
		
		if (m_aTaskConfigs.IsEmpty())
		{
			COE_Utils.PrintError("No tasks are configured", "COE_ObjectiveManager");
			return;
		};
		
		if (m_pObjectiveCounter < m_iNumberOfObjectives)
		{
			COE_TaskBaseConfig taskConfig = m_aTaskConfigs[m_pObjectiveCounter % m_aTaskConfigs.Count()];
			COE_LocationBaseConfig locationConfig = GetRandomLocationConfigForTaskConfig(taskConfig);
			if (!locationConfig)
			{
				COE_Utils.PrintError(string.Format("No suitable location config found for %1", taskConfig), "COE_ObjectiveManager");
				return;
			}
			
			m_pCurrentLocation = locationConfig.Create();
			taskConfig.Create(m_pCurrentLocation);
		}	
		else
		{
			m_aExfilConfig.Create();
		};	
		
		m_pObjectiveCounter++;
	}
	
	COE_LocationBaseConfig GetRandomLocationConfigForTaskConfig(COE_TaskBaseConfig taskConfig)
	{
		COE_ETaskType taskType = taskConfig.GetTaskType();
		
		array<COE_LocationBaseConfig> locationConfigs = {};
		foreach (COE_LocationBaseConfig locationConfig : m_aLocationConfigs)
		{
			if (locationConfig.TaskTypeIsSupported(taskType))
			 locationConfigs.Insert(locationConfig);
		};
		
		if (!locationConfigs.IsEmpty())
			return null;
		
		Math.Randomize(-1);
		return locationConfigs[Math.RandomInt(0, locationConfigs.Count())];		
	}
}