//------------------------------------------------------------------------------------------------
class COE_FreeRoamingLocationConfig : COE_LocationBaseConfig
{
	[Attribute(defvalue: "15", desc: "Radius of cylinder in meter for checking that position is empty")]
	protected float m_fEmptyPositionCylinderRadius;
	
	[Attribute(defvalue: "2", desc: "Height of cylinder in meter for checking that position is empty")]
	protected float m_fEmptyPositionCylinderHeight;
	
	[Attribute(defvalue: "5", desc: "Maximum slope angle at the position in degrees")]
	protected float m_fMaxSlopeAngle;
	
	//------------------------------------------------------------------------------------------------
	override bool PickPosition(out vector pos, out IEntity associatedStructure)
	{
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = m_fEmptyPositionCylinderRadius;
		params.EmptyHeight = m_fEmptyPositionCylinderHeight;
		params.MaxSlopeAngle = m_fMaxSlopeAngle;
		vector transform[4];
		COE_WorldTools.SampleTransformInArea(transform, m_pSampledArea, m_aExcludedAreas, params);
		pos = transform[3];
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override COE_Location Create()
	{
		vector pos;
		IEntity associatedEntity;
		if (!PickPosition(pos, associatedEntity))
			return null;
		
		COE_Location location = COE_FreeRoamingLocation(pos, m_fLocationRadius);
		
		if (associatedEntity)
			location.SetMainStructure(associatedEntity);
				
		return location;
	}
}

//------------------------------------------------------------------------------------------------
class COE_FreeRoamingLocation : COE_Location
{
	override void SpawnAI()
	{
		// Reset center to structure pos
		m_vCenter = GetMainStructure().GetOrigin();
		COE_CircleArea sampleArea = COE_CircleArea(m_vCenter, 50);
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = 5;
		params.MaxSlopeAngle = 10;
		array<SCR_SiteSlotEntity> slots = COE_WorldTools.QuerySiteSlotEntitiesBySphere(m_vCenter, 50);
		Math.Randomize(-1);
		int count = Math.RandomIntInclusive(2, 3);
		
		for (int i = 0; i < count; i++)
		{
			vector transform[4];
			
			if (i < slots.Count())
			{
				slots[i].GetWorldTransform(transform);
			}
			else
			{
				COE_WorldTools.SampleTransformInArea(transform, sampleArea, {}, params);
			};
			
			COE_WorldTools.SetTransformRotation(transform, vector.Direction(m_vCenter, transform[3]).ToYaw());
			COE_GameTools.SpawnStructurePrefab("{114DE81321786CD9}Prefabs/Compositions/Slotted/SlotFlatSmall/MachineGunNest_S_USSR_01_PKM.et", transform);
		};
		
		super.SpawnAI();
	}
}