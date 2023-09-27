//------------------------------------------------------------------------------------------------
class COE_ObjectiveBaseType : ScriptAndConfig
{
	//------------------------------------------------------------------------------------------------
	COE_ObjectiveBase CreateInstance()
	{
		return COE_ObjectiveBase();
	}
}

//------------------------------------------------------------------------------------------------
class COE_ObjectiveBase : Managed
{
	protected IEntity m_pTaskLayer;
	
	//------------------------------------------------------------------------------------------------
	void SetTaskLayer(IEntity layer)
	{
		m_pTaskLayer = layer;
	}
}