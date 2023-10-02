//------------------------------------------------------------------------------------------------
class COE_TaskDeliverIntelConfig : COE_TaskBaseConfig
{
	[Attribute(desc: "Label of intel prefab", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EPrefabLabel))]
	protected COE_EPrefabLabel m_iIntelPrefabLabel;
	
	//------------------------------------------------------------------------------------------------
	override void Create(COE_LocationBase location = null)
	{
		IEntity mainStructure = location.GetMainStructure();
		if (!mainStructure)
		{
			COE_Utils.PrintError(string.Format("%1 has no main struture", location), "COE_TaskDeliverIntel");
			return;
		};
		
		COE_IntelSlotsComponent slotComponent = COE_IntelSlotsComponent.Cast(mainStructure.FindComponent(COE_IntelSlotsComponent));
		if (!slotComponent)
		{
			COE_Utils.PrintError(string.Format("%1 has no COE_IntelSlotsComponent", mainStructure), "COE_TaskDeliverIntel");
			return;
		};
		
		IEntity intel = slotComponent.SpawnInRandomSlot(GetIntelPrefabName());
		SpawnSFTaskPrefab(mainStructure.GetOrigin(), intel);
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetIntelPrefabName()
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return string.Empty;
		
		COE_Faction faction = factionManager.GetEnemyFaction();
		if (!faction)
			return string.Empty;
		
		return faction.GetRandomPrefabByLabel(m_iIntelPrefabLabel);
	}
	
	//------------------------------------------------------------------------------------------------
	override void SpawnSFTaskPrefab(vector pos = vector.Zero, IEntity targetEntity = null)
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		IEntity base = faction.GetPlayerMainBase();
		if (!base)
			return;
				
		vector transform[4];
		Math3D.MatrixIdentity4(transform);
		transform[3] = pos;
		IEntity layer = COE_GameTools.SpawnPrefab(m_sSFTaskPrefabName, transform);
		
		IEntity slot = layer.GetChildren();
		while (slot)
		{
			SCR_ScenarioFrameworkSlotPick slotPickComponent = SCR_ScenarioFrameworkSlotPick.Cast(slot.FindComponent(SCR_ScenarioFrameworkSlotPick));
			if (slotPickComponent)
			{
				slotPickComponent.SetEntity(targetEntity);
				slot.SetOrigin(pos);
			};
			
			SCR_ScenarioFrameworkSlotDelivery slotDeliverComponent = SCR_ScenarioFrameworkSlotDelivery.Cast(slot.FindComponent(SCR_ScenarioFrameworkSlotDelivery));
			if (slotDeliverComponent)
			{
				slot.SetOrigin(base.GetOrigin());
			};
			
			slot = slot.GetSibling();
		};
		
		SCR_ScenarioFrameworkLayerBase layerSFComponent = SCR_ScenarioFrameworkLayerBase.Cast(layer.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (!layerSFComponent)
			return;
		
		layerSFComponent.Init();
	}
}
