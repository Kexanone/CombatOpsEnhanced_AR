//------------------------------------------------------------------------------------------------
enum COE_ETaskType
{
	CLEAR_AREA,
	DESTROY_INSTALLATION,
	DESTROY_SUPPLIES,
	DESTROY_ARMOR,
	KILL_HVT,
	FIND_INTEL,
	EXFIL
}

//------------------------------------------------------------------------------------------------
class COE_LoationBaseConfig : ScriptAndConfig
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
	void COE_LoationBaseConfig()
	{
		if (!GetGame().InPlayMode())
			return;
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		vector mainBasePos = factionManager.GetPlayerFaction().GetPlayerMainBase().GetOrigin();
		m_pSampledArea = COE_CircleArea(mainBasePos, m_fMaxDistanceMainBase);
		
		// Get and store excluded areas
		foreach (string pickerName : m_aExcludedAreaPickerNames)
		{
			COE_AreaPickerBase picker = COE_AreaPickerBase.Cast(GetGame().GetWorld().FindEntityByName(pickerName));
			if (picker)
				m_aExcludedAreas.Insert(picker.GetArea());
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected vector PickPosition() { return vector.Zero; };
	
	//------------------------------------------------------------------------------------------------
	COE_LoationBase CreateInstance()
	{
		return COE_LoationBase(PickPosition(), m_fLocationRadius);
	}
}

//------------------------------------------------------------------------------------------------
class COE_LoationBase : Managed
{
	protected vector m_vCenter;
	protected float m_fRadius;
	protected ResourceName m_sMainPrefabName;
	protected ref array<IEntity> m_aStructures;
	protected ref array<AIGroup> m_aGroups;
	protected COE_Faction m_Faction;
	
	void COE_LoationBase(vector center, float radius)
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

//------------------------------------------------------------------------------------------------
class COE_PrefabLocationConfig : COE_LoationBaseConfig
{
	[Attribute(desc: "Prefabs to search for")]
	protected ref array<ResourceName> m_aPrefabsToQuery;
	protected ref array<IEntity> m_aEntitiesToPick = {};
	
	//------------------------------------------------------------------------------------------------
	void COE_PrefabLocationConfig()
	{
		if (!GetGame().InPlayMode())
			return;
		
		foreach (ResourceName prefabName : m_aPrefabsToQuery)
		{
			COE_WorldTools.GetAllPrefabEntities(m_aEntitiesToPick, prefabName);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected IEntity PickEntity()
	{
		if (m_aEntitiesToPick.IsEmpty())
			return null;
		
		return m_aEntitiesToPick[Math.RandomInt(0, m_aEntitiesToPick.Count())];
	}
	
	//------------------------------------------------------------------------------------------------
	override COE_PrefabLocation CreateInstance()
	{
		IEntity entity = PickEntity();
		if (!entity)
			return null;
		
		COE_PrefabLocation location = COE_PrefabLocation(entity.GetOrigin(), m_fLocationRadius);
		location.SetMainStructure(entity);
		return location;
	}
}

//------------------------------------------------------------------------------------------------
class COE_PrefabLocation : COE_LoationBase
{
}

//------------------------------------------------------------------------------------------------
class COE_SlotLocationConfig : COE_PrefabLocationConfig
{
	[Attribute(desc: "Prefab to spawn on slot")]
	protected ResourceName m_sPrefabToSpawn;

	//------------------------------------------------------------------------------------------------
	override COE_SlotLocation CreateInstance()
	{
		IEntity entity = PickEntity();
		if (!entity)
			return null;
		
		COE_SlotLocation location = COE_SlotLocation(entity.GetOrigin(), m_fLocationRadius);
		location.SetMainPrefabName(m_sPrefabToSpawn);
		return location;
	}
}

//------------------------------------------------------------------------------------------------
class COE_SlotLocation : COE_PrefabLocation
{
}

//------------------------------------------------------------------------------------------------
class COE_NamedLocationConfig : COE_LoationBaseConfig
{
	[Attribute(desc: "Named locations to pick from")]
	protected ref array<string> m_aNamedLocations;
	
	//------------------------------------------------------------------------------------------------
	override protected vector PickPosition()
	{
		if (m_aNamedLocations.IsEmpty())
			return vector.Zero;
		
		string locationName = m_aNamedLocations[Math.RandomInt(0, m_aNamedLocations.Count())];
		IEntity location = GetGame().GetWorld().FindEntityByName(locationName);
		if (!location)
			return vector.Zero;
		
		return location.GetOrigin();
	}
}

//------------------------------------------------------------------------------------------------
class COE_NamedLocation : COE_LoationBase
{
}

//------------------------------------------------------------------------------------------------
class COE_AreaLocationConfig : COE_LoationBaseConfig
{
	[Attribute(desc: "Names of area pickers")]
	protected ref array<string> m_aAreaPickerNames;
	protected ref array<ref COE_AreaBase> m_aAreas = {};
	
	//------------------------------------------------------------------------------------------------
	void COE_NamedLocationConfig()
	{
		if (!GetGame().InPlayMode())
			return;
		
		foreach (string pickerName : m_aAreaPickerNames)
		{
			COE_AreaPickerBase picker = COE_AreaPickerBase.Cast(GetGame().GetWorld().FindEntityByName(pickerName));
			if (picker)
				m_aAreas.Insert(picker.GetArea());
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected COE_AreaBase PickArea()
	{
		if (m_aAreas.IsEmpty())
			return null;
		
		return m_aAreas[Math.RandomInt(0, m_aAreas.Count())];
	}
	
	//------------------------------------------------------------------------------------------------
	override COE_AreaLocation CreateInstance()
	{
		COE_AreaBase area = PickArea();
		if (!area)
			return null;
		
		COE_AreaLocation location = COE_AreaLocation(vector.Zero, 0);
		location.SetArea(area);
		return location;
	}
}

//------------------------------------------------------------------------------------------------
class COE_AreaLocation : COE_LoationBase
{
	protected ref COE_AreaBase m_aArea;
	
	//------------------------------------------------------------------------------------------------
	void SetArea(COE_AreaBase area)
	{
		m_aArea = area;
	}
}

//------------------------------------------------------------------------------------------------
class COE_FreeRoamingLocationConfig : COE_LoationBaseConfig
{
	[Attribute(defvalue: "15", desc: "Radius of cylinder in meter for checking that position is empty")]
	protected float m_fEmptyPositionCylinderRadius;
	
	[Attribute(defvalue: "2", desc: "Height of cylinder in meter for checking that position is empty")]
	protected float m_fEmptyPositionCylinderHeight;
	
	[Attribute(defvalue: "5", desc: "Maximum slope angle at the position in degrees")]
	protected float m_fMaxSlopeAngle;
	
	//------------------------------------------------------------------------------------------------
	override protected vector PickPosition()
	{
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = m_fEmptyPositionCylinderRadius;
		params.EmptyHeight = m_fEmptyPositionCylinderHeight;
		params.MaxSlopeAngle = m_fMaxSlopeAngle;
		vector transform[4];
		COE_WorldTools.SampleTransformInArea(transform, m_pSampledArea, m_aExcludedAreas, params);
		
		return transform[3];
	}
		
	//------------------------------------------------------------------------------------------------
	override COE_ObjectiveBase CreateInstance()
	{
		return COE_FreeRoamingLocation(PickPosition(), m_fLocationRadius);
	}
}

//------------------------------------------------------------------------------------------------
class COE_FreeRoamingLocation : COE_LoationBase
{
}