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
	protected SCR_BaseTask m_pTask;
	protected ref ScriptInvoker m_OnObjectiveCompleted = new ScriptInvoker;
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnObjectiveCompleted()
	{
		return m_OnObjectiveCompleted;
	}
}