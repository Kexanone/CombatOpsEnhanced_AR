//------------------------------------------------------------------------------------------------
modded class SCR_ScenarioFrameworkSlotDelivery : SCR_ScenarioFrameworkSlotTask
{
	//------------------------------------------------------------------------------------------------
	void COE_AddAssociatedTaskLayer(string layerName)
	{
		m_aAssociatedTaskLayers.Insert(layerName);
	}
}