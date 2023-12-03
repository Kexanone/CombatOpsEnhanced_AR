//------------------------------------------------------------------------------------------------
class COE_GameModeClass : SCR_BaseGameModeClass
{
};

//------------------------------------------------------------------------------------------------
class COE_GameMode : SCR_BaseGameMode
{
	[Attribute(desc: "Names of GenericEntity for main base location", category: "Main Base")]
	protected ref array<string> m_aBaseLocationNames;
	
	/*
	[Attribute(desc: "Label of main base prefab", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EEntityLabel))]
	protected COE_EEntityLabel m_iMainBasePrefabLabel;
	
	[Attribute(desc: "Inner border for main base positions", category: "Main Base")]
	protected string m_sMainBaseInnerBorderName;
	
	[Attribute(desc: "Outer border for main base positions", category: "Main Base")]
	protected string m_sMainBaseOuterBorderName;
	*/
	
	protected ref COE_MapMarkerBase m_COE_mainBaseMarker = new COE_MapMarkerBase();
	
	//------------------------------------------------------------------------------------------------
	void COE_GameMode(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		// Read mission header
		COE_MissionHeader header = COE_MissionHeader.Cast(GetGame().GetMissionHeader());
		if (header)
		{
		};
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnGameStart()
	{
		super.OnGameStart();
		//CreateMainBaseMarker();
		
		if (!Replication.IsServer())
			return;
		
		CreateMainBase();
		
		COE_ObjectiveManager objectiveManager = COE_ObjectiveManager.GetInstance();
		if (objectiveManager)
			objectiveManager.OnGameStart();
	}
	
	//------------------------------------------------------------------------------------------------
	void CreateMainBase()
	{
		if (m_aBaseLocationNames.IsEmpty())
		{
			COE_Utils.PrintError(string.Format("No location for main base has been specified"), "COE_GameMode.CreateMainBase");
			return;
		};
		
		Math.Randomize(-1);
		IEntity base = GetGame().GetWorld().FindEntityByName(m_aBaseLocationNames[Math.RandomInt(0, m_aBaseLocationNames.Count())]);
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		
		faction.SetPlayerMainBase(base);
		vector transform[4];
		base.GetWorldTransform(transform);
		COE_GameTools.SpawnStructurePrefab(faction.GetRandomPrefabByLabel(COE_EEntityLabel.PLAYER_MAIN_BASE), transform);
		
		/*
		COE_PolygonAreaPicker innerBorderPicker = COE_PolygonAreaPicker.Cast(GetGame().GetWorld().FindEntityByName(m_sMainBaseInnerBorderName));
		if (!innerBorderPicker)
			return;
		
		COE_PolygonArea innerBorder = COE_PolygonArea.Cast(innerBorderPicker.GetArea());

		COE_PolygonAreaPicker outerBorderPicker = COE_PolygonAreaPicker.Cast(GetGame().GetWorld().FindEntityByName(m_sMainBaseOuterBorderName));
		if (!outerBorderPicker)
			return;
		
		COE_PolygonArea outerBorder = COE_PolygonArea.Cast(outerBorderPicker.GetArea());
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		Resource resource = Resource.Load(faction.GetRandomPrefabByLabel(m_iMainBasePrefabLabel));
		if (!resource)
			return;
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		Math.Randomize(-1);
		Math3D.AnglesToMatrix(Vector(Math.RandomFloat(0, 360), 0, 0), params.Transform);				
		params.Transform[3] = COE_AreaBase.SamplePointInArea(outerBorder, innerBorder);
		faction.SetPlayerMainBase(GetGame().SpawnEntityPrefab(resource, GetWorld(), params));
		*/
	}

	//------------------------------------------------------------------------------------------------
	/*
	void CreateMainBaseMarker()
	{
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		SCR_MapMarkerManagerComponent markerManager = SCR_MapMarkerManagerComponent.Cast(FindComponent(SCR_MapMarkerManagerComponent));
		if (!markerManager)
			return;
		
		IEntity base = GetGame().GetWorld().FindEntityByName("COE_MainBase_1");
		m_COE_mainBaseMarker.SetType(SCR_EMapMarkerType.PLACED_MILITARY);
		vector basePos = base.GetOrigin();
		m_COE_mainBaseMarker.SetWorldPos(basePos[0], basePos[2]);
		markerManager.InsertLocalMarker(m_COE_mainBaseMarker);
		m_COE_mainBaseMarker.SetMilitarySymbol(faction, EMilitarySymbolDimension.INSTALLATION, EMilitarySymbolIcon.RESPAWN);
	}
	*/
}
