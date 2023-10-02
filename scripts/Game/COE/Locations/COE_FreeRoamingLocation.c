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
	override bool PickPosition(out vector pos)
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
	override COE_LocationBase Create()
	{
		vector pos;
		if (!PickPosition(pos))
			return null;
		
		return COE_FreeRoamingLocation(pos, m_fLocationRadius);
	}
}

//------------------------------------------------------------------------------------------------
class COE_FreeRoamingLocation : COE_LocationBase
{
}
