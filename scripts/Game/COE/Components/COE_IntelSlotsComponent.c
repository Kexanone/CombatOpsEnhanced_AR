//------------------------------------------------------------------------------------------------
class COE_IntelSlotConfig : ScriptAndConfig
{
#ifdef WORKBENCH
	IEntity m_pPreviewEntity;
#endif
	
	[Attribute(defvalue:"0 0 0", UIWidgets.EditBox, desc: "Position of the intel", params: "inf inf 0 purpose=coords space=entity")]
	vector m_vPosition;
	
	[Attribute(defvalue: "0 0 0", UIWidgets.EditBox, desc: "Angles of the intel", params: "inf inf 0 purpose=angles space=entity")]
	vector m_vAngles;
}

//------------------------------------------------------------------------------------------------
class COE_IntelSlotsComponentClass : ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
class COE_IntelSlotsComponent : ScriptComponent
{
#ifdef WORKBENCH
	[Attribute(defvalue: "0", desc: "Show entity preview")]
	protected bool m_bShowDebugEntityPreviewInWorkbench;
	
	[Attribute(defvalue: "{6D56FED1E55A8F84}Prefabs/Items/Misc/IntelligenceFolder_E_01/IntelligenceFolder_E_01.et", desc: "Name of the prefab for the preview")]
	protected ResourceName m_sDebugPreviewPrefabName;
#endif
	
	[Attribute(desc: "Configurations for intel slots")]
	protected ref array<ref COE_IntelSlotConfig> m_aIntelSlotConfigs;
	
	//------------------------------------------------------------------------------------------------
	void GetTransformFromConfig(out vector transform[4], COE_IntelSlotConfig config)
	{
		vector ownerTransform[4];
		GetOwner().GetWorldTransform(ownerTransform);
		Math3D.AnglesToMatrix(config.m_vAngles, transform);
		Math3D.MatrixMultiply3(ownerTransform, transform, transform);
		transform[3] = GetOwner().CoordToParent(config.m_vPosition);
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity SpawnInRandomSlot(ResourceName prefabName)
	{
		if (m_aIntelSlotConfigs.IsEmpty())
			return null;
		
		Math.Randomize(-1);
		return Spawn(prefabName, m_aIntelSlotConfigs[Math.RandomInt(0, m_aIntelSlotConfigs.Count())]);
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity Spawn(ResourceName prefabName, COE_IntelSlotConfig config = null)
	{
		if (!config)
		{
			Math.Randomize(-1);
			config = m_aIntelSlotConfigs[Math.RandomInt(0, m_aIntelSlotConfigs.Count())];
		};
		
		vector transform[4];
		GetTransformFromConfig(transform, config);
		return COE_GameTools.SpawnPrefab(prefabName, transform);
	}

#ifdef WORKBENCH	
	//------------------------------------------------------------------------------------------------
	void SpawnEntityPreview(IEntity owner, ResourceName prefabName)
	{
		foreach (COE_IntelSlotConfig slotConfig : m_aIntelSlotConfigs)
		{
			Spawn(prefabName, slotConfig);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	override void _WB_OnInit(IEntity owner, inout vector mat[4], IEntitySource src)
	{
		if (!m_bShowDebugEntityPreviewInWorkbench)
			return;
		
		foreach (COE_IntelSlotConfig slotConfig : m_aIntelSlotConfigs)
		{
			SCR_EntityHelper.DeleteEntityAndChildren(slotConfig.m_pPreviewEntity);
			SpawnEntityPreview(owner, m_sDebugPreviewPrefabName);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	override bool _WB_OnKeyChanged(IEntity owner, BaseContainer src, string key, BaseContainerList ownerContainers, IEntity parent)
	{
		if (key == "m_bShowDebugEntityPreviewInWorkbench")
		{
			foreach (COE_IntelSlotConfig slotConfig : m_aIntelSlotConfigs)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(slotConfig.m_pPreviewEntity);
				
				if (m_bShowDebugEntityPreviewInWorkbench)
					SpawnEntityPreview(owner, m_sDebugPreviewPrefabName);
			};
			
			return true;			
		};
		
		if (!m_bShowDebugEntityPreviewInWorkbench)
			return false;
		
		if (!m_bShowDebugEntityPreviewInWorkbench)
		{
			return true;	
		}
		else if (key == "coords")
		{
			foreach (COE_IntelSlotConfig slotConfig : m_aIntelSlotConfigs)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(slotConfig.m_pPreviewEntity);
				SpawnEntityPreview(owner, m_sDebugPreviewPrefabName);
			};
			
			return true;
		}
		else if (key == "m_sDebugPreviewPrefabName")
		{
			foreach (COE_IntelSlotConfig slotConfig : m_aIntelSlotConfigs)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(slotConfig.m_pPreviewEntity);
			};
		};
		
		return false;
	}
#endif
}