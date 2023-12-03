//------------------------------------------------------------------------------------------------
modded class SCR_TaskDestroyObject : SCR_ScenarioFrameworkTask
{
	//------------------------------------------------------------------------------------------------
	override void Init()
	{
		super.Init();
					
		if (!m_Asset)
			return;		
		ScriptedDamageManagerComponent objectDmgManager = ScriptedDamageManagerComponent.Cast(m_Asset.FindComponent(ScriptedDamageManagerComponent));
		
		// Set subjet to child if parent is not destructible
		if (!objectDmgManager)
		{
			m_Asset = m_Asset.GetChildren();
			objectDmgManager = ScriptedDamageManagerComponent.Cast(m_Asset.FindComponent(ScriptedDamageManagerComponent));
		};
		
		if (objectDmgManager)
			objectDmgManager.GetOnDamageStateChanged().Insert(OnObjectDamage);
	}
}
