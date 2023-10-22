class COE_ObjectiveManagerClass : GenericEntityClass
{
}

class COE_ObjectiveManager : GenericEntity
{
	[Attribute("3", UIWidgets.EditBox, desc: "Number of objectives required to complete the mission")]
	protected int m_iNumberOfObjectives;
	
	[Attribute(defvalue: "15", desc: "Timeout until next objective is spawned")]
	protected float m_fNextObjectiveTimeout;
	
	[Attribute(defvalue: "500", desc: "Minimum distance between objectives in meter")]
	protected float m_fMinDistanceObjectives;
	
	[Attribute(desc: "Configs for objective locations")]
	ref array<ref COE_LocationBaseConfig> m_aLocationConfigs;
	
	[Attribute(desc: "Configs for objective tasks")]
	ref array<ref COE_TaskBaseConfig> m_aTaskConfigs;
	
	[Attribute(desc: "Config for exfil")]
	protected ref COE_TaskBaseConfig m_aExfilConfig;
	
	private static COE_ObjectiveManager m_pInstance;
	protected ref COE_Location m_pCurrentLocation;
	protected ref array<ref COE_Location> m_pOldLocations = {};
	protected int m_pObjectiveCounter = 0;
	
	static COE_ObjectiveManager GetInstance()
	{
		return m_pInstance;
	}
	
	void COE_ObjectiveManager(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		// Remove disabled location configs
		for (int i = m_aLocationConfigs.Count() - 1; i >= 0; i--)
		{
			COE_LocationBaseConfig locationConfig = m_aLocationConfigs[i];
			if (!locationConfig.IsEnabled())
				m_aLocationConfigs.Remove(i);
		};
		
		// Remove disabled task configs
		for (int i = m_aTaskConfigs.Count() - 1; i >= 0; i--)
		{
			COE_TaskBaseConfig taskConfig = m_aTaskConfigs[i];
			if (!taskConfig.IsEnabled())
				m_aTaskConfigs.Remove(i);
		};
		
		Math.Randomize(-1);
		SCR_ArrayHelperT<ref COE_TaskBaseConfig>.Shuffle(m_aTaskConfigs);
		
		COE_MissionHeader header = COE_MissionHeader.Cast(GetGame().GetMissionHeader());
		if (header)
		{
			m_iNumberOfObjectives = header.m_iCOE_NumberOfObjectives;
		};
		
		m_pInstance = this;
	};
	
	void OnGameStart()
	{
		foreach (COE_LocationBaseConfig config : m_aLocationConfigs)
		{
			config.Init();
		};
		
		foreach (COE_TaskBaseConfig config : m_aTaskConfigs)
		{
			config.Init();
		};
		
		ScheduleNextObjective();
	}
	
	void ScheduleNextObjective()
	{
		if (!Replication.IsServer())
			return;
		
		// Clean-up previous location
		if (m_pCurrentLocation)
		{
			m_pCurrentLocation.ScheduleCleanUp();
			m_pOldLocations.Insert(m_pCurrentLocation);
			m_pCurrentLocation = null;
		};
		
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
				m_aTaskConfigs.RemoveItem(taskConfig);
				CreateNextObjective();
				return;
			};
			
			COE_Location location = locationConfig.Create();
			if (!location)
			{
				COE_Utils.PrintError(string.Format("Location of type %1 could not be created", locationConfig), "COE_ObjectiveManager");
				m_aLocationConfigs.RemoveItem(locationConfig);
				CreateNextObjective();
				return;
			};
			
			m_pCurrentLocation = location;
			taskConfig.Create(m_pCurrentLocation);
			m_pCurrentLocation.SpawnAI();
			
			// Add location to excluded areas for next objective
			COE_CircleArea area = COE_CircleArea(m_pCurrentLocation.GetCenter(), m_fMinDistanceObjectives);
			
			foreach (COE_LocationBaseConfig config : m_aLocationConfigs)
			{
				config.AddExcludedArea(area);
			};
		}	
		else
		{
			m_aExfilConfig.Init();
			m_aExfilConfig.Create();
		};	
		
		m_pObjectiveCounter++;
	}
	
	COE_LocationBaseConfig GetRandomLocationConfigForTaskConfig(COE_TaskBaseConfig taskConfig)
	{
		COE_ETaskType taskType = taskConfig.GetTaskType();
		
		SCR_WeightedArray<COE_LocationBaseConfig> locationConfigs = new SCR_WeightedArray<COE_LocationBaseConfig>;
		foreach (COE_LocationBaseConfig locationConfig : m_aLocationConfigs)
		{
			if (locationConfig.TaskTypeIsSupported(taskType))
				locationConfigs.Insert(locationConfig, locationConfig.GetWeight());
		};
		
		if (locationConfigs.IsEmpty())
			return null;
		
		COE_LocationBaseConfig locationConfig;
		Math.Randomize(-1);
		locationConfigs.GetRandomValue(locationConfig);
		return locationConfig;		
	}
}