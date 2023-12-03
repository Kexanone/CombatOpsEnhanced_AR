//------------------------------------------------------------------------------------------------
class COE_Utils
{
	//------------------------------------------------------------------------------------------------
	static array<IEntity> GetPlayers(bool aliveOnly = false)
	{
		PlayerManager playerManager = GetGame().GetPlayerManager();
		array<int> playerIds = {};
		playerManager.GetAllPlayers(playerIds);
		
		if (playerIds.IsEmpty())
			return {};
		
		array<IEntity> players = {};
		foreach (int playerId : playerIds)
		{
			IEntity player = playerManager.GetPlayerControlledEntity(playerId);
			
			if (!player)
				continue;
			
			if (!aliveOnly || SCR_AIDamageHandling.IsAlive(player))
			{
				players.Insert(player);
			};
		};
		
		return players;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool SurfaceIsWater(vector pos)
	{
		return GetGame().GetWorld().GetSurfaceY(pos[0], pos[2]) <= GetGame().GetWorld().GetOceanBaseHeight();
	}
	
	//------------------------------------------------------------------------------------------------
	static bool SurfaceIsFlat(vector pos, float maxAngle = 5)
	{
		vector normal = SCR_TerrainHelper.GetTerrainNormal(pos);
		return Math.RAD2DEG * Math.Acos(vector.Dot(Vector(0, 1, 0), normal)) < maxAngle;
	}
	
	//------------------------------------------------------------------------------------------------
	static void PrintError(string message, string component = string.Empty)
	{
		string combinedMessage = "Combat Ops Enhanced";
		
		if (!component.IsEmpty())
			combinedMessage += " - " + component;
				
		combinedMessage += ": " + message;
		Print(message, LogLevel.ERROR);
	}
	
	//------------------------------------------------------------------------------------------------
	static void SetStanceChange(IEntity unit, ECharacterStanceChange stanceChange)
	{
		ECharacterStance stance = -1;
		
		if (stanceChange > 0)
			stance = stanceChange - 1;
		
		AIControlComponent aiCtrlComponent = AIControlComponent.Cast(unit.FindComponent(AIControlComponent));
		if (!aiCtrlComponent)
			return;
		
		AIAgent agent = aiCtrlComponent.GetControlAIAgent();
		if (!agent)
			return;
		
		SCR_AIInfoComponent infoComponent = SCR_AIInfoComponent.Cast(agent.FindComponent(SCR_AIInfoComponent));
		if (!infoComponent)
			return;
			
		CharacterControllerComponent charCtrlComponent = CharacterControllerComponent.Cast(unit.FindComponent(CharacterControllerComponent));
		if (!charCtrlComponent)
			return;
		
		if (stance >= 0)
			infoComponent.SetStance(stance);
		
		charCtrlComponent.SetStanceChange(stanceChange);
	}
}
