/*!
* Implementation and declaration of CLocalizer class.
* 	created:	2007/01/29
* 	created:	29:1:2007   4:18
* 	filename: 	c:\dev\c++\Projects\WordAssist\WordAssist\Localizer.h
* 	file path:	c:\dev\c++\Projects\WordAssist\WordAssist
* 	file base:	Localizer
* 	file ext:	h
* 	author:		Ryan Ginstrom
* 	
* 	purpose:	
* 
*/
#pragma once

#include "Path.h"
#include "Broadcaster.h"
#include "Broker.h"
#include <set>
#include <boost/bind.hpp>
#import <msxml3.dll> named_guids

class CLocalizer
{
	typedef std::map< string, wstring > messageEntry ;
	typedef boost::shared_ptr< messageEntry > messagePtr ;
	typedef std::map< string, messagePtr > messageDictionary ;
	typedef std::pair< string, wstring > entryPair ;

	messageDictionary m_Messages ;
	string m_language ;
	std::set< string > m_languages ;

	/*!
	 * Used for localizing app strings.
	 */
	CLocalizer(void)
	{
		BANNER( "CLocalizer::CLocalizer" ) ;

		m_languages.insert( "en" ) ;
		m_languages.insert( "jp" ) ;

		CBroadcaster::instance().Subscribe( "app", "shutting down", boost::bind( &CLocalizer::handleShutdown, this, _1) ) ;

		file::CPath path = getFileName();
		if ( ! path.FileExists() )
		{
			ATLTRACE( _T("File %s does not exist\n"), (LPCTSTR)path.Path() ) ;
			return ;
		}

		CComPtr< MSXML2::IXMLDOMDocument2 > doc ;
		ATLVERIFY( SUCCEEDED( doc.CoCreateInstance( __uuidof( MSXML2::DOMDocument ) ) ) ) ;

		MSXML2::IXMLDOMNodeListPtr nodes = getStringNodes(path,doc);

		loadXmlTable(nodes);

		ATLTRACE( "There are %d entries\n", m_Messages.size() ) ;
	}
public:

	file::CPath getFileName()
	{
		file::CPath path ;
		// path.GetModulePath( _AtlModule.GetResourceInstance() ) ;
		path.Append( _T("stringdb.xml") ) ;
		return path ;
	}
	bool handleShutdown( boost::any &Param )
	{
		Param ;



		return false ; // continue processing
	}
	/*!
	 * \brief
	 * Write brief comment for getStringNodes here.
	 * 
	 * \param file::CPath &path
	 * \param CComPtr< MSXML2::IXMLDOMDocument2 > &doc
	 * Description of parameter file::CPath &path.
	 * Description of parameter CComPtr< MSXML2::IXMLDOMDocument2 > &doc.
	 * 
	 * \returns
	 * MSXML2::IXMLDOMNodeListPtr
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for getStringNodes here.
	 * 
	 * \remarks
	 * Write remarks for getStringNodes here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	MSXML2::IXMLDOMNodeListPtr getStringNodes( file::CPath &path, CComPtr< MSXML2::IXMLDOMDocument2 > &doc )
	{
		doc->validateOnParse = VARIANT_FALSE ;
		doc->async = VARIANT_FALSE ;

		_variant_t xmlSource( path.Path() ) ;
		doc->load( xmlSource ) ;

		return doc->getElementsByTagName( L"string" ) ;
	}
	/*!
	 * \brief
	 * Write brief comment for loadXmlTable here.
	 * 
	 * \param MSXML2::IXMLDOMNodeListPtr nodes
	 * Description of parameter MSXML2::IXMLDOMNodeListPtr nodes.
	 * 
	 * \returns
	 * void
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for loadXmlTable here.
	 * 
	 * \remarks
	 * Write remarks for loadXmlTable here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	void loadXmlTable( MSXML2::IXMLDOMNodeListPtr nodes )
	{
		string key ;
		wstring en, jp ;
		for ( int i = 0 ; i < nodes->length ; ++i )
		{
			MSXML2::IXMLDOMNodePtr node = nodes->item[i] ;

			messagePtr entry = messagePtr( new messageEntry() ) ;

			TRACE( i ) ;

			for ( int j = 0 ; j < node->childNodes->length ; ++j )
			{
				MSXML2::IXMLDOMNodePtr child = node->childNodes->item[j] ;
				wstring tagName = BSTR2wstring( child->nodeName ) ;
				if ( tagName == L"key" )
				{
					key = CW2A( child->text ) ;
					TRACE( key ) ;
				}
				else
				{
					ATLASSERT( tagName == L"val" ) ;
					_bstr_t langId = child->attributes->item[0]->nodeValue ;
					_bstr_t val = child->text ;

					string aLangId = BSTR2string( langId ) ;
					m_languages.insert( aLangId ) ;
					TRACE( langId ) ;
					TRACE( val ) ;
					entry->insert( entryPair( aLangId, BSTR2wstring( val ) ) ) ;
				}
			}

			m_Messages[key] = entry ;
		}
	}

	~CLocalizer(void)
	{
	}

	/*!
	 * \brief
	 * Factory method for CLocalizer class.
	 * 
	 * 
	 * \returns
	 * CLocalizer&
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Implements singleton pattern for our string localizer.
	 */
	static CLocalizer& instance()
	{
		static CLocalizer inst ;
		return inst ;
	}

	/*!
	 * \brief
	 * Stub for translation function.
	 * 
	 * \param string msgid
	 * Description of parameter string msgid.
	 * 
	 * \returns
	 * CString - The translated code
	 * 
	 * Write detailed description for translate here.
	 * 
	 * \remarks
	 * This function will look up translations in our map of msgid -> en/ja/etc.(?).
	 * 
	 * \see
	 * _
	 *
	 * \todo
	 * Fill in actual translation code
	 */
	CString translate( string msgid )
	{
		BANNER( "CLocalizer::translate" ) ;
		TRACE( msgid ) ;

		if ( m_Messages.find( msgid ) == m_Messages.end() )
		{
			loadDefaultTranslations(msgid);
			return defaultTrans(msgid) ;
		}
		if ( ! ensureLanguage() )
		{
			return defaultTrans(msgid) ;
		}
	
		messagePtr entry = m_Messages[msgid] ;

		if ( entry->find( m_language ) == entry->end() )
		{
			entry->insert( entryPair( m_language, string2wstring( msgid ) ) ) ;
			return defaultTrans(msgid) ;
		}


		messageEntry::iterator keyVal = entry->find( m_language ) ;
		wstring msg = keyVal->second ;
		TRACE( msg ) ;
		return CString( CW2T( msg.c_str() ) ) ;
	}
	/*!
	 * \brief
	 * Write brief comment for ensureLanguage here.
	 * 
	 * 
	 * \returns
	 * bool
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for ensureLanguage here.
	 * 
	 * \remarks
	 * Write remarks for ensureLanguage here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	bool ensureLanguage()
	{
		if ( ! m_language.empty() )
		{
			return true ;
		}

		boost::any languageParam ;
		if ( ! CBroker::instance().Request( "ActiveDocumentInterface", languageParam ) )
		{
			ATLTRACE( "Failed to retrieve preferred language!\n" ) ;
			return false ;
		}
		m_language = boost::any_cast<string>(languageParam) ;
		return m_language.empty() == false ;
	}

	/*!
	 * \brief
	 * Write brief comment for defaultTrans here.
	 * 
	 * \param string &msgid
	 * Description of parameter string &msgid.
	 * 
	 * \returns
	 * CString
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for defaultTrans here.
	 * 
	 * \remarks
	 * Write remarks for defaultTrans here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	CString defaultTrans( string &msgid )
	{
		return CString( CA2T( msgid.c_str() ) );
	}
	/*!
	 * \brief
	 * Write brief comment for loadDefaultTranslations here.
	 * 
	 * \param string msgid
	 * Description of parameter string msgid.
	 * 
	 * \returns
	 * void
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for loadDefaultTranslations here.
	 * 
	 * \remarks
	 * Write remarks for loadDefaultTranslations here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	void loadDefaultTranslations( string msgid )
	{
		messagePtr entry = messagePtr( new messageEntry() ) ;
		for ( std::set< string >::iterator pos = m_languages.begin() ; pos != m_languages.end() ; ++pos )
		{
			entry->insert( entryPair( *pos, string2wstring( msgid ) ) ) ;
		}
		m_Messages[msgid] = entry ;
	}
};

/*!
 * \brief
 * Gets the localized version of a string.
 * 
 * \param string msgid
 * The message id in the form of the English text.
 * 
 * \returns
 * CString - the localized string
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Grabs an instance of our singleton CLocalizer object and uses it to get the localized
 * version of the string.
 * 
 * \see
 * CLocalizer.
 */
inline CString _( string msgid )
{
	CLocalizer &localizer = CLocalizer::instance() ;
	return localizer.translate( msgid ) ;
}
