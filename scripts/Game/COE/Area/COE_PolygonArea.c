//------------------------------------------------------------------------------------------------
class COE_PolygonArea : COE_AreaBase
{
	protected ref array<float> m_aPolygon;
	
	//------------------------------------------------------------------------------------------------
	void COE_PolygonArea(array<float> polygon)
	{
		m_aPolygon = polygon;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsPointInArea(vector pos)
	{
		return Math2D.IsPointInPolygon(m_aPolygon, pos[0], pos[2]);
	}
	
	//------------------------------------------------------------------------------------------------
	override vector SamplePointInArea()
	{
		float x, z;
		SCR_Math2D.GetRandomPointInPolygon(m_aPolygon, x, z);		
		return Vector(x, 0, z);
	}
}


//------------------------------------------------------------------------------------------------
class COE_PolygonAreaPickerClass : COE_AreaPickerBaseClass
{
} 

//------------------------------------------------------------------------------------------------
class COE_PolygonAreaPicker : COE_AreaPickerBase
{
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP1;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP2;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP3;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP4;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP5;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP6;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP7;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP8;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP9;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP10;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP11;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP12;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP13;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP14;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP15;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Position of polygon point", params: "inf inf 0 purpose=coords space=entity", category: "Points")]
	protected vector m_vP16;
	
#ifdef WORKBENCH
	protected int m_iDebugPointCount = 0;
	protected vector m_aDebugPoints[17];
#endif
		
	//------------------------------------------------------------------------------------------------
	void COE_PolygonAreaPicker(IEntitySource src, IEntity parent)
	{
		array<float> polygon = {};
		
		foreach (vector relPoint : {m_vP1, m_vP2, m_vP3, m_vP4, m_vP5, m_vP6, m_vP7, m_vP8, m_vP9, m_vP10, m_vP11, m_vP12, m_vP13, m_vP14, m_vP15, m_vP16})
		{
			if (relPoint == vector.Zero)
				break;
			
			vector point = CoordToParent(relPoint);
			polygon.Insert(point[0]);
			polygon.Insert(point[2]);
			
		#ifdef WORKBENCH
			m_aDebugPoints[m_iDebugPointCount] = point;
			m_iDebugPointCount++;
		#endif
		};
		
		m_pArea = COE_PolygonArea(polygon);
		
	#ifdef WORKBENCH
		// Close the path
		m_aDebugPoints[m_iDebugPointCount] = m_aDebugPoints[0];
		m_iDebugPointCount++;
	#endif
	}
	
#ifdef WORKBENCH
	//------------------------------------------------------------------------------------------------
	protected void DrawDebugShape(bool draw)
	{
		if (!draw)
			return;
						
		if (!m_iDebugPointCount)
			return;
		
		Shape.CreateLines(
			m_cDebugShapeColor.PackToInt(),
			ShapeFlags.ONCE | ShapeFlags.NOZWRITE,
			m_aDebugPoints,
			m_iDebugPointCount
		);
	};
	
	//------------------------------------------------------------------------------------------------
	override void _WB_AfterWorldUpdate(float timeSlice)
	{
		DrawDebugShape(m_bShowDebugShapesInWorkbench);
	};
	
	//------------------------------------------------------------------------------------------------
	override bool _WB_OnKeyChanged(BaseContainer src, string key, BaseContainerList ownerContainers, IEntity parent)
	{
		if (key == "m_bShowDebugShapesInWorkbench")
			DrawDebugShape(m_bShowDebugShapesInWorkbench);
		
		return false;
	};
#endif
}