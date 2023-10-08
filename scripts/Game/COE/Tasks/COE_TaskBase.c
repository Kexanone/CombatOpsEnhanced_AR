//------------------------------------------------------------------------------------------------
class COE_TaskBaseConfig : ScriptAndConfig
{
	[Attribute(defvalue: "1", desc: "Whether this task is enabled")]
	protected bool m_bEnabled;
	
	[Attribute(desc: "Types of the task", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_ETaskType))]
	protected COE_ETaskType m_iTaskType;
	
	[Attribute(desc: "Name of the Scenario Workflow task prefab")]
	protected ResourceName m_sSFTaskPrefabName;
	
	protected COE_Faction m_EnemyFaction;
	
	//------------------------------------------------------------------------------------------------
	bool IsEnabled()
	{
		return m_bEnabled;
	}
	
	//------------------------------------------------------------------------------------------------
	void Init()
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		m_EnemyFaction = factionManager.GetEnemyFaction();
	}
	
	//------------------------------------------------------------------------------------------------
	COE_ETaskType GetTaskType()
	{
		return m_iTaskType;
	}
	
	//------------------------------------------------------------------------------------------------
	void Create(COE_Location location = null)
	{
		if (location)
			SpawnSFTaskPrefab(location.GetCenter());
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
