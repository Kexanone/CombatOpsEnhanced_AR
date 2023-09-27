//------------------------------------------------------------------------------------------------
class COE_ObjectiveDestroyRadarType : COE_ObjectiveBaseType
{
	//------------------------------------------------------------------------------------------------
	override COE_ObjectiveBase CreateInstance()
	{
		return COE_ObjectiveDestroyRadar();
	}
}

//------------------------------------------------------------------------------------------------
class COE_ObjectiveDestroyRadar : COE_ObjectiveBase
{
	protected IEntity m_pTarget;
	protected SCR_DestructionMultiPhaseComponent m_pTargetDestructionComponent;
	private const float EMPTY_POSITION_CYLINDER_RADIUS = 15;
	
	//------------------------------------------------------------------------------------------------
	void COE_ObjectiveDestroyRadar()
	{
		array<COE_AreaBase> excludedAreas = {};
		COE_AreaPickerBase picker = COE_AreaPickerBase.Cast(GetGame().GetWorld().FindEntityByName("AirfieldBorder"));
		if (picker)
			excludedAreas.Insert(picker.GetArea());
		
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = EMPTY_POSITION_CYLINDER_RADIUS;
		params.MaxSlopeAngle = 5;
		vector transform[4];
		COE_WorldTools.SampleTransformInWorld(transform, excludedAreas, params);
		IEntity target = COE_GameTools.SpawnStructurePrefab("{DC7B76A39454380A}Prefabs/Compositions/Installation/COE_RPL5_Radar.et", transform);
		SetTaskLayer(COE_GameTools.SpawnSFTaskPrefab("{982CCB885E1BAB04}Prefabs/Compositions/Tasks/COE_TaskDestroy.et", transform[3], target));
		
		vector targetPos = m_pTaskLayer.GetOrigin();
				
		COE_WorldTools.RemoveTreesInRadius(targetPos, EMPTY_POSITION_CYLINDER_RADIUS);
				
		COE_CircleArea sampleArea = COE_CircleArea(targetPos, 50);
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
		
		SCR_AIGroup group = COE_GameTools.SpawnGroupPrefab("{657590C1EC9E27D3}Prefabs/Groups/OPFOR/Group_USSR_LightFireTeam.et", targetPos);
		group.AddWaypoint(COE_GameTools.SpawnWaypointPrefab("{2A81753527971941}Prefabs/AI/Waypoints/AIWaypoint_Defend_CP.et", targetPos));
	}
}
