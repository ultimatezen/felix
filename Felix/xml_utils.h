/*
	Utilities for reading/writing xml.
	Uses pugxml.
*/

// read/write xml

// read xml values
BOOL read_xml_bool(pugi::xml_node &node, string name) ;
long read_xml_long(pugi::xml_node &node, string name) ;
unsigned long read_xml_ulong(pugi::xml_node &node, string name) ;
wstring read_xml_string(pugi::xml_node &node, string name) ;

// write xml values
void write_xml_bool(pugi::xml_node &node, string name, BOOL val) ;
void write_xml_long(pugi::xml_node &node, string name, long val) ;
void write_xml_ulong(pugi::xml_node &node, string name, unsigned long val) ;
pugi::xml_node get_prop_node( pugi::xml_document &doc, string node_name ) ;
