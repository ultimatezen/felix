
#pragma once
#include "html_processing.h"
#include <list>

/** class for formatting HTML text */
class html_formatter : public html_processing 
{
	typedef int						tag_type ;
	typedef std::stack< tag_type >	tag_stack ;
	typedef std::list< tag_type >	tag_buffer ;
	
	tag_buffer	m_tag_buffer ;
	tag_buffer	m_tags_to_close ;
	
	wstring		m_text ;
	wstring		m_current_buffer ;
	
	int			m_heading_level ;

public:

	/** brief. 
	@param setting parameter. 
	*/
	void set_superscript( bool setting );

	/** brief. 
	@param setting parameter. 
	*/
	void set_subscript( bool setting );

	/** brief. 
	@param valign parameter. 
	*/
	void set_v_alignment( VERT_ALIGNMENT valign );

	/** brief. 
	@return void description. 
	*/
	void clear_fore_color();

	/** brief. 
	@param level parameter. 
	*/
	void set_heading_level( size_t level );

	/** initialize the formatter. 
	*/
	void init();


	/** default constructor. 
	*/
	html_formatter();

	/** destructor. 
	*/
	virtual ~html_formatter();


	/** brief. 
	@return const wstring description. 
	*/
	const wstring get_paragraph_text() ;

	/** get formatted text. 
	@return const wstring description. 
	*/
	const wstring get_text() ;


	/** brief. 
	Specify size in points.
	Specify -1 to clear font-size information.
	@return void description. 
	@param font_size parameter. 
	*/
	void set_size( const int font_size ) ;

	/** default constructor. 
	*/
	void set_bold( const bool setting ) ;	

	/** brief. 
	@param setting parameter. 
	*/
	void set_italic( const bool setting ) ;

	/** brief. 
	@param setting parameter. 
	*/
	void set_underline( const bool setting ) ;

	/** Set the foreground color.
	@param color
	*/
	void set_fore_color( const CColorRef &color ) ;	
	// specify empty string to clear font-face information

	/** brief. 
	@return void description. 
	@param &font_name parameter. 
	*/
	void set_font_name( const wstring &font_name ) ;

	/** brief. 
	@return void description. 
	@param just_setting parameter. 
	*/
	void set_justification( const JUSTIFICATION just_setting );


	/** brief. 
	@return void description. 
	*/
	void add_paragraph_break() ;

	/** brief. 
	@return void description. 
	*/
	void add_linebreak() ;

	/** brief. 
	@return void add_text description. 
	@param &text parameter. 
	*/
	void add_text ( const wstring &text ) ;

private:

	/** brief. 
	@return bool description. 
	@param tag parameter. 
	*/
	bool tag_needs_closing( const tag_type tag );

	/** brief. 
	@return void description. 
	@param tag parameter. 
	*/
	void add_tag( const tag_type tag ) ;

	/** brief. 
	@return void description. 
	*/
	void flush_buffer() ;

	/** brief. 
	@return void description. 
	@param tag_to_check parameter. 
	*/
	void unwind_stack_until( const tag_type tag_to_check ) ;

	/** default constructor. 
	*/
	void close_off_tags() ;	

	/** brief. 
	@return void description. 
	@param tag_to_close parameter. 
	*/
	void close_off_tag( const tag_type tag_to_close ) ;

	/** brief. 
	@return void description. 
	*/
	void flush_tag_buffer();

	/** brief. 
	@return void description. 
	@param tag parameter. 
	*/
	void remove_from_tag_buffer( const tag_type tag );

} ;

