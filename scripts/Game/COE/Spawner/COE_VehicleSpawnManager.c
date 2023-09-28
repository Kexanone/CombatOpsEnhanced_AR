//------------------------------------------------------------------------------------------------
class COE_VehicleSpawnManagerClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class COE_VehicleSpawnManager : GenericEntity
{
	[Attribute("3", UIWidgets.EditBox, desc: "Number of seconds after which vehicle respawns when no player is with the deserted distance")]
	protected float m_fDesertedDelay;
	
	[Attribute("1000", UIWidgets.EditBox, desc: "Deserted distance for players in meters")]
	protected float m_fDesertedPlayerDistance;
	protected float m_fDesertedPlayerDistanceSq;
	
	[Attribute("1.5", UIWidgets.EditBox, desc: "Deserted distance for spawn in meters")]
	protected float m_fDesertedSlotDistance;
	protected float m_fDesertedSlotDistanceSq;
	
	protected static COE_VehicleSpawnManager m_pInstance;
	protected ref array<COE_VehicleSpawnSlot> m_aManagedSlots = {};
	protected float m_fHandlerTimer = 0;
	
	void COE_VehicleSpawnManager(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		m_pInstance = this;
		m_fDesertedPlayerDistanceSq = Math.Pow(m_fDesertedPlayerDistance, 2);
		m_fDesertedSlotDistanceSq = Math.Pow(m_fDesertedSlotDistance, 2);
		Activate();
		SetEventMask(EntityEvent.FRAME);
	};
	
	static COE_VehicleSpawnManager GetInstance()
	{
		return m_pInstance;
	}
	
	override protected void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		m_fHandlerTimer += timeSlice;
		
		if (m_fHandlerTimer < m_fDesertedDelay)
			return;
		
		m_fHandlerTimer = 0;
		array<IEntity> players = COE_Utils.GetPlayers();
		
		foreach (COE_VehicleSpawnSlot slot : m_aManagedSlots)
		{
			bool isDeserted = true;
			vector vehiclePos = slot.m_pVehicle.GetOrigin();
			
			if (slot.GetIsVehicleAlive() && vehiclePos[1] > GetGame().GetWorld().GetOceanBaseHeight() - 1)
			{
				
				// Skip deserted check if vehicle is close to spawn
				if (vector.DistanceSqXZ(slot.GetOrigin(), vehiclePos) < m_fDesertedSlotDistanceSq)
				{
					slot.m_bWasVehicleDeserted = false;
					continue;
				};
				
				foreach (IEntity player : players)
				{
					// Skip deserted check if player is close
					if (vector.DistanceSqXZ(player.GetOrigin(), vehiclePos) < m_fDesertedPlayerDistanceSq)
					{
						isDeserted =  false;
						break;
					};
				};
			};
			
			if (slot.m_bWasVehicleDeserted && isDeserted)
				slot.ScheduleRespawn();
			else
				slot.m_bWasVehicleDeserted = isDeserted;
		};
	}
	
	void AddSlot(COE_VehicleSpawnSlot slot)
	{
		slot.Respawn();
		m_aManagedSlots.Insert(slot);
	}
}
