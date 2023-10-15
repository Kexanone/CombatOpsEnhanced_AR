//------------------------------------------------------------------------------------------------
modded class SCR_ScenarioFrameworkSlotBase : SCR_ScenarioFrameworkLayerBase
{
	protected IEntity m_pCOE_asset;
	
	//------------------------------------------------------------------------------------------------
	void COE_SetAsset(IEntity entity)
	{
		m_pCOE_asset = entity;
	}
		
	//------------------------------------------------------------------------------------------------
	override IEntity SpawnAsset()
	{
		if (m_pCOE_asset)
			return m_pCOE_asset;
		else
			return super.SpawnAsset();
	}
}