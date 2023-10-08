//------------------------------------------------------------------------------------------------
class COE_VehicleSpawnSlotClass : GenericEntityClass
{
};

//------------------------------------------------------------------------------------------------
class COE_VehicleSpawnSlot : GenericEntity
{
	[Attribute(desc: "Vehicle prefab to spawn on this slot")]
	protected ResourceName m_sVehiclePrefabToSpawn;
	
	Vehicle m_pVehicle = null;
	protected IEntity m_pPreviewEntity;
	bool m_bWasVehicleDeserted = false;
	
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
		COE_VehicleSpawnManager spawnManager = COE_VehicleSpawnManager.GetInstance();
		spawnManager.AddSlot(this);
	};
	
	void ScheduleRespawn()
	{
		SCR_EditableVehicleComponent editableVehicle = SCR_EditableVehicleComponent.Cast(m_pVehicle.FindComponent(SCR_EditableVehicleComponent));
		array<CompartmentAccessComponent> crewCompartmentAccess = new array<CompartmentAccessComponent>;
		editableVehicle.GetCrew(crewCompartmentAccess, false);

		foreach (CompartmentAccessComponent compartmentAccess: crewCompartmentAccess)
		{
			compartmentAccess.EjectOutOfVehicle();
		};
		
		GetGame().GetCallqueue().CallLater(Respawn, 3000);	
	}
	
	void Respawn()
	{
		if (m_pVehicle)
			SCR_EntityHelper.DeleteEntityAndChildren(m_pVehicle);
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(params.Transform);
		m_pVehicle = Vehicle.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sVehiclePrefabToSpawn), null, params));
		m_pVehicle.GetPhysics().SetActive(ActiveState.ACTIVE);
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
