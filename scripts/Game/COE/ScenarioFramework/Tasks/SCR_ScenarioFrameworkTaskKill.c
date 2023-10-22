//------------------------------------------------------------------------------------------------
//! We modify the kill task such that the task marker will follow the HVT
modded class SCR_TaskKill : SCR_ScenarioFrameworkTask
{	
	
	//------------------------------------------------------------------------------------------------
	override void OnObjectDamage(EDamageState state)
	{
		super.OnObjectDamage(state);
		
		if (state != EDamageState.DESTROYED || !m_Asset || !m_SupportEntity)
			return;
				
		GetGame().GetCallqueue().Remove(UpdateTaskMarkerPosition);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Init()
	{
		super.Init();
					
		if (!m_Asset)
			return;
		
		/*
		SCR_ScenarioFrameworkLayerTaskKill layerTaskKill = SCR_ScenarioFrameworkLayerTaskKill.Cast(m_Layer);
		if (!layerTaskKill)
			return;
		
		GetGame().GetCallqueue().CallLater(UpdateTaskMarkerPosition, 1000*layerTaskKill.GetKillMapMarkerUpdateDelay(), true);
		*/
		
		// Temporary solution until m_Layer is properly defined
		GetGame().GetCallqueue().CallLater(UpdateTaskMarkerPosition, 30000, true);
	}
	
	void UpdateTaskMarkerPosition()
	{
		if (m_Asset)
		{
			m_SupportEntity.MoveTask(m_Asset.GetOrigin(), this.GetTaskID());
		}
		else
		{
			COE_Utils.PrintError("No asset associated with task!", "SCR_TaskKill.UpdateTaskMarkerPosition");
		};
	}
}
