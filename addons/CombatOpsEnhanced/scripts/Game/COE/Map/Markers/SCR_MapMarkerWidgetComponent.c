//------------------------------------------------------------------------------------------------
//! Map marker layout component
//! Attached to root of marker base layout
modded class SCR_MapMarkerWidgetComponent : SCR_ScriptedWidgetComponent
{
	//------------------------------------------------------------------------------------------------
	string GetText()
	{
		return m_wMarkerText.GetText();
	}
}