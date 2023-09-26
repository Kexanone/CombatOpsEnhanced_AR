//------------------------------------------------------------------------------------------------
//! A base class for tasks.
modded class SCR_BaseTask : GenericEntity
{
	//------------------------------------------------------------------------------------------------
	void COE_SetIconName(string name)
	{
		m_sMapIconName = name;
		m_sTaskListIconName = name;
	}
}