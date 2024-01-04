class COE_CircleArea : COE_AreaBase
{
	protected vector m_vCenter;
	protected float m_fRadius;
	protected float m_fRadiusSq;
	
	//------------------------------------------------------------------------------------------------
	void COE_CircleArea(vector center, float radius)
	{
		m_vCenter = center;
		SetRadius(radius);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsPointInArea(vector pos)
	{
		return vector.DistanceSqXZ(m_vCenter, pos) <= m_fRadiusSq;
	}
	
	//------------------------------------------------------------------------------------------------
	override vector SamplePointInArea()
	{
		return SCR_Math2D.GenerateRandomPointInRadius(0, m_fRadius, m_vCenter);
	}
	
	override void GetBoundBox(out vector min, out vector max)
	{
		vector world_mins, world_maxs;
		GetGame().GetWorld().GetBoundBox(world_mins, world_maxs);
		vector offset = {m_fRadius, 0, m_fRadius};
		min = m_vCenter - offset;
		min[1] = world_mins[1];
		max = m_vCenter + offset;
		max[1] = world_maxs[1];
	}
	
	//------------------------------------------------------------------------------------------------
	void SetRadius(float radius)
	{
		m_fRadius = radius;
		m_fRadiusSq = Math.Pow(radius, 2);
	}
	
	//------------------------------------------------------------------------------------------------
	float GetRadius()
	{
		return m_fRadius;
	}
	
	//------------------------------------------------------------------------------------------------
	vector GetCenter()
	{
		return m_vCenter;
	}
}

/*
//------------------------------------------------------------------------------------------------
class COE_CircleAreaPickerClass : COE_AreaPickerBaseClass
{
} 

//------------------------------------------------------------------------------------------------
class COE_CircleAreaPicker : COE_AreaPickerBase
{
	[Attribute("50", UIWidgets.EditBox, desc: "Radius of the area")]
	protected float m_fRadius;
	protected float m_fRadiusSq;
	
	//------------------------------------------------------------------------------------------------
	void COE_CircleArea(IEntitySource src, IEntity parent)
	{
		m_fRadiusSq = Math.Pow(m_fRadius, 2);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsPointInArea(vector pos)
	{
		return vector.DistanceSqXZ(GetOrigin(), pos) <= m_fRadiusSq;
	}
	
	//------------------------------------------------------------------------------------------------
	override vector SamplePointInArea()
	{
		return SCR_Math2D.GenerateRandomPointInRadius(0, m_fRadius, GetOrigin());
	}
	
	void SetRadius(float radius)
	{
		m_fRadius = radius;
		m_fRadiusSq = Math.Pow(radius, 2);
	}
}
*/