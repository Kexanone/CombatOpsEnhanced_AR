//------------------------------------------------------------------------------------------------
class COE_AreaBase : Managed
{
	//------------------------------------------------------------------------------------------------
	bool IsPointInArea(vector pos) { return false; }
	
	//------------------------------------------------------------------------------------------------
	static vector SamplePointInArea(COE_AreaBase outer, COE_AreaBase inner)
	{
		vector pos;
		
		Print("|gog|COE_AreaBase.IsPointInArea|Start|");
		while(!pos || inner.IsPointInArea(pos))
			pos = outer.SamplePointInArea();
		Print("|gog|COE_AreaBase.IsPointInArea|End|");
		
		return pos;
	}
	
	//------------------------------------------------------------------------------------------------
	vector SamplePointInArea() { return vector.Zero; }
	
	//------------------------------------------------------------------------------------------------
	void GetBoundBox(out vector min, out vector max) {}
}

//------------------------------------------------------------------------------------------------
class COE_AreaPickerBaseClass : GenericEntityClass
{
} 

//------------------------------------------------------------------------------------------------
class COE_AreaPickerBase : GenericEntity
{
#ifdef WORKBENCH
	[Attribute(defvalue: "1", desc: "Show the debug shapes in Workbench", category: "Debug")];
	protected bool m_bShowDebugShapesInWorkbench;
	
	[Attribute(defvalue: "1.0 1.0 1.0 0.0", uiwidget: UIWidgets.ColorPicker, desc: "Color of the debug shape", category: "Debug")]
	protected ref Color m_cDebugShapeColor;
#endif
	
	protected ref COE_AreaBase m_pArea;
	
	COE_AreaBase GetArea()
	{
		return m_pArea;
	}
}