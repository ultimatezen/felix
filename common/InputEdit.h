
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//--------------------------------------------
class CInputEdit : 
public CWindowImpl<CInputEdit, CEdit>,
public CEditCommands<CInputEdit>
{
public:
	BEGIN_MSG_MAP(CInputEdit)
		CHAIN_MSG_MAP_ALT(CEditCommands<CInputEdit>, 1)
	END_MSG_MAP()
		
};
