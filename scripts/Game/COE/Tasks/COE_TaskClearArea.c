//------------------------------------------------------------------------------------------------
class COE_TaskClearAreaConfig : COE_TaskBaseConfig
{
	//------------------------------------------------------------------------------------------------
	override void Create(COE_Location location = null)
	{
		super.Create(location);
		int apcCount = Math.RandomIntInclusive(0, 1);
		SpawnAPCs(location, apcCount);
	}
	
	void SpawnAPCs(COE_Location location, int count)
	{
		COE_CircleArea area = COE_CircleArea(location.GetCenter(), location.GetRadius());
		COE_SamplePosParams params = COE_SamplePosParams();
		params.EmptyRadius = 5;
		vector transform[4];
		
		
		for (int i = 0; i < count; i++)
		{
			COE_WorldTools.SampleTransformInArea(transform, area, {}, params);
			IEntity btr = COE_GameTools.SpawnStructurePrefab(m_EnemyFaction.GetRandomPrefabByLabel(COE_EEntityLabel.ARMED_VEHICLE), transform);
			btr.GetPhysics().SetActive(ActiveState.ACTIVE);
		}
	}
}
