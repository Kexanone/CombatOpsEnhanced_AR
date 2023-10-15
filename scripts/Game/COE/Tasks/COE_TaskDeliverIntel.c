//------------------------------------------------------------------------------------------------
class COE_TaskDeliverIntelConfig : COE_TaskBaseConfig
{
	[Attribute(desc: "Label of intel prefab", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EEntityLabel))]
	protected COE_EEntityLabel m_iIntelPrefabLabel;
	
	protected const static string s_sLayerNameFormat = "COE_INTEL_LAYER_%1";
	protected static int s_iLayerCounter = 0;
	
	static string GenerateLayerName()
	{
		s_iLayerCounter++;
		return string.Format(s_sLayerNameFormat, s_iLayerCounter);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Create(COE_Location location = null)
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
			array<COE_IntelSlotsComponent> slotComponents = {};
			IEntity furniture = mainStructure.GetChildren();
			
			while (furniture)
			{
				slotComponent = COE_IntelSlotsComponent.Cast(furniture.FindComponent(COE_IntelSlotsComponent));
				if (slotComponent)
					slotComponents.Insert(slotComponent);
				
				furniture = furniture.GetSibling();
			};
			
			if (slotComponents.IsEmpty())
			{
				COE_Utils.PrintError(string.Format("%1 has no COE_IntelSlotsComponent", mainStructure), "COE_TaskDeliverIntel");
				return;
			};
			
			Math.Randomize(-1);
			slotComponent = slotComponents[Math.RandomInt(0, slotComponents.Count())];
		};
		
		ResourceName intelPrefabName = m_EnemyFaction.GetRandomPrefabByLabel(m_iIntelPrefabLabel);
		IEntity intel = slotComponent.SpawnInRandomSlot(intelPrefabName);
		SpawnSFTaskPrefab(mainStructure.GetOrigin(), intel);
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
		IEntity layerEntity = COE_GameTools.SpawnPrefab(m_sSFTaskPrefabName, transform);
		string layerName = GenerateLayerName();
		layerEntity.SetName(layerName);
		
		IEntity slotEntity = layerEntity.GetChildren();
		while (slotEntity)
		{
			SCR_ScenarioFrameworkSlotPick slotPick = SCR_ScenarioFrameworkSlotPick.Cast(slotEntity.FindComponent(SCR_ScenarioFrameworkSlotPick));
			if (slotPick)
			{
				slotPick.COE_SetAsset(targetEntity);
				slotEntity.SetOrigin(pos);
			};
			
			SCR_ScenarioFrameworkSlotDelivery slotDeliver = SCR_ScenarioFrameworkSlotDelivery.Cast(slotEntity.FindComponent(SCR_ScenarioFrameworkSlotDelivery));
			if (slotDeliver)
			{
				slotEntity.SetOrigin(base.GetOrigin());
				slotDeliver.COE_AddAssociatedTaskLayer(layerName);
				slotDeliver.COE_SetFactionKey(m_PlayerFaction.GetFactionKey());
			};
			
			slotEntity = slotEntity.GetSibling();
		};
		
		SCR_ScenarioFrameworkLayerBase layer = SCR_ScenarioFrameworkLayerBase.Cast(layerEntity.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (!layer)
			return;
		
		layer.COE_SetFactionKey(m_PlayerFaction.GetFactionKey());
		layer.Init();
	}
}
