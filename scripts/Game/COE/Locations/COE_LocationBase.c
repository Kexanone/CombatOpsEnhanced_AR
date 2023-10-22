//------------------------------------------------------------------------------------------------
class COE_LocationBaseConfig : ScriptAndConfig
{
#ifdef WORKBENCH
	[Attribute()]
	protected string m_sComment;
#endif
	
	[Attribute(defvalue: "1", desc: "Whether this location config is enabled")]
	protected bool m_bEnabled;
	
	[Attribute(defvalue: "1", desc: "The larger the weight, the more likely this config is selected")]
	protected int m_bWeight;
	
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
	protected COE_Faction m_Faction;
	
	//------------------------------------------------------------------------------------------------
	bool IsEnabled()
	{
		return m_bEnabled;
	}
	
	//------------------------------------------------------------------------------------------------
	bool GetWeight()
	{
		return m_bWeight;
	}
	
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
		
		m_Faction = factionManager.GetEnemyFaction();
	}
	
	void AddExcludedArea(COE_AreaBase area)
	{
		m_aExcludedAreas.Insert(area);
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool PickPosition(out vector pos, out IEntity associatedStructure) { return false; }
	
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
	COE_Location Create()
	{
		vector pos;
		IEntity associatedEntity;
		if (!PickPosition(pos, associatedEntity))
			return null;
		
		COE_Location location = COE_Location(pos, m_fLocationRadius);
		
		if (associatedEntity)
			location.SetMainStructure(associatedEntity);
				
		return location;
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
class COE_Location : Managed
{
	protected vector m_vCenter;
	protected float m_fRadius;
	protected ResourceName m_sMainPrefabName;
	protected ref array<IEntity> m_aStructures = {};
	protected ref array<IEntity> m_aEntitiesMarkedForCleanUp = {};
	protected ref array<AIGroup> m_aGroupsMarkedForCleanUp = {};
	protected COE_Faction m_Faction;
	protected ref array<SCR_SiteSlotEntity> m_aFlatSlots = {};
	protected ref array<SCR_SiteSlotEntity> m_aRoadSlots = {};
	protected ref array<COE_AISlotConfig> m_aRiflemanSlots = {};
	protected ref array<COE_AISlotConfig> m_aMarksmanSlots = {};
	protected ref array<BaseCompartmentSlot> m_aTurretSeats = {};
	
	static const float ROAD_BLOCK_RADIUS_EXTENSION = 100;
	
	//------------------------------------------------------------------------------------------------
	void COE_Location(vector center, float radius)
	{
		m_vCenter = center;
		m_fRadius = radius;
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		m_Faction = factionManager.GetEnemyFaction();
	}
	
	//------------------------------------------------------------------------------------------------
	vector GetCenter()
	{
		return m_vCenter;
	}
	
	//------------------------------------------------------------------------------------------------
	float GetRadius()
	{
		return m_fRadius;
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
	void AddStructure(IEntity structure)
	{
		m_aStructures.Insert(structure);
	};
	
	//------------------------------------------------------------------------------------------------
	array<IEntity> GetStructures()
	{
		return m_aStructures;
	}
	
	//------------------------------------------------------------------------------------------------
	void MarkGroupForCleanUp(AIGroup group)
	{
		m_aGroupsMarkedForCleanUp.Insert(group);
	}
	
	//------------------------------------------------------------------------------------------------
	void MarkEntityForCleanUp(IEntity entity)
	{
		m_aEntitiesMarkedForCleanUp.Insert(entity);
	}
	
	//------------------------------------------------------------------------------------------------
	void ScheduleCleanUp()
	{
		array<int> playerIds = {};
		GetGame().GetPlayerManager().GetPlayers(playerIds);
		
		foreach (int playerId : playerIds)
		{
			IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
			if (!player)
				continue;
			
			if (vector.DistanceXZ(m_vCenter, player.GetOrigin()) > 1000)
				continue;
			
			// There are still players nearby => Delay clean-up
			GetGame().GetCallqueue().CallLater(ScheduleCleanUp, 60000);
			return;
		};
		
		// All players have left => Start clean-up
		foreach (IEntity entity : m_aEntitiesMarkedForCleanUp)
			SCR_EntityHelper.DeleteEntityAndChildren(entity);
		
		foreach (AIGroup group : m_aGroupsMarkedForCleanUp)
			SCR_EntityHelper.DeleteEntityAndChildren(group);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SpawnAIAtStructures()
	{
		array<COE_AISlotConfig> configs = {};
		
		foreach (IEntity structure : m_aStructures)
		{
			COE_AISlotsComponent slotsComponent = COE_AISlotsComponent.Cast(structure.FindComponent(COE_AISlotsComponent));
			if (!slotsComponent)
				continue;
			
			configs.InsertAll(slotsComponent.GetSlotConfigs(COE_EEntityLabel.RIFLEMAN, true));
		};
		
		if (configs.IsEmpty())
			return;
				
		int maxCount = Math.Min(4, configs.Count());
		SCR_ArrayHelperT<COE_AISlotConfig>.Shuffle(configs);
			
		for (int i = 0; i < maxCount; i++)
		{
			IEntity unit = configs[i].Spawn();	
			MarkEntityForCleanUp(unit);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SpawnRoadBlocks()
	{
		Math.Randomize(-1);
		int minCount = Math.Min(2, m_aRoadSlots.Count());
		int maxCount = Math.Min(3, m_aRoadSlots.Count());
		int count = Math.RandomIntInclusive(minCount, maxCount);
		
		COE_ArrayHelperT<SCR_SiteSlotEntity>.Shuffle(m_aRoadSlots);
		
		for (int i = 0; i < count; i++)
		{
			COE_EEntityLabel label;
			
			SCR_SiteSlotEntity slot = m_aRoadSlots[i];
			switch (GetSiteSlotLabel(slot))
			{
				case EEditableEntityLabel.SLOT_ROAD_SMALL: {label = COE_EEntityLabel.SMALL_ROAD_SLOT; break;};
				case EEditableEntityLabel.SLOT_ROAD_MEDIUM: {label = COE_EEntityLabel.MEDIUM_ROAD_SLOT; break;};
				case EEditableEntityLabel.SLOT_ROAD_LARGE: {label = COE_EEntityLabel.LARGE_ROAD_SLOT; break;};
			}
			IEntity roadblock = slot.SpawnEntityInSlot(Resource.Load(m_Faction.GetRandomPrefabByLabel(label)));
			
			if (!roadblock.FindComponent(SCR_AISmartActionComponent))
				continue;
			
			AIWaypoint wp = COE_GameTools.SpawnWaypointPrefab("{B2F9598C9DEFE645}Prefabs/AI/Waypoints/COE_AIWaypoint_Defend_10m.et", roadblock.GetOrigin());
			AIGroup group = COE_GameTools.SpawnGroupPrefab(m_Faction.GetRandomPrefabByLabel(COE_EEntityLabel.CHECKPOINT_GROUP), roadblock.GetOrigin());
			group.AddWaypoint(wp);
			MarkGroupForCleanUp(group);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	void SpawnAI()
	{
		Math.Randomize(-1);
		
		if (m_aRiflemanSlots.IsEmpty())
		{
			GetGame().GetWorld().QueryEntitiesBySphere(m_vCenter, m_fRadius, QuerySlotsCallback);
			GetGame().GetWorld().QueryEntitiesBySphere(m_vCenter, m_fRadius + ROAD_BLOCK_RADIUS_EXTENSION, QueryRoadSlotsCallback);
		};
		
		// Spawn marksmen
		if (!m_aMarksmanSlots.IsEmpty())
		{
			int maxCount = Math.RandomIntInclusive(2, Math.Min(3, m_aMarksmanSlots.Count()));
			SCR_ArrayHelperT<COE_AISlotConfig>.Shuffle(m_aMarksmanSlots);
			
			for (int i = 0; i < maxCount; i++)
			{
				IEntity unit = m_aMarksmanSlots[i].Spawn();
				MarkEntityForCleanUp(unit);
			};		
		};
		
		// Spawn units at structures
		if (!m_aStructures.IsEmpty())
		{
			SpawnAIAtStructures();
		};
		
		// Spawn road blocks and add occupants to all turrets
		SpawnRoadBlocks();
		GetGame().GetWorld().QueryEntitiesBySphere(m_vCenter, m_fRadius + ROAD_BLOCK_RADIUS_EXTENSION, SpawnTurretOccupantsCallback);
		
		COE_CircleArea sampleArea = COE_CircleArea(m_vCenter, m_fRadius);
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = 0.5;
		params.MaxSlopeAngle = 90;
		vector transform[4];
		
		array<ResourceName> groupNames = m_Faction.GetPrefabsByLabel(COE_EEntityLabel.PATROL_GROUP);
		SCR_ArrayHelperT<ResourceName>.Shuffle(groupNames);
		
		for (int i = 0; i < 3; i++)
		{
			COE_WorldTools.SampleTransformInArea(transform, sampleArea, {}, params);
			AIGroup group = COE_GameTools.SpawnGroupPrefab(groupNames[i], transform[3]);
			COE_AITasks.Patrol(group, m_vCenter, m_fRadius);
			MarkGroupForCleanUp(group);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity GetRandomFlatSlot()
	{
		if (m_aFlatSlots.IsEmpty())
			GetGame().GetWorld().QueryEntitiesBySphere(m_vCenter, m_fRadius, QuerySlotsCallback);
		
		Math.Randomize(-1);
		
		while (true)
		{
			if (m_aFlatSlots.IsEmpty())
				return null;
			
			 SCR_SiteSlotEntity slot = SCR_SiteSlotEntity.Cast(m_aFlatSlots[Math.RandomInt(0, m_aFlatSlots.Count())]);
			
			if (!slot.IsOccupied())
				return slot;
			
			m_aFlatSlots.RemoveItem(slot);
		}
		
		return null;
	}
	
	EEditableEntityLabel GetSiteSlotLabel(SCR_SiteSlotEntity slot)
	{
		SCR_EditableEntityComponent editableComponent = SCR_EditableEntityComponent.Cast(slot.FindComponent(SCR_EditableEntityComponent));
		if (!editableComponent)
			return EEditableEntityLabel.NONE;
		
		SCR_EditableEntityUIInfo editableEntityUIInfo = SCR_EditableEntityUIInfo.Cast(editableComponent.GetInfo());
		if (!editableEntityUIInfo)
			return EEditableEntityLabel.NONE;
		
		array<EEditableEntityLabel> labels = {};
		editableEntityUIInfo.GetEntityLabels(labels);
		
		if (labels.IsEmpty())
			return EEditableEntityLabel.NONE;
		
		return labels[0];
	}
	
	//------------------------------------------------------------------------------------------------
	bool QuerySlotsCallback(IEntity entity)
	{
		SCR_SiteSlotEntity slot = SCR_SiteSlotEntity.Cast(entity);
		
		if (!slot)
		{
			COE_AISlotsComponent aiSlot = COE_AISlotsComponent.Cast(entity.FindComponent(COE_AISlotsComponent));
			if (!aiSlot)
				return true;
			
			m_aRiflemanSlots.InsertAll(aiSlot.GetSlotConfigs(COE_EEntityLabel.RIFLEMAN));
			return true;
		};
		
		if (slot.IsOccupied())
			return true;
		
		EEditableEntityLabel label = GetSiteSlotLabel(slot);
		
		if (label == EEditableEntityLabel.SLOT_FLAT_SMALL || label == EEditableEntityLabel.SLOT_FLAT_MEDIUM)
			m_aFlatSlots.Insert(slot);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	bool QueryRoadSlotsCallback(IEntity entity)
	{
		SCR_SiteSlotEntity slot = SCR_SiteSlotEntity.Cast(entity);
		
		if (!slot)
		{
			COE_AISlotsComponent aiSlot = COE_AISlotsComponent.Cast(entity.FindComponent(COE_AISlotsComponent));
			if (!aiSlot)
				return true;
			
			m_aMarksmanSlots.InsertAll(aiSlot.GetSlotConfigs(COE_EEntityLabel.MARKSMAN));
			return true;
		};
		
		if (slot.IsOccupied())
			return true;
		
		EEditableEntityLabel label = GetSiteSlotLabel(slot);
		
		if (label == EEditableEntityLabel.SLOT_ROAD_SMALL || label == EEditableEntityLabel.SLOT_ROAD_MEDIUM || label == EEditableEntityLabel.SLOT_ROAD_LARGE)
			m_aRoadSlots.Insert(slot);
		
		return true;
	}
		
	//------------------------------------------------------------------------------------------------
	bool SpawnTurretOccupantsCallback(IEntity entity)
	{
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(entity.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return true;
				
		compartmentManager.SpawnDefaultOccupants({ECompartmentType.Turret});
		return true;
	}
}
