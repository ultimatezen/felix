// class_representation.cpp: implementation of the class_representation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "class_representation.h"
#include "textstream_reader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class_representation::class_representation()
{

}

class_representation::~class_representation()
{

}

class_representation::class_representation(const class_representation &rhs)
{
	internal_copy( rhs ) ;
}

class_representation& class_representation::operator =(const class_representation &rhs)
{
	internal_copy( rhs ) ;
	return *this ;
}

void class_representation::internal_copy(const class_representation &rhs)
{
	variable_list vars( rhs.m_variables ) ;
	m_variables.swap( vars ) ;

	method_list methods( rhs.m_methods ) ;
	m_methods.swap( methods ) ;
}

prog_lines::iterator class_representation::suss_class( prog_lines::iterator pos )
{
	bool is_into_class = false ;
	int brace_stack = 0 ;

	while ( true ) 
	{
		string line = *pos ;

		if ( line.find( "BEGIN_MSG_MAP" ) != string::npos ) 
		{
			while ( line.find( "END_MSG_MAP" ) == string::npos ) 
			{
				++pos ;
				line = *pos ;
			}
		}
		else if ( line.substr(0,2) == "//" ) 
		{

		}
		else if ( line.substr(0,2) == "/*" ) 
		{
			while ( line.find( "*/" ) != string::npos ) 
			{
				++pos ;
				line = *pos ;
			}
		}
		else
		{
			suss_line( line ) ;
			
			brace_stack += count_open_braces( line ) ;
			if ( brace_stack > 0 ) 
			{
				is_into_class = true ;
			}
			
			brace_stack -= count_close_braces( line ) ;
			if ( is_into_class && brace_stack <= 0 ) 
			{
				return pos ;			
			}
			
		}
		++pos ;
	}
}

void class_representation::suss_line(const string &line)
{
	if ( line.find_first_of( '(' ) != string::npos ) 
	{
		method_rep meth = suss_method( line ) ;
		if ( meth.m_name.empty() == false ) 
		{
			m_methods.push_back(meth) ;
		}
	}
	else 
	{
		variable_rep var = suss_variable( line ) ;
		if ( var.m_name.empty() == false ) 
		{
			m_variables.push_back( var ) ;
		}
	}

}

int class_representation::count_open_braces(const string &line)
{
	int num = 0 ;
	size_t pos = 0 ;
	pos = line.find_first_of( '{', pos ) ;
	while ( pos != string::npos ) 
	{
		num++ ;
		pos = line.find_first_of( '{', pos+1 ) ;
	}

	return num ;
}

int class_representation::count_close_braces(const string &line)
{
	int num = 0 ;
	size_t pos = 0 ;
	pos = line.find_first_of( '}', pos ) ;
	while ( pos != string::npos ) 
	{
		num++ ;
		pos = line.find_first_of( '}', pos+1 ) ;
	}
	
	return num ;
}

method_rep class_representation::suss_method(const string &line)
{
	static const char* stop_chars = " \t(),\r{}*&" ;
	
	textstream_reader< char > reader ;
	reader.set_buffer( line.c_str() ) ;
	
	method_rep meth ;
	string token ;
	
	reader.eat_whitespace() ;
	while ( reader.getline( token, stop_chars, false ) ) 
	{
		if ( token == "const" || token == "virtual" ) 
		{
			reader.eat_whitespace() ;
			reader.getline( token, stop_chars, false ) ;
		}
		
		meth.m_retval = token ;
		reader.eat_whitespace() ;
		if ( reader.current_is( '*' ) || reader.current_is('&') ) 
		{
			reader.get( meth.m_retval ) ;
		}
		reader.getline( meth.m_name, " \t(", true ) ;

		reader.eat_whitespace() ;
		if ( ! reader.current_is('(') )  
		{
			reader.jump_to_first_of( '(' ) ;
		}
		reader.advance( ) ;

		while ( ! reader.current_is(')') && ! reader.empty() ) 
		{
			reader.getline( token, ",)", false ) ;
			variable_rep var = suss_variable( token ) ;
			if ( var.m_name.empty() == false ) 
			{
				meth.m_variables.push_back( var ) ;
			}
			if ( reader.current_is(',') ) 
			{
				reader.advance( ) ;
			}
			reader.eat_whitespace() ;
		}

		return meth ;
	}

	return meth ;

}

variable_rep class_representation::suss_variable(const string &line)
{
	static const char* stop_chars = " \t(),\r{}*&" ;
	
	textstream_reader< char > reader ;
	reader.set_buffer( line.c_str() ) ;
	
	variable_rep var ;
	string token ;
	
	reader.eat_whitespace() ;
	while ( reader.getline( token, stop_chars, false ) ) 
	{
		if ( token == "typedef" || token == "enum" ) 
		{
			return var ;
		}
		if ( token == "private:" || token == "public:" || token == "protected:" ) 
		{
			return var ;
		}
		if ( token == "struct" || token == "class" || token == "protected:" ) 
		{
			return var ;
		}
		
		if ( token == "const" ) 
		{
			reader.eat_whitespace() ;
			reader.getline( token, stop_chars, false ) ;
		}
		
		var.m_type = token ;
		reader.eat_whitespace() ;
		if ( reader.current_is( '*' ) || reader.current_is('&') ) 
		{
			reader.get( var.m_type ) ;
		}
		reader.getline( var.m_name, stop_chars ) ;
		
		if ( reader.jump_to_first_of('=', true ) ) 
		{
			reader.eat_whitespace() ;
			reader.getline(token) ;
			var.m_default = token ;
		}

		return var ;
	}

	return var ;
}
