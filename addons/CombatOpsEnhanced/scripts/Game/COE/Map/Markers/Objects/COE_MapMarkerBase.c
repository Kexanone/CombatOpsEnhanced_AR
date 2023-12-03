//------------------------------------------------------------------------------------------------
//! Player map marker class 
class COE_MapMarkerBase : SCR_MapMarkerBase
{
	protected Color m_IconColor;
	protected SCR_Faction m_Faction;
	protected EMilitarySymbolDimension m_iSymbolDimension;
	protected EMilitarySymbolIcon m_iSymbolIcon;
	protected EMilitarySymbolAmplifier m_iSymbolAmplifier;
	protected OverlayWidget m_wSymbolOverlay;
	protected SCR_MilitarySymbolUIComponent m_MilitarySymbolComponent;
	
	//------------------------------------------------------------------------------------------------
	override string GetCustomText()
	{
		if (m_MarkerWidgetComp)
			return m_MarkerWidgetComp.GetText();
		else
			return m_sCustomText;
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetCustomText(string text)
	{
		if (m_MarkerWidgetComp)
			m_MarkerWidgetComp.SetText(text);
		else
			m_sCustomText = text;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetIconColor(Color color)
	{
		if (m_MarkerWidgetComp)
			m_MarkerWidgetComp.SetColor(color);
		else
			m_IconColor = color;
	}
	
	void SetMilitarySymbol(SCR_Faction faction, EMilitarySymbolDimension dimension, EMilitarySymbolIcon icon, EMilitarySymbolAmplifier amplifier = EMilitarySymbolAmplifier.NONE)	
	{
		if (m_MilitarySymbolComponent)
		{
			
			EMilitarySymbolIdentity identity = EMilitarySymbolIdentity.UNKNOWN;
			
			switch (faction.GetFactionKey())
			{
				case "US": {identity = EMilitarySymbolIdentity.BLUFOR; break;};
				case "USSR": {identity = EMilitarySymbolIdentity.OPFOR; break;};
				case "FIA": {identity = EMilitarySymbolIdentity.INDFOR; break;};
			};
			
			SCR_MilitarySymbol symbol = new SCR_MilitarySymbol();
			symbol.Set(identity, dimension, icon, amplifier);
			m_MilitarySymbolComponent.Update(symbol);
			m_wSymbolOverlay.SetColor(faction.GetFactionColor());
		}
		else
		{
			m_Faction = faction;
			m_iSymbolDimension = dimension;
			m_iSymbolIcon = icon;
			m_iSymbolAmplifier = amplifier;
		};
	}
		
	//------------------------------------------------------------------------------------------------
	//! Fetch marker definition from config & create widget
	override void OnCreateMarker()
	{
		if (m_wRoot)
			return;
		
		if (!SCR_MapEntity.GetMapInstance().GetMapMenuRoot())
			return;
		
		super.OnCreateMarker();
		
		if (!m_wRoot)
			return;
		
		m_MarkerWidgetComp.SetAuthor("");
		SetMarkerOwnerID(-1);
		
		m_wSymbolOverlay = OverlayWidget.Cast(m_wRoot.FindAnyWidget("SymbolOverlay"));				
		m_MilitarySymbolComponent = SCR_MilitarySymbolUIComponent.Cast(m_wSymbolOverlay.FindHandler(SCR_MilitarySymbolUIComponent));
		
		if (m_Faction)
			SetMilitarySymbol(m_Faction, m_iSymbolDimension, m_iSymbolIcon, m_iSymbolAmplifier);
	}
}
