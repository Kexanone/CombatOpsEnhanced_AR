//------------------------------------------------------------------------------------------------
class COE_GameModeClass : SCR_BaseGameModeClass
{
};

//------------------------------------------------------------------------------------------------
class COE_GameMode : SCR_BaseGameMode
{
	[Attribute(desc: "Names of GenericEntity for main base location", category: "Main Base")]
	protected ref array<string> m_aBaseLocationNames;
	
	[Attribute(desc: "If this prefab name exists, a carrier will be used as main base", category: "Main Base Carrier")]
	protected ResourceName m_sCarrierToCheckPrefabName;
	
	[Attribute(defvalue: "5000", desc: "Maximal objective distance from carrier", category: "Main Base Carrier")]
	protected float m_fMaxObjectiveDistanceCarrier;
	
	[Attribute(defvalue: "CarrierSpawnInnerBorder", desc: "Inner border for carrier positions", category: "Main Base Carrier")]
	protected string m_sCarrierInnerBorderName;
	
	[Attribute(defvalue: "CarrierSpawnOuterBorder", desc: "Outer border for carrier positions", category: "Main Base Carrier")]
	protected string m_sCarrierOuterBorderName;
	
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
		if (Resource.Load(m_sCarrierToCheckPrefabName).IsValid() && GetGame().GetWorld().FindEntityByName(m_sCarrierInnerBorderName))
		{
			CreateMainBaseCarrier();
			return;
		};
		
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
		IEntity baseStructure = COE_GameTools.SpawnStructurePrefab(faction.GetRandomPrefabByLabel(COE_EEntityLabel.PLAYER_MAIN_BASE), transform);
	}
	
	//------------------------------------------------------------------------------------------------
	void CreateMainBaseCarrier()
	{
		COE_PolygonAreaPicker innerBorderPicker = COE_PolygonAreaPicker.Cast(GetGame().GetWorld().FindEntityByName(m_sCarrierInnerBorderName));
		if (!innerBorderPicker)
			return;
		
		COE_PolygonArea innerBorder = COE_PolygonArea.Cast(innerBorderPicker.GetArea());

		COE_PolygonAreaPicker outerBorderPicker = COE_PolygonAreaPicker.Cast(GetGame().GetWorld().FindEntityByName(m_sCarrierOuterBorderName));
		if (!outerBorderPicker)
			return;
		
		COE_PolygonArea outerBorder = COE_PolygonArea.Cast(outerBorderPicker.GetArea());
		
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		Resource resource = Resource.Load(faction.GetRandomPrefabByLabel(COE_EEntityLabel.PLAYER_MAIN_BASE_CARRIER));
		if (!resource)
			return;
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		Math.Randomize(-1);
		Math3D.AnglesToMatrix(Vector(Math.RandomFloat(0, 360), 0, 0), params.Transform);				
		params.Transform[3] = COE_AreaBase.SamplePointInArea(outerBorder, innerBorder);
		faction.SetPlayerMainBase(GetGame().SpawnEntityPrefab(resource, GetWorld(), params));
		COE_ObjectiveManager.GetInstance().SetMaxDistanceToMainBase(m_fMaxObjectiveDistanceCarrier);
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
