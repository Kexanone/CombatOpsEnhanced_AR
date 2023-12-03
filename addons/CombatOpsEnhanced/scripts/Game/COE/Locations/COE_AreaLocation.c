//------------------------------------------------------------------------------------------------
class COE_AreaLocationConfig : COE_LocationBaseConfig
{
	[Attribute(desc: "Names of area pickers")]
	protected ref array<string> m_aAreaPickerNames;
	protected ref array<ref COE_AreaBase> m_aAreas = {};
	
	//------------------------------------------------------------------------------------------------
	void COE_NamedLocationConfig()
	{
		if (!GetGame().InPlayMode())
			return;
		
		foreach (string pickerName : m_aAreaPickerNames)
		{
			COE_AreaPickerBase picker = COE_AreaPickerBase.Cast(GetGame().GetWorld().FindEntityByName(pickerName));
			if (picker)
				m_aAreas.Insert(picker.GetArea());
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected COE_AreaBase PickArea()
	{
		if (m_aAreas.IsEmpty())
			return null;
		
		return m_aAreas[Math.RandomInt(0, m_aAreas.Count())];
	}
	
	//------------------------------------------------------------------------------------------------
	override COE_AreaLocation Create()
	{
		COE_AreaBase area = PickArea();
		if (!area)
			return null;
		
		COE_AreaLocation location = COE_AreaLocation(vector.Zero, 0);
		location.SetArea(area);
		return location;
	}
}

//------------------------------------------------------------------------------------------------
class COE_AreaLocation : COE_Location
{
	protected ref COE_AreaBase m_aArea;
	
	//------------------------------------------------------------------------------------------------
	void SetArea(COE_AreaBase area)
	{
		m_aArea = area;
	}
}
