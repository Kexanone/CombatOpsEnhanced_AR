class COE_MissionHeader : SCR_MissionHeader
{
	[Attribute("US", UIWidgets.EditBox, "Playable faction name. Randomly chosen if left empty.")]
	string m_sCOE_PlayerFactionKey;
	
	[Attribute("3", UIWidgets.EditBox, "Total number of objectives to be generated.")]
	string m_iCOE_NumberOfObjectives;
};
