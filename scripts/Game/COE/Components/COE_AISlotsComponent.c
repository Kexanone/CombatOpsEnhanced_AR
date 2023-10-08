//------------------------------------------------------------------------------------------------
class COE_AISlotConfig : ScriptAndConfig
{
#ifdef WORKBENCH
	IEntity m_pPreviewEntity;
#endif
	
	[Attribute(defvalue:"0 0 0", UIWidgets.EditBox, desc: "Position of the AI", params: "inf inf 0 purpose=coords space=entity")]
	vector m_vPosition;
	
	[Attribute(defvalue: "0 0 0", UIWidgets.EditBox, desc: "Position the AI should look at", params: "inf inf 0 purpose=coords space=entity")]
	vector m_vLookPosition;
	
	[Attribute(defvalue: "0", desc: "Slots can be used when owner not main structure in location", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ECharacterStanceChange))]
	ECharacterStanceChange m_iStanceChange;
	
	[Attribute(desc: "Label of AI suitable for this slot", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EEntityLabel))]
	COE_EEntityLabel m_iEntityLabel;
	
	[Attribute(defvalue: "1", desc: "Slots can be used when owner not main structure in location")]
	bool m_bOccupyWhenNotMain;
	
	[Attribute(defvalue: "{000CD338713F2B5A}Prefabs/AI/Groups/Group_Base.et", desc: "Prefab name for group base")]
	protected ResourceName m_sGroupBasePrefabName;
	
	[Attribute(defvalue: "{4C919D3F6F1CD1B8}Prefabs/AI/Waypoints/COE_AIWaypoint_Defend_0m.et", desc: "Name of the prefab for the waypoint")]
	protected ResourceName m_sWaypointPrefabName;
	
	protected bool m_bIsOccupied = false;
	protected IEntity m_pOwner;
	
	//------------------------------------------------------------------------------------------------
	void SetOwner(IEntity owner)
	{
		m_pOwner = owner;
	}
	
		
	//------------------------------------------------------------------------------------------------
	void GetTransform(out vector transform[4])
	{
		transform[3] = m_pOwner.CoordToParent(m_vPosition);
		vector lookPos = m_pOwner.CoordToParent(m_vLookPosition);
		Math3D.AnglesToMatrix(Vector(vector.Direction(transform[3], lookPos).ToYaw(), 0, 0), transform);
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity Spawn()
	{
		if (m_bIsOccupied)
			return null;
		
		m_bIsOccupied = true;
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return null;
		
		COE_Faction faction = factionManager.GetEnemyFaction();
		if (!faction)
			return null;
		
		ResourceName prefabName = faction.GetRandomPrefabByLabel(m_iEntityLabel);
		if (prefabName.IsEmpty())
			return null;
		
		EntitySpawnParams spawnParams = new EntitySpawnParams;
		spawnParams.TransformMode = ETransformMode.WORLD;
		GetTransform(spawnParams.Transform);
		SCR_AIGroup group = SCR_AIGroup.Cast(COE_GameTools.SpawnGroupPrefab(m_sGroupBasePrefabName));
		group.SetFaction(faction);
		IEntity entity = GetGame().SpawnEntityPrefab(Resource.Load(prefabName), null, spawnParams);
		group.AddAIEntityToGroup(entity, 0);
		AIWaypoint wp = COE_GameTools.SpawnWaypointPrefab(m_sWaypointPrefabName, spawnParams.Transform[3]);
		group.AddWaypoint(wp);
		
		if (m_iStanceChange != ECharacterStanceChange.STANCECHANGE_NONE)
			COE_Utils.SetStanceChange(entity, m_iStanceChange);
		
		return entity;
	}
}

//------------------------------------------------------------------------------------------------
class COE_AISlotsComponentClass : ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
class COE_AISlotsComponent : ScriptComponent
{
#ifdef WORKBENCH
	[Attribute(defvalue: "0", desc: "Show entity preview")]
	protected bool m_bShowDebugEntityPreviewInWorkbench;
	
	[Attribute(defvalue: "{7B2420C8BC72373A}Prefabs/Characters/Core/Character_Base_Preview.et", desc: "Name of the prefab for the preview")]
	protected ResourceName m_sDebugPreviewPrefabName;
#endif
	
	[Attribute(desc: "Configurations for AI slots")]
	protected ref array<ref COE_AISlotConfig> m_aAISlotConfigs;
	
	//------------------------------------------------------------------------------------------------
	void COE_AISlotsComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
		foreach (COE_AISlotConfig config : m_aAISlotConfigs)
		{
			config.SetOwner(GetOwner());
		};
	}
	
	//------------------------------------------------------------------------------------------------
	array<COE_AISlotConfig> GetSlotConfigs(COE_EEntityLabel label, bool isMain = false)
	{
		array<COE_AISlotConfig> configs = {};
		
		foreach (COE_AISlotConfig config : m_aAISlotConfigs)
		{
			if (config.m_iEntityLabel != label)
				continue;
			
			if (!isMain && !config.m_bOccupyWhenNotMain)
				continue;
			
			configs.Insert(config);
		};
		
		return configs;
	}

#ifdef WORKBENCH	
	//------------------------------------------------------------------------------------------------
	void SpawnEntityPreview(IEntity owner, ResourceName prefabName)
	{
		foreach (COE_AISlotConfig slotConfig : m_aAISlotConfigs)
		{
			EntitySpawnParams params = new EntitySpawnParams;
			params.TransformMode = ETransformMode.WORLD;
			slotConfig.GetTransform(params.Transform);
			GetGame().SpawnEntityPrefab(Resource.Load(prefabName), null, params);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	override void _WB_OnInit(IEntity owner, inout vector mat[4], IEntitySource src)
	{
		if (!m_bShowDebugEntityPreviewInWorkbench)
			return;
		
		foreach (COE_AISlotConfig slotConfig : m_aAISlotConfigs)
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
			foreach (COE_AISlotConfig slotConfig : m_aAISlotConfigs)
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
			foreach (COE_AISlotConfig slotConfig : m_aAISlotConfigs)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(slotConfig.m_pPreviewEntity);
				SpawnEntityPreview(owner, m_sDebugPreviewPrefabName);
			};
			
			return true;
		}
		else if (key == "m_sDebugPreviewPrefabName")
		{
			foreach (COE_AISlotConfig slotConfig : m_aAISlotConfigs)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(slotConfig.m_pPreviewEntity);
			};
		};
		
		return false;
	}
#endif
}