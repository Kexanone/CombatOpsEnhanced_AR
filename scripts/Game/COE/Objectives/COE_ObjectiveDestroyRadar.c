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
		excludedAreas.Insert(picker.GetArea());
		
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = EMPTY_POSITION_CYLINDER_RADIUS;
		params.MaxSlopeAngle = 5;
		vector transform[4];
		COE_WorldTools.SampleTransformInWorld(transform, excludedAreas, params);
		m_pTarget = COE_GameTools.SpawnStructurePrefab("{DC7B76A39454380A}Prefabs/Compositions/Installation/COE_RPL5_Radar.et", transform);
		vector targetPos = m_pTarget.GetOrigin();
		IEntity antenna = m_pTarget.GetChildren();
				
		COE_WorldTools.RemoveTreesInRadius(targetPos, EMPTY_POSITION_CYLINDER_RADIUS);
				
		COE_CircleArea sampleArea = COE_CircleArea(targetPos, 50);
		params.EmptyRadius = 5;
		params.MaxSlopeAngle = 10;
		
		array<SCR_SiteSlotEntity> slots = COE_WorldTools.QuerySiteSlotEntitiesBySphere(targetPos, 50);
		
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
		
		CreateTask(antenna);
	}
	
	//------------------------------------------------------------------------------------------------
	void CreateTask(IEntity target)
	{
		m_pTask = GetTaskManager().SpawnTask("{3AD0A1257870E6C7}Prefabs/Tasks/CustomTask.et");
		m_pTask.SetOrigin(target.GetOrigin());
		m_pTask.SetTargetFaction(GetGame().GetFactionManager().GetFactionByKey("US"));
		m_pTask.SetTitle("Destroy Radar");
		m_pTask.SetDescription("Destroy the enemy radar installation");
		m_pTask.COE_SetIconName("Icon_Task_Guard");
		m_pTask.Create();
		
		// Complete task when target is destroyed
		m_pTargetDestructionComponent = SCR_DestructionMultiPhaseComponent.Cast(target.FindComponent(SCR_DestructionMultiPhaseComponent));
		if (m_pTargetDestructionComponent)
			m_pTargetDestructionComponent.GetOnDamageInvoker().Insert(OnTargetDamage);
	}
	
	//------------------------------------------------------------------------------------------------
	// Marks task as completed when target is destroyed
	void OnTargetDamage()
	{
		if (!m_pTargetDestructionComponent.GetDestroyed() || !m_pTarget)
			return;
		
		m_pTargetDestructionComponent.GetOnDamageInvoker().Remove(OnTargetDamage);
		
		m_pTask.Finish();
		m_OnObjectiveCompleted.Invoke();
	}
}
