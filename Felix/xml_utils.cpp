#include "stdafx.h"
#include "xml_utils.h"

// read xml notes
BOOL read_xml_bool(pugi::xml_node &node, string name)
{
	const string val = node.child(name.c_str()).child_value() ;
	return val == "true" ? TRUE : FALSE ;
}
long read_xml_long(pugi::xml_node &node, string name)
{
	const string val = node.child(name.c_str()).child_value() ;
	return string2long(val) ;
}
unsigned long read_xml_ulong(pugi::xml_node &node, string name)
{
	const string val = node.child(name.c_str()).child_value() ;
	return string2ulong(val) ;
}
wstring read_xml_string(pugi::xml_node &node, string name)
{
	const string val = node.child(name.c_str()).child_value() ;
	return string2wstring(val, CP_UTF8) ;
}

// write xml nodes
void write_xml_bool(pugi::xml_node &node, string name, BOOL val)
{
	pugi::xml_node child_node = node.append_child();
	child_node.set_name(name.c_str()) ;
	child_node.append_child(pugi::node_pcdata).set_value(val ? "true" : "false");
}
void write_xml_long(pugi::xml_node &node, string name, long val)
{
	pugi::xml_node child_node = node.append_child();
	child_node.set_name(name.c_str()) ;
	child_node.append_child(pugi::node_pcdata).set_value(int2string(val).c_str());
}
void write_xml_ulong(pugi::xml_node &node, string name, unsigned long val)
{
	pugi::xml_node child_node = node.append_child();
	child_node.set_name(name.c_str()) ;
	child_node.append_child(pugi::node_pcdata).set_value(ulong2string(val).c_str());
}
