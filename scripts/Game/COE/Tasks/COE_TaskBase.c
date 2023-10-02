//------------------------------------------------------------------------------------------------
class COE_TaskBaseConfig : ScriptAndConfig
{
	[Attribute(desc: "Types of the task", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_ETaskType))]
	protected COE_ETaskType m_iTaskType;
	
	[Attribute(desc: "Name of the Scenario Workflow task prefab")]
	protected ResourceName m_sSFTaskPrefabName;
	
	//------------------------------------------------------------------------------------------------
	COE_ETaskType GetTaskType()
	{
		return m_iTaskType;
	}
	
	//------------------------------------------------------------------------------------------------
	void Create(COE_LocationBase location = null)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SpawnSFTaskPrefab(vector pos, IEntity targetEntity = null)
	{
		vector transform[4];
		Math3D.MatrixIdentity4(transform);
		transform[3] = pos;
		IEntity layer = COE_GameTools.SpawnPrefab(m_sSFTaskPrefabName, transform);
		IEntity slot = layer.GetChildren();
		
		if (targetEntity && slot)
		{
			SCR_ScenarioFrameworkLayerBase slotSFComponent = SCR_ScenarioFrameworkLayerBase.Cast(slot.FindComponent(SCR_ScenarioFrameworkLayerBase));
			slotSFComponent.SetEntity(targetEntity);
		};
			
		SCR_ScenarioFrameworkLayerBase layerSFComponent = SCR_ScenarioFrameworkLayerBase.Cast(layer.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (!layerSFComponent)
			return;
		
		layerSFComponent.Init();
	}
}
