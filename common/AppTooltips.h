#pragma once

#include "vxtool/VXPLib.h"
#include "Broadcaster.h"

typedef std::vector< int > CommandVector ;

class CAppTooltips
{
public:
	typedef Loki::AssocVector< int, std::wstring > TooltipMapType ;

private:
	CAppTooltips(void);
public:
	~CAppTooltips(void);

	static CAppTooltips& instance()
	{
		static CAppTooltips tips ;
		return tips ;
	}

	template< class ContainerType >
	void SetTips( ContainerType &commands )
	{
		for ( ContainerType::iterator pos = commands.begin() ; pos != commands.end() ; ++pos )
		{
			m_TipManager.commands.Add( *pos, m_tips[*pos].c_str() ) ;
		}
	}
private:
	bool OnAppQuit( boost::any &Param )
	{
		Param ;
		m_TipManager.Detach() ;
		return false ; // not handled
	}
	TooltipMapType m_tips ;
	vxlib::CVXPTooltipManager m_TipManager ;

};
