//------------------------------------------------------------------------------------------------
class COE_GameModeClass : SCR_BaseGameModeClass
{
};

//------------------------------------------------------------------------------------------------
class COE_GameMode : SCR_BaseGameMode
{
	[Attribute(desc: "Label of main base prefab", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(COE_EEntityLabel))]
	protected COE_EEntityLabel m_iMainBasePrefabLabel;
	
	[Attribute(desc: "Inner border for main base positions", category: "Main Base")]
	protected string m_sMainBaseInnerBorderName;
	
	[Attribute(desc: "Outer border for main base positions", category: "Main Base")]
	protected string m_sMainBaseOuterBorderName;
	
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
		COE_FactionManager factionManager = COE_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return;
		
		COE_Faction faction = factionManager.GetPlayerFaction();
		if (!faction)
			return;
		
		IEntity base = GetGame().GetWorld().FindEntityByName("COE_MainBaseLandUS");
		faction.SetPlayerMainBase(base);
		vector basePos = base.GetOrigin();
		SCR_MapMarkerManagerComponent markerManager = SCR_MapMarkerManagerComponent.Cast(FindComponent(SCR_MapMarkerManagerComponent));
		markerManager.InsertStaticMarkerByType(SCR_EMapMarkerType.PLACED_MILITARY, basePos[0], basePos[2], 32);
		
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
};
