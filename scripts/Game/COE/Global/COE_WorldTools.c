//------------------------------------------------------------------------------------------------
class COE_SamplePosParams : Managed
{
	// Radius and height of cylinder that needs to be empty
	float EmptyRadius = 0.5;
	float EmptyHeight = 2;
	float MaxSlopeAngle = 90;
	int MaxNumTrials = -1;
}

//------------------------------------------------------------------------------------------------
class COE_WorldTools
{
	//------------------------------------------------------------------------------------------------
	static void SetTransformRotation(out vector transform[4], float rotation)
	{
		vector angles = Math3D.MatrixToAngles(transform);
		angles[0] = rotation;
		Math3D.AnglesToMatrix(angles, transform);
	}
	
	//------------------------------------------------------------------------------------------------
	protected static bool IsPositionAccepted(vector pos, array<COE_AreaBase> excludedAreas, COE_SamplePosParams params)
	{
		if (COE_Utils.SurfaceIsWater(pos))
			return false;
		
		foreach(COE_AreaBase area : excludedAreas)
		{
			if (area.IsPointInArea(pos))
				return false;
		};
		
		if (!COE_Utils.SurfaceIsFlat(pos, params.MaxSlopeAngle))
			return false;
		
		return SCR_WorldTools.TraceCylinder(pos, params.EmptyRadius, params.EmptyHeight);
	}
	
	//------------------------------------------------------------------------------------------------
	// Samples positions in a given area
	static void SampleTransformInArea(out vector transform[4], COE_AreaBase area, array<COE_AreaBase> excludedAreas, COE_SamplePosParams params = null)
	{
		if (!params)
			params = COE_SamplePosParams();
		
		Math3D.MatrixIdentity4(transform);	
		
		vector pos;
		int trialCounter = 0;
		
		while (!IsPositionAccepted(pos, excludedAreas, params) && (params.MaxNumTrials < 0 || params.MaxNumTrials > trialCounter))
		{
			pos = area.SamplePointInArea();
			pos[1] = GetGame().GetWorld().GetSurfaceY(pos[0], pos[2]);
			trialCounter++;
		};
		
		transform[3] = pos;
		SCR_TerrainHelper.SnapAndOrientToTerrain(transform);
		Math.Randomize(-1);
		SetTransformRotation(transform, Math.RandomFloat(0, 360));		
	}
	
	//------------------------------------------------------------------------------------------------
	// Samples positions in the world
	static void SampleTransformInWorld(out vector transform[4], array<COE_AreaBase> excludedAreas, COE_SamplePosParams params = null)
	{
		Math.Randomize(-1);
		
		if (!params)
			params = COE_SamplePosParams();
		
		Math3D.MatrixIdentity4(transform);
		
		vector mins, maxs;
		GetGame().GetWorld().GetBoundBox(mins, maxs);
		vector pos;
		int trialCounter = 0;
		
		while (!IsPositionAccepted(pos, excludedAreas, params) && (params.MaxNumTrials < 0 || params.MaxNumTrials > trialCounter))
		{
			pos[0] = Math.RandomFloat(mins[0], maxs[0]);
			pos[2] = Math.RandomFloat(mins[2], maxs[2]);
			pos[1] = GetGame().GetWorld().GetSurfaceY(pos[0], pos[2]);
			trialCounter++;
		}
		
		transform[3] = pos;
		SCR_TerrainHelper.SnapAndOrientToTerrain(transform);
		SetTransformRotation(transform, Math.RandomFloat(0, 360));			
	}
	
	//------------------------------------------------------------------------------------------------
	protected static ref array<SCR_SiteSlotEntity> m_aQueriedSlots;
	
	//------------------------------------------------------------------------------------------------
	static array<SCR_SiteSlotEntity> QuerySiteSlotEntitiesBySphere(vector center, float radius)
	{
		m_aQueriedSlots = {};
		GetGame().GetWorld().QueryEntitiesBySphere(center, radius, QuerySiteSlotEntitiesCallback);
		return m_aQueriedSlots;
	}
	
	//------------------------------------------------------------------------------------------------
	protected static bool QuerySiteSlotEntitiesCallback(IEntity entity)
	{
		SCR_SiteSlotEntity slot = SCR_SiteSlotEntity.Cast(entity);
		
		if (slot)
			m_aQueriedSlots.Insert(slot);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	static void RemoveTreesInRadius(vector center, float radius)
	{
		GetGame().GetWorld().QueryEntitiesBySphere(center, radius, TreeRemovalCallback);
	}
	
	//------------------------------------------------------------------------------------------------
	protected static bool TreeRemovalCallback(IEntity entity)
	{
		if (Tree.Cast(entity))
			SCR_EntityHelper.DeleteEntityAndChildren(entity);
		
		return true;
	}
}