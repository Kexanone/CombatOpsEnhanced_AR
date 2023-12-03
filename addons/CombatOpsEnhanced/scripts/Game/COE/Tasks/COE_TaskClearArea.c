//------------------------------------------------------------------------------------------------
class COE_TaskClearAreaConfig : COE_TaskBaseConfig
{
	//------------------------------------------------------------------------------------------------
	override void Create(COE_Location location = null)
	{
		super.Create(location);
		location.SpawnAPCs();
	}
}
