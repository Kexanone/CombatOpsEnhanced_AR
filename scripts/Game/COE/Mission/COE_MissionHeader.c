class COE_MissionHeader : SCR_MissionHeader
{
	[Attribute(desc: "Opposing factions configurations. Randomly chosen if multiple are given.")]
	ref array<ref COE_OpposingFactionsConfig> m_aCOE_OpposingFactionsConfigs;
	
	[Attribute(defvalue: "3", desc: "Total number of objectives to be generated.")]
	string m_iCOE_NumberOfObjectives;
}
