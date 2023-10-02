//------------------------------------------------------------------------------------------------
class COE_LocationBaseConfig : ScriptAndConfig
{
#ifdef WORKBENCH
	[Attribute()]
	protected string m_sComment;
#endif
	
	[Attribute(defvalue: "7000", desc: "Maximum distance to main base in meters")]
	protected float m_fMaxDistanceMainBase;
	
	[Attribute(desc: "Names of area picker entities for excluded area")]
	protected ref array<string> m_aExcludedAreaPickerNames;
	protected ref array<ref COE_AreaBase> m_aExcludedAreas = {};
	
	[Attribute(defvalue: "100", desc: "Radius for the generated location in meters")]
	protected float m_fLocationRadius;
	
	[Attribute(desc: "Types of tasks that can be generated on this location", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_ETaskType))]
	protected ref array<COE_ETaskType> m_aSupportedTaskTypes;
	
	protected ref COE_AreaBase m_pSampledArea;
	
	//------------------------------------------------------------------------------------------------
	void Init()
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		IEntity mainBase = faction.GetPlayerMainBase();
		if (!mainBase)
			return;
		
		m_pSampledArea = COE_CircleArea(mainBase.GetOrigin(), m_fMaxDistanceMainBase);
		
		// Get and store excluded areas
		foreach (string pickerName : m_aExcludedAreaPickerNames)
		{
			COE_AreaPickerBase picker = COE_AreaPickerBase.Cast(GetGame().GetWorld().FindEntityByName(pickerName));
			if (picker)
				m_aExcludedAreas.Insert(picker.GetArea());
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool PickPosition(out vector pos) { return false; }
	
	//------------------------------------------------------------------------------------------------
	protected bool IsPositionAccepted(vector pos)
	{
		if (!m_pSampledArea.IsPointInArea(pos))
			return false;
		
		foreach (COE_AreaBase area : m_aExcludedAreas)
		{
			if (area.IsPointInArea(pos))
				return false;
		};
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	COE_LocationBase Create()
	{
		vector pos;
		if (!PickPosition(pos))
			return null;
		
		return COE_LocationBase(pos, m_fLocationRadius);
	}
	
	//------------------------------------------------------------------------------------------------
	bool TaskTypeIsSupported(COE_ETaskType taskTypeToQuery)
	{
		foreach (COE_ETaskType taskType : m_aSupportedTaskTypes)
		{
			if (taskType == taskTypeToQuery)
				return true;
		};
		
		return false;
	}
}

//------------------------------------------------------------------------------------------------
class COE_LocationBase : Managed
{
	protected vector m_vCenter;
	protected float m_fRadius;
	protected ResourceName m_sMainPrefabName;
	protected ref array<IEntity> m_aStructures = {};
	protected ref array<AIGroup> m_aGroups = {};
	protected COE_Faction m_Faction;
	
	void COE_LocationBase(vector center, float radius)
	{
		m_vCenter = center;
		m_fRadius = radius;
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		m_Faction = factionManager.GetEnemyFaction();
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMainPrefabName(ResourceName prefabName)
	{
		m_sMainPrefabName = prefabName;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMainStructure(IEntity entity)
	{
		if (m_aStructures.IsEmpty())
			m_aStructures.Insert(entity);
		else
			m_aStructures[0] = entity;
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity GetMainStructure()
	{
		if (m_aStructures.IsEmpty())
			return null;
		
		return m_aStructures[0];
	}
	
	//------------------------------------------------------------------------------------------------
	array<IEntity> GetStructures()
	{
		return m_aStructures;
	}
	
	//------------------------------------------------------------------------------------------------
	array<AIGroup> GetGroups()
	{
		return m_aGroups;
	}
	
	//------------------------------------------------------------------------------------------------
	void Generate()
	{
	}
}
