//------------------------------------------------------------------------------------------------
class COE_NamedLocationConfig : COE_LocationBaseConfig
{
	[Attribute(desc: "Named locations to pick from")]
	protected ref array<string> m_aNamedLocations;
	
	//------------------------------------------------------------------------------------------------
	override bool PickPosition(out vector pos, out IEntity associatedStructure)
	{
		while (!m_aNamedLocations.IsEmpty())
		{
			string locationName = m_aNamedLocations[Math.RandomInt(0, m_aNamedLocations.Count())];
			IEntity location = GetGame().GetWorld().FindEntityByName(locationName);		
			pos = location.GetOrigin();
			
			if (IsPositionAccepted(pos))
				return true;
			
			m_aNamedLocations.RemoveItem(locationName);
		};
		
		return false;
	}
}
