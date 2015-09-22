#pragma once

inline void register_message_filter( CMessageFilter *msg_filter )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	if (pLoop)
	{
		pLoop->AddMessageFilter(msg_filter);
	}
}