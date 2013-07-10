#include "stdafx.h"
#include "FelixApp.h"
#include "FelixModel.h"

app_props::props_ptr app::get_props()
{
	return app_props::get_props() ;
}

model_iface_ptr app::get_model_interface()
{
	app_props::props_ptr props = get_props(); 
	return model_iface_ptr(new FelixModel(props)) ;
}

MemoryWindowFrame& app::get_app( void )
{
	static model_iface_ptr the_model = get_model_interface();
	app_props::props_ptr props = get_props(); 
	static MemoryWindowFrame main_frame( the_model, props ) ;

	return main_frame ;
}
