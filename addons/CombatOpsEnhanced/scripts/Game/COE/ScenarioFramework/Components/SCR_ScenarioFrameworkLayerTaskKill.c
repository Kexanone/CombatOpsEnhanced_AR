//------------------------------------------------------------------------------------------------
//! Modify kill task to add an update time for the HVT marker
modded class SCR_ScenarioFrameworkLayerTaskKill : SCR_ScenarioFrameworkLayerTask
{	
	[Attribute(defvalue: "30", desc: "Delay in seconds for kill map marker to update", category: "Task")]
	protected int m_iKillMapMarkerUpdateDelay;
	
	//------------------------------------------------------------------------------------------------
	int GetKillMapMarkerUpdateDelay()
	{
		return m_iKillMapMarkerUpdateDelay;
	}	
}
