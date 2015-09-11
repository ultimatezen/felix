#pragma once

using std::wstring ;

namespace mem_engine
{

	//! Function object for sorting glossary matches
	class GlossMatchComparator
	{
	public:
		bool operator()(const std::wstring& s1, const std::wstring& s2)
		{
			if (s1.size() > s2.size())
			{
				return true ;
			}
			return s1 > s2 ;
		}
	};

	typedef std::set<std::wstring, GlossMatchComparator> gloss_match_set ;

	namespace placement
	{
		typedef std::pair< std::wstring, std::wstring > trans_pair ;
		wchar_t narrow_num( wchar_t c ) ;

		void fix_match_spans(wstring &segment);
		void fix_html_entities(wstring &segment);
		bool is_num_rep(std::wstring& PotentialNum);
		bool is_substitution(trans_pair& trans, 
							 std::wstring& SourceNum, 
							 size_t TransPos, 
							 std::wstring& QueryNum);
		int is_num_or_null( wchar_t c );

		enum MatchType { 
			MATCH = 1<<0, 
			NOMATCH = 1<<1, 
			PLACEMENT = 1<<2, 
			GLOSS_MATCH = 1<<3 
		} ;
		enum CharType { SOURCE, QUERY } ;

		/*!
		Internal structure of character pairs.

		Conceptually, it takes the following form:
		SOURCE_CHAR	MATCH_TYPE	QUERY_CHAR

		Where:
		SOURCE_CHAR = source string character (0 for epsilon)
		MATCH_TYPE = indicator of how chars are matched (match, no match, placement)
		QUERY_CHAR = query string character (0 for epsilon)
		*/
		struct pairing_entity
		{
			wchar_t m_Chars[2] ;
			MatchType m_MatchType ;

			pairing_entity(wchar_t s, MatchType mt, wchar_t q ):
			m_MatchType(mt)
			{
				m_Chars[SOURCE] = s ;
				m_Chars[QUERY] = q ;
			}

			//! Returns the match type
			MatchType match_type()
			{
				return m_MatchType ;
			}
			//! Adds a match type to the pairing
			//! Match types are a union
			void add_match_type(MatchType m_type)
			{
				this->m_MatchType = static_cast<MatchType>(this->match_type() | m_type) ;
			}
			//! Gets the source character
			wchar_t& source()
			{
				return get_char(SOURCE) ; 
			}
			//! Gets the query character
			wchar_t& query()
			{
				return get_char(QUERY) ; 
			}
			//! Gets the character of type `index`
			wchar_t& get_char(const CharType &index)
			{
				return m_Chars[index] ;
			}
			//! Get character, keeping const-correctness
			const wchar_t get_char(const CharType &index) const
			{
				return m_Chars[index] ;
			}
		} ;
		typedef std::list< pairing_entity > pair_list ;
		typedef std::vector< pairing_entity > pairings_t ;
		typedef pairing_entity pairing_t ;

		/** Adds the buffer we have stored to the marked up string.
		*/	
		wstring add_buffer_to_markup(MatchType MatchState, const wstring buffer, CharType ct);

		/** Marks up a string.
		* 
		* Keep accumulating characters in a buffer until the match type
		* changes, and then dump it into our markup string.
		*/
		wstring mark_up( pairings_t &pairs, CharType ct );

		/** Are we looking at a pair of number characters?
		*/
		int is_num_pair(pairing_entity& pe);
		
		/** Returns whether the pairing query has a gloss match at the
		 * specified position.
		 */
		bool has_gloss_match_in_query_at(pairings_t &pairs, const wstring gloss, const size_t pos) ;
		/** Marks up gloss match in query as specified pos
		 */
		void mark_gloss_match_in_query_at(pairings_t &pairs, const wstring gloss, const size_t pos) ;

		/* Marks up gloss matches appearing in query
		 */
		void mark_up_gloss_matches(pairings_t &pairs, const gloss_match_set &gloss_entries) ;
		/** Calculates the score based on our pairings.
		*/

		//! Calculates the score after applying matches
		double calc_score(pairings_t &pairs);
		//! Calculates the score for a glossary pair
		double calc_score_gloss(pairings_t &pairs);

		/*!
		* Represents a pairing of match strings.
		*/
		class match_string_pairing
		{
		public:

			pair_list m_pairs ; //!< List of source/query pairs
			pairings_t m_pairvec ; //!< Vector of source/query pairs (for copying)

			match_string_pairing(void);

			//! Retrieve a copy of the pairings
			pairings_t &get();
			//! Set the pairings
			void set(pairings_t &pairs);
			//! Clear the pairing list
			void clear();

			//! Match a source character to null
			void source_to_epsilon( wchar_t s );
			//! Match a query character to null
			void query_to_epsilon( wchar_t q );
			//! Match a source character to a query character
			void match( wchar_t s, wchar_t q );
			//! Mark a source and query character as mismatched
			void no_match( wchar_t s, wchar_t q );
			//! Mark up the source string in HTML according to matches
			wstring mark_up_source();
			//! Mark up the query string in HTML according to matches
			wstring mark_up_query();

		};

	}
}