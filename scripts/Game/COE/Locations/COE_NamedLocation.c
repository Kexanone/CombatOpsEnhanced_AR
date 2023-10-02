//------------------------------------------------------------------------------------------------
class COE_NamedLocationConfig : COE_LocationBaseConfig
{
	[Attribute(desc: "Named locations to pick from")]
	protected ref array<string> m_aNamedLocations;
	
	//------------------------------------------------------------------------------------------------
	override bool PickPosition(out vector pos)
	{
		if (m_aNamedLocations.IsEmpty())
			return false;
		
		string locationName = m_aNamedLocations[Math.RandomInt(0, m_aNamedLocations.Count())];
		IEntity location = GetGame().GetWorld().FindEntityByName(locationName);
		if (!location)
			return false;
		
		pos = location.GetOrigin();
		return true;
	}
}

//------------------------------------------------------------------------------------------------
class COE_NamedLocation : COE_LocationBase
{
}
