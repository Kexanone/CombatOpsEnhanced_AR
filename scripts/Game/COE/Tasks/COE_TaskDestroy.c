//------------------------------------------------------------------------------------------------
class COE_TaskDestroyConfig : COE_TaskBaseConfig
{
	[Attribute(desc: "Label of target prefab", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EPrefabLabel))]
	protected COE_EPrefabLabel m_iTargetPrefabLabel;
	
	//------------------------------------------------------------------------------------------------
	override void Create(COE_LocationBase location = null)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetTargetPrefabName()
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return string.Empty;
		
		COE_Faction faction = factionManager.GetEnemyFaction();
		if (!faction)
			return string.Empty;
		
		return faction.GetRandomPrefabByLabel(m_iTargetPrefabLabel);
	}
}

/*
//------------------------------------------------------------------------------------------------
class COE_TaskDestroy : COE_TaskBase
{
	static int m_iCounter = 0;
	static ref array<ResourceName> targetPrefabNames = {"{DC7B76A39454380A}Prefabs/Compositions/Installation/COE_RPL5_Radar.et", "{CC15340158615106}Prefabs/Compositions/Installation/COE_R404_Antenna.et", "{5A5D82E72D1AD220}Prefabs/Compositions/Installation/COE_Ural4320_Command.et", "{D074913C6D18C69E}Prefabs/Compositions/Installation/COE_Ural4320_Tanker.et"};
	static ref array<ResourceName> groupPrefabNames = {"{96BAB56E6558788E}Prefabs/Groups/OPFOR/Group_USSR_Team_AT.et", "{43C7A28EEB660FF8}Prefabs/Groups/OPFOR/Group_USSR_Team_GL.et", "{1C0502B5729E7231}Prefabs/Groups/OPFOR/Group_USSR_Team_Suppress.et"};
	
	protected IEntity m_pTarget;
	protected SCR_DestructionMultiPhaseComponent m_pTargetDestructionComponent;
	private const float EMPTY_POSITION_CYLINDER_RADIUS = 15;
	
	//------------------------------------------------------------------------------------------------
	void COE_TaskDestroy(COE_TaskBaseConfig config, COE_LocationBase location)
	{
		COE_EntitySpawner baseSpawner = COE_EntitySpawner.Cast(GetGame().GetWorld().FindEntityByName("CarrierSpawner"));
		if (!baseSpawner)
			return;
		
		array<IEntity> baseEntities = baseSpawner.GetSpawnedEntities();
		if (baseEntities.IsEmpty() || !baseEntities[0])
			return;
		
		COE_CircleArea sampleArea = COE_CircleArea(baseEntities[0].GetOrigin(), 7500);
		
		array<ref COE_AreaBase> excludedAreas = {};
		COE_AreaPickerBase picker = COE_AreaPickerBase.Cast(GetGame().GetWorld().FindEntityByName("AirfieldBorder"));
		if (picker)
			excludedAreas.Insert(picker.GetArea());
		
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = EMPTY_POSITION_CYLINDER_RADIUS;
		params.MaxSlopeAngle = 5;
		vector transform[4];
		COE_WorldTools.SampleTransformInArea(transform, sampleArea, excludedAreas, params);
		
		if (m_iCounter == 0 || m_iCounter >= targetPrefabNames.Count())
		{
			m_iCounter = 0;
			SCR_ArrayHelperT<ResourceName>.Shuffle(targetPrefabNames);
		};	
			
		IEntity target = COE_GameTools.SpawnStructurePrefab(targetPrefabNames[m_iCounter], transform);
		COE_GameTools.SpawnSFTaskPrefab("{982CCB885E1BAB04}Prefabs/Compositions/Tasks/COE_TaskDestroy.et", transform[3], target);
		m_iCounter++;
		
		vector targetPos = m_pTaskLayer.GetOrigin();
				
		COE_WorldTools.RemoveTreesInRadius(targetPos, EMPTY_POSITION_CYLINDER_RADIUS);
				
		sampleArea = COE_CircleArea(targetPos, 50);
		params.EmptyRadius = 5;
		params.MaxSlopeAngle = 10;
		
		array<SCR_SiteSlotEntity> slots = COE_WorldTools.QuerySiteSlotEntitiesBySphere(targetPos, 50);
		
		Math.Randomize(-1);
		for (int i = 0; i < Math.RandomIntInclusive(2, 3); i++)
		{			
			if (i < slots.Count())
			{
				slots[i].GetWorldTransform(transform);
			}
			else
			{
				COE_WorldTools.SampleTransformInArea(transform, sampleArea, excludedAreas, params);
			};
			
			COE_WorldTools.SetTransformRotation(transform, vector.Direction(targetPos, transform[3]).ToYaw());
			COE_GameTools.SpawnStructurePrefab("{114DE81321786CD9}Prefabs/Compositions/Slotted/SlotFlatSmall/MachineGunNest_S_USSR_01_PKM.et", transform);
		};
		
		AIGroup group = COE_GameTools.SpawnGroupPrefab("{657590C1EC9E27D3}Prefabs/Groups/OPFOR/Group_USSR_LightFireTeam.et", targetPos);
		group.AddWaypoint(COE_GameTools.SpawnWaypointPrefab("{2A81753527971941}Prefabs/AI/Waypoints/AIWaypoint_Defend_CP.et", targetPos));
		
		sampleArea = COE_CircleArea(targetPos, 100);
		params.EmptyRadius = 0.5;
		params.MaxSlopeAngle = 90;
		
		foreach (ResourceName groupPrefabName : groupPrefabNames)
		{
			COE_WorldTools.SampleTransformInArea(transform, sampleArea, excludedAreas, params);
			group = COE_GameTools.SpawnGroupPrefab(groupPrefabName, transform[3]);
			COE_AITasks.Patrol(group, targetPos);
		}
	}
}
*/