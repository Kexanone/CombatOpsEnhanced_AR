[EntityEditorProps(category: "GameScripted/UI/Inventory")]
class COE_VehicleArsenalInventoryStorageManagerComponentClass : SCR_VehicleInventoryStorageManagerComponentClass
{
};

class COE_VehicleArsenalInventoryStorageManagerComponent : SCR_VehicleInventoryStorageManagerComponent
{
	//------------------------------------------------------------------------------------------------
	protected override void FillInitialPrefabsToStore(out array<ResourceName> prefabsToSpawn)
	{
		super.FillInitialPrefabsToStore(prefabsToSpawn);
		
		if (SCR_Global.IsEditMode())
			return;
		
		SCR_ArsenalComponent arsenalComponent = SCR_ArsenalComponent.Cast(GetOwner().FindComponent(SCR_ArsenalComponent));
		if (arsenalComponent)
		{
			arsenalComponent.GetAvailablePrefabs(prefabsToSpawn);
		}
	}
};
