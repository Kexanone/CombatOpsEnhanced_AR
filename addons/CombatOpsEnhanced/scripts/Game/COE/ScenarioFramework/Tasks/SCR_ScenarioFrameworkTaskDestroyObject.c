//------------------------------------------------------------------------------------------------
modded class SCR_TaskDestroyObject : SCR_ScenarioFrameworkTask
{
	//------------------------------------------------------------------------------------------------
	override void Init()
	{
		super.Init();
					
		if (!m_Asset)
			return;		
		SCR_DamageManagerComponent objectDmgManager = SCR_DamageManagerComponent.Cast(m_Asset.FindComponent(SCR_DamageManagerComponent));
		
		// Set subjet to child if parent is not destructible
		if (!objectDmgManager)
		{
			m_Asset = m_Asset.GetChildren();
			objectDmgManager = SCR_DamageManagerComponent.Cast(m_Asset.FindComponent(SCR_DamageManagerComponent));
		};
		
		if (objectDmgManager)
			objectDmgManager.GetOnDamageStateChanged().Insert(OnObjectDamage);
	}
}
