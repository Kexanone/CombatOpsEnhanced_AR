//------------------------------------------------------------------------------------------------
class COE_VehicleSpawnSlotClass : GenericEntityClass
{
};

//------------------------------------------------------------------------------------------------
class COE_VehicleSpawnSlot : GenericEntity
{
	[Attribute(desc: "Vehicle prefab to spawn on this slot")]
	protected ResourceName m_sVehiclePrefabToSpawn;
	
	[Attribute(defvalue: "3", desc: "Respawn delay in seconds")]
	protected float m_iRespawnDelay;
	
	protected Vehicle m_pVehicle = null;
	protected IEntity m_pPreviewEntity;
	bool m_bWasVehicleDeserted = false;
	bool m_bIsRespawning = false;
	
	//------------------------------------------------------------------------------------------------
	void COE_VehicleSpawnSlot(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode())
			return;
				
		Activate();
		SetEventMask(EntityEvent.INIT);
	};
		
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!Replication.IsServer())
			return;
		
		COE_VehicleSpawnManager spawnManager = COE_VehicleSpawnManager.GetInstance();
		spawnManager.AddSlot(this);
	};
	
	//------------------------------------------------------------------------------------------------
	IEntity GetVehicle()
	{
		return m_pVehicle;
	}
	
	protected array<CompartmentAccessComponent> GetCrew()
	{
		SCR_EditableVehicleComponent editableVehicle = SCR_EditableVehicleComponent.Cast(m_pVehicle.FindComponent(SCR_EditableVehicleComponent));
		array<CompartmentAccessComponent> crewCompartmentAccess = new array<CompartmentAccessComponent>;
		editableVehicle.GetCrew(crewCompartmentAccess, false);
		return crewCompartmentAccess;
	}
	
	void ScheduleRespawn()
	{
		m_bIsRespawning = true;
		EjectCrew();		
		GetGame().GetCallqueue().CallLater(Respawn, 1000*m_iRespawnDelay);	
	}
	
	protected void EjectCrew()
	{
		if (!m_pVehicle)
			return;
		
		SCR_EditableVehicleComponent editableVehicle = SCR_EditableVehicleComponent.Cast(m_pVehicle.FindComponent(SCR_EditableVehicleComponent));
		array<CompartmentAccessComponent> crewCompartmentAccess = GetCrew();

		foreach (CompartmentAccessComponent compartmentAccess: crewCompartmentAccess)
		{
			compartmentAccess.EjectOutOfVehicle();
		};
	}
	
	void Respawn()
	{
		if (m_pVehicle)
		{
			// Crew ejection can be severley delayed
			// => If it is not completed, we reschedule the respawn, since otherwise players that are still in the vehicle get deleted too
			if (!GetCrew().IsEmpty())
			{
				ScheduleRespawn();
				return;
			};
			
			SCR_EntityHelper.DeleteEntityAndChildren(m_pVehicle);
		};
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(params.Transform);
		m_pVehicle = Vehicle.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sVehiclePrefabToSpawn), null, params));
		m_pVehicle.GetPhysics().SetActive(ActiveState.ACTIVE);
		m_bIsRespawning = false;
	};
	
	//------------------------------------------------------------------------------------------------
	bool GetIsVehicleAlive()
	{
		if (!m_pVehicle)
			return false;
		
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(m_pVehicle.FindComponent(DamageManagerComponent));
		return !damageManager.IsDestroyed();
	}

	
#ifdef WORKBENCH
	//------------------------------------------------------------------------------------------------
	void SpawnEntityPreview(Resource resource)
	{
		EntitySpawnParams spawnParams = new EntitySpawnParams;
		spawnParams.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(spawnParams.Transform);
			
		m_pPreviewEntity = GetGame().SpawnEntityPrefab(resource, null, spawnParams);
	}
	
	//------------------------------------------------------------------------------------------------
	override void _WB_OnInit(inout vector mat[4], IEntitySource src)
	{
		SCR_EntityHelper.DeleteEntityAndChildren(m_pPreviewEntity);
			
		Resource resource = Resource.Load(m_sVehiclePrefabToSpawn);
		if (!resource)
			return;
			
		SpawnEntityPreview(resource);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool _WB_OnKeyChanged(BaseContainer src, string key, BaseContainerList ownerContainers, IEntity parent)
	{
		if (key == "coords")
		{
			SCR_EntityHelper.DeleteEntityAndChildren(m_pPreviewEntity);
			
			Resource resource = Resource.Load(m_sVehiclePrefabToSpawn);
			if (!resource)
				return false;
			
			SpawnEntityPreview(resource);
			return true;
		}
		else if (key == "m_sVehiclePrefabToSpawn")
		{
			SCR_EntityHelper.DeleteEntityAndChildren(m_pPreviewEntity);
			return false;
		}
		return false;
	}
#endif
}
