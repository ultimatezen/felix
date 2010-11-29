// (c) 2000 Microsoft Corporation
//Regular Expression helper class
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

#define S_PARSE_FAILED 0x2
#define S_NO_MATCH 0x3
#define S_INVALID_REPLACE_EXP 0x4

#include "atlstr.h"
#include <atlrx.h>


template <class CharTraits=CAtlRECharTraitsW>
class CRxUtil
{
private:

	
CAtlRegExp<CharTraits> atlRegExp;


	/*
		Helper function for s(), substitutes variables($N)
	*/
	HRESULT SubstituteVars(CStringW &original,CAtlREMatchContext<CharTraits> &atlRematchContext){
		CAtlREMatchContext<CharTraits>::MatchGroup tempGroup;
		HRESULT hr=S_OK;
		char *buffer;
		buffer = new char[(atlRematchContext.m_uNumGroups/10)+2];
		CStringW temp,variableid;
		UINT i,y;
		for(i=1;i<=atlRematchContext.m_uNumGroups;i++){
			itoa(i,buffer,10);
			temp="";
			atlRematchContext.GetMatch(i-1,&tempGroup);
			for(y=0;y+tempGroup.szStart<tempGroup.szEnd;y++){
				temp=temp+tempGroup.szStart[y];
			}

			variableid="{$";
			variableid=variableid+buffer+"}";
			hr=s(original,variableid,temp,true,false);
			if(hr!=S_OK&&hr!=S_NO_MATCH)
				return hr;
		}
		delete[] buffer;

		return hr;


	}

	/*
		Helper for FindAllInstances
	*/
	CStringW getFoundat(int pos,CAtlREMatchContext<CharTraits> &atlRematchContext)
	{
		CStringW temp;
		temp=L"";
		int y;
		for(y=0;y+atlRematchContext.m_pMatches[pos].szStart<atlRematchContext.m_pMatches[pos].szEnd;y++)
		{
				temp=temp+atlRematchContext.m_pMatches[pos].szStart[y];
		}
		return temp;
	}

public:
	typedef typename CharTraits::RECHARTYPE RECHAR;

	/*
	Regular expression substitution:

	baseStr:

		Original String,

	oldexp:
		
		Regular expression to look for inside 'baseStr'

	newexp:
	
		Instances of 'oldexp' inside 'baseStr' will be replaced by this

	global:

		Substitute all instances of 'oldexp' found or only the first one

	usingvars:
		
		Use variables in replacements, you can specify variables using $(var#) (see example)

	casesensitive:

		Case sensitive or insesitive search for 'oldexp'


	Example:

		baseStr			="Hello World! Goodbye World!"
		oldexp			="{.*?}World{.}"
		newexp			="<b>$1</b>World?"
		global			=true
		usingvars		=true
		casesensitive	=true

		the output would be:

			"<b>Hello </b>World?<b> Goodbye </b>World?"

	
	*/
	HRESULT s(CStringW &baseStr,CStringW oldexp,CStringW newexp,const bool &global=false,const bool &usingvars=false,const bool &casesensitive=true){

		HRESULT hr;

		CAtlREMatchContext<CharTraits> atlRematchContext;
		CStringW substitutedexpr,strRightSide;
		if(atlRegExp.Parse(oldexp,casesensitive)==REPARSE_ERROR_OK)
			if(	atlRegExp.Match(baseStr,&atlRematchContext)){
				strRightSide=atlRematchContext.m_Match.szEnd;
				if(usingvars){
					substitutedexpr=newexp;
					SubstituteVars(substitutedexpr,atlRematchContext);
				}
				else
					substitutedexpr=newexp;
				if(global){
					hr=s(strRightSide,oldexp,newexp,true,usingvars,casesensitive);	//substitute recursivly
					if(hr!=S_OK&&hr!=S_NO_MATCH)
						return hr;		
				}
				baseStr=baseStr.Left(baseStr.GetLength()-wcslen(atlRematchContext.m_Match.szStart))+substitutedexpr+strRightSide;
				return S_OK;
			}
			else
				return S_NO_MATCH;
		else
			return S_PARSE_FAILED;
			
		
	

	}
	/*
		Will return in 'allinstances' every occurrance it finds of 'oldexp' inside 'basestring'
	*/	
	int FindAllInstances(CAtlArray<CStringW> &allinstances,CStringW basestring,CStringW oldexp){
		int count=0;
		atlRegExp.Parse(oldexp);
		CAtlREMatchContext<CharTraits> atlRematchContext;
		while(atlRegExp.Match(basestring,&atlRematchContext)){
			count++;
			allinstances.Add(getFoundat(0,atlRematchContext));
			basestring=atlRematchContext.m_Match.szEnd;
		}
		return count;

	}

	/*
		Will return the occurrence number 'pos' of 'oldexp' in finds inside 'basestring'
	*/
	CStringW GetInstanceNum(int pos,CStringW basestring,CStringW oldexp){

		atlRegExp.Parse(oldexp);
		CAtlREMatchContext<CharTraits> atlRematchContext;
		for(int i=1;i<=pos;i++){
			if(atlRegExp.Match(basestring,&atlRematchContext)){
				if(i==pos)
					return getFoundat(0,atlRematchContext);
				else basestring=atlRematchContext.m_Match.szEnd;
			}
			else break;
		}
		return "";
		

	}


	CRxUtil(void){
	}
	~CRxUtil(void){

		}



};