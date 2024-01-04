class COE_WorldArea : COE_AreaBase
{
	vector m_vMins, m_vMaxs;
	
	//------------------------------------------------------------------------------------------------
	void COE_WorldArea()
	{
		GetGame().GetWorld().GetBoundBox(m_vMins, m_vMaxs);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsPointInArea(vector pos)
	{
		for (int i = 0; i < 3; i++)
		{
			if (pos[i] < m_vMins[i] || pos[i] > m_vMaxs[i])
				return false;
		};
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override vector SamplePointInArea()
	{
		float x, z;
		SCR_Math2D.GetRandomPointInRectangle(m_vMins[0], m_vMaxs[0], m_vMins[2], m_vMaxs[2], x, z);
		return Vector(x, 0, z);
	}
	
	//------------------------------------------------------------------------------------------------
	override void GetBoundBox(out vector min, out vector max)
	{
		min = m_vMins;
		max = m_vMaxs;
	}
}
