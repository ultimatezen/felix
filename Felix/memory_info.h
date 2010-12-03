#pragma once

namespace mem_engine
{

class MemoryInfo
{
public:
	MemoryInfo(){}
	virtual ~MemoryInfo(){}

	virtual void set_creator_to_current_user()= 0 ;

	virtual wstring get_current_user()= 0 ;

	virtual wstring get_creator() const = 0 ;
	virtual wstring get_field() const = 0 ;
	virtual wstring get_created_on() const = 0 ;

	virtual wstring get_modified_by() const = 0 ; 
	virtual wstring get_modified_on() const = 0 ; 

	virtual wstring get_source_language() const = 0 ;
	virtual wstring get_target_language() const = 0 ;
	virtual wstring get_client() const = 0 ;
	virtual wstring get_creation_tool() const = 0 ;
	virtual wstring get_creation_tool_version() const = 0 ;
	virtual long get_count() const = 0 ;
	virtual bool is_locked() const = 0 ;
	virtual bool is_memory() const = 0 ;

	virtual void set_is_memory(bool) = 0 ;
	virtual void set_is_locked(bool) = 0 ;
	virtual void set_creator( const wstring setting ) = 0 ;
	virtual void set_field( const wstring setting ) = 0 ;
	virtual void set_created_on( const wstring setting ) = 0 ;
	virtual void set_source_language( const wstring setting ) = 0 ;
	virtual void set_target_language( const wstring setting ) = 0 ;
	virtual void set_client( const wstring setting ) = 0 ;
	virtual void set_count( const long setting ) = 0 ;
	virtual void set_locked_on( ) = 0 ;
	virtual void set_locked_off( ) = 0 ;
	virtual void set_is_memory_on() = 0 ;
	virtual void set_is_memory_off() = 0 ;
	virtual void set_creation_tool( const wstring setting )= 0 ;
	virtual void set_creation_tool_version( const wstring setting )= 0 ;

	virtual void set_modified_by(const wstring modified_by) = 0 ; 
	virtual void set_modified_on(const wstring modified_on) = 0 ; 
	virtual void set_modified_now()= 0 ;
	virtual void modified_by_current_user()= 0 ;

};
}
