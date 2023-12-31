//------------------------------------------------------------------------------------------------
class COE_AITasks
{
	//------------------------------------------------------------------------------------------------
	static void Patrol(AIGroup group, vector position, float radius = 100, int count = 5, float minDistance = 50)
	{
		AIFormationComponent formationHandler = AIFormationComponent.Cast(group.FindComponent(AIFormationComponent));
		
		if (!formationHandler)
			return;
		
		formationHandler.SetFormation("Column");
		
		vector prevPos = position;
		array<AIAgent> agents = {};
		group.GetAgents(agents);
		
		if (!agents.IsEmpty())
		{
			prevPos = agents[0].GetControlledEntity().GetOrigin();
			
		};
		
		prevPos[1] = SCR_TerrainHelper.GetTerrainY(prevPos);
		
		array<AIWaypoint> wpList = {};
		int i = 1;
		
		Print("|gog|COE_AITasks.Patrol|Start|");
		PrintFormat("|%1|%2|", position, radius);
		while (i <= count)
		{
			vector pos = SCR_Math2D.GenerateRandomPointInRadius(0, radius, position);
			pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
			if (vector.Distance(prevPos, pos) >= minDistance && !COE_Utils.SurfaceIsWater(pos) &&  COE_WorldTools.IsPosEmpty(pos))
			{
				AIWaypoint wp = COE_GameTools.SpawnWaypointPrefab("{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et", pos);
				group.AddWaypoint(wp);
				wpList.Insert(wp);
				prevPos = pos;
				i++;
			};
		};
		Print("|gog|COE_AITasks.Patrol|End|");
		
		AIWaypointCycle wpCycle = AIWaypointCycle.Cast(COE_GameTools.SpawnWaypointPrefab("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et", prevPos));
		wpCycle.SetWaypoints(wpList);
		group.AddWaypoint(wpCycle);
	}
}