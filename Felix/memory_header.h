/*!
	@file memory_header.h
	@brief interface for the memory_header class.
	@date 2005/06/25
	Time: 14:48:55
	@author Ryan Ginstrom
 */

#pragma once

#include "File.h"
#include "MiscWrappers.h"
#include "output_device.h"
#include "memory_info.h"

namespace mem_engine
{

/** Wrapper for memory header.

	Stores information about the TM, such as creator, creation tool, etc.
	Mostly useful for import/export and Memory Manager.
 */
	class memory_header : public MemoryInfo
	{
		wstring			m_creator ;
		wstring			m_field ;
		wstring			m_created_on ;
		wstring			m_source_language ;
		wstring			m_target_language ;
		wstring			m_client ;

		wstring			m_creation_tool ;
		wstring			m_creation_tool_version ;

		long			m_count ;
		bool			m_is_locked ;
		bool			m_is_memory ;

		wstring			m_modified_by ;
		wstring			m_modified_on ;
	public:
		memory_header(const memory_header &rhs);
		memory_header();
		virtual ~memory_header();
		memory_header &operator=(const memory_header &rhs);


		// Getters

		/** Gets the name of the current user.

			The user can set this name in the preferences, otherwise the
			account name will be used.
		*/
		wstring get_current_user();
		/// Gets the name of the creator.
		wstring get_creator() const ;
		/// Gets the field (electronics, medicine, etc.)
		wstring get_field() const ;
		/// Gets the date that the TM was created.
		wstring get_created_on() const ;

		/// Gets the name of the user who last modified the TM.
		wstring get_modified_by() const { return m_modified_by ; }
		/// Gets the date that the TM was last modified.
		wstring get_modified_on() const { return m_modified_on ; }

		wstring get_source_language() const ;
		wstring get_target_language() const ;
		wstring get_client() const ;
		wstring get_creation_tool() const ;
		wstring get_creation_tool_version() const ;
		/// Gets the number of records in the TM.
		long get_count() const ;
		/// Gets whether the TM is locked. Locked TMs can't be modified.
		bool is_locked() const ;
		/** Gets whether this TM is a memory.

			@return true if it is a memory, false if it is a glossary.
		*/
		bool is_memory() const ;

		// Setters

		/// Sets the name of the user who created the TM.
		void set_creator( const wstring setting ) ;
		/// Sets the field (electronics, medicine, etc.)
		void set_field( const wstring setting ) ;
		/// Sets the date that the TM was created.
		void set_created_on( const wstring setting ) ;
		/// Sets the source language.
		void set_source_language( const wstring setting ) ;
		/// Sets the target language.
		void set_target_language( const wstring setting ) ;
		/// Sets the name of the client (customer).
		void set_client( const wstring setting ) ;
		/// Sets the number of records in the TM.
		void set_count( const long setting ) ;
		/// Locks the TM.
		void set_locked_on( ) ;
		/// Unlocks the TM.
		void set_locked_off( ) ;
		/// Sets TM type to memory.
		void set_is_memory_on() ;
		/// Sets TM type to glossary.
		void set_is_memory_off() ;
		/** Sets the TM type.
			true is for memory, false is for glossary.
		*/
		void set_is_memory(bool setting);
		/** Sets whether the TM is locked.
			Locked TMs can't be modified.
		*/
		void set_is_locked(bool setting);
		/// Sets the tool used to create the TM. Used for import/export.
		void set_creation_tool( const wstring setting );
		/// Sets the version of the tool used to create the TM. Used for import/export.
		void set_creation_tool_version( const wstring setting );

		/// Sets the user who last modified the TM.
		void set_modified_by(const wstring modified_by) { m_modified_by = modified_by ; }
		/// Sets the date the TM was last modified on.
		void set_modified_on(const wstring modified_on) { m_modified_on = modified_on ; }
		/// Sets the modified date to the current date/time.
		void set_modified_now();
		/// Sets the modified-by value to the current user.
		void modified_by_current_user();
		/// Sets the created-by value to the current user.
		void set_creator_to_current_user();

		// read/write -- let's split this out of the class.
		void read_header(LPCWSTR text) ;
		void write_header(OutputDevice *out_file) ;
		wstring get_header_field(wc_reader &reader, LPCWSTR tag) ;
		void fill_value(wc_reader &reader, LPCWSTR tag, wstring &val);
		void fill_value(wc_reader &reader, LPCWSTR tag, bool &val);

	private:
		void internal_copy( const memory_header &rhs );

		void set_created_on_to_local_time();
	};

}
