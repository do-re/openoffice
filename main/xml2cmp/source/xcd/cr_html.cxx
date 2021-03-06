/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#include <fstream>
#include "cr_html.hxx"
#include "xmltree.hxx"
#include "../support/syshelp.hxx"




char C_sHtmlFileHeader1[] =
	"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n"
	"<HTML>\n"
	"<HEAD>\n"
	"	<TITLE>";

char C_sHtmlFileHeader2[] =
		"</TITLE>\n"
	"		<META NAME=\"GENERATOR\" CONTENT=\"xml2cmp\">\n"
	"</HEAD>\n"
	"<BODY BGCOLOR=\"#ffffff\">\n<P><BR></P>";


char C_sHtmlFileFoot[] = "</BODY>\n</HTML>\n";


HtmlCreator::HtmlCreator( const char *		  		i_pOutputFileName,
						  const XmlElement &	    i_rDocument,
						  const Simstr &		    i_sIDL_BaseDirectory )
	:	aFile(i_pOutputFileName, std::ios::out
#if defined(WNT) || defined(OS2)
											   | std::ios::binary
#endif
															 ),
		rDocument(i_rDocument),
        sIdl_BaseDirectory(i_sIDL_BaseDirectory)
{
	if ( !aFile )
	{
		std::cerr << "Error: " << i_pOutputFileName << " could not be created." << std::endl;
		exit(0);
	}
}

HtmlCreator::~HtmlCreator()
{
	aFile.close();
}

void
HtmlCreator::Run()
{
	WriteStr( C_sHtmlFileHeader1 );
	WriteStr( "ModuleDescription" );
	WriteStr( C_sHtmlFileHeader2 );

	rDocument.Write2Html(*this);

	WriteStr( "<P><BR><BR></P>\n" );
	WriteStr( C_sHtmlFileFoot );
}

void
HtmlCreator::StartTable()
{
	WriteStr( "<P><BR></P>\n" );
	WriteStr(
			"<TABLE WIDTH=95% BORDER=1 CELLSPACING=0 CELLPADDING=4>\n"
			"	<TBODY>\n"	);
}

void
HtmlCreator::FinishTable()
{
	WriteStr( "	</TBODY>\n"
			  "</TABLE>\n\n" );
}

void
HtmlCreator::StartBigCell( const char *	i_sTitle )
{
	WriteStr( "<TR><TD COLSPAN=2>\n"
			  "<H4><BR>" );
	WriteStr( i_sTitle );
	WriteStr( "</H4>\n" );

}

void
HtmlCreator::FinishBigCell()
{
	WriteStr( "</TD><TR>\n" );
}

void
HtmlCreator::Write_SglTextElement( const SglTextElement &  i_rElement,
								   bool                    i_bStrong )
{
	StartRow();

	WriteElementName( i_rElement.Name(), i_bStrong );

	StartCell( "77%");
	if (i_bStrong)
	{
		WriteStr( "<H4><A NAME=\"" );
		unsigned nLen = strlen(i_rElement.Data());
		if ( i_rElement.IsReversedName())
		{
			const char * pEnd = strchr(i_rElement.Data(), ' ');
			nLen = (unsigned)( pEnd - i_rElement.Data() );
		}
		aFile.write( i_rElement.Data(), (int) nLen );
		WriteStr( "\">" );
	}

	WriteName( aFile, sIdl_BaseDirectory, i_rElement.Data(),
			   i_bStrong ? lt_nolink : i_rElement.LinkType() );

	if (i_bStrong)
		WriteStr( "</A></H4>" );
	FinishCell();

	FinishRow();
}

void
HtmlCreator::Write_MultiTextElement( const MultipleTextElement & 	i_rElement )
{
	StartRow();

	WriteElementName( i_rElement.Name(), false );

	StartCell( "77%");
	unsigned i_max = i_rElement.Size();
	for ( unsigned i = 0; i < i_max; ++i )
	{
		if (i > 0)
			WriteStr( "<BR>\n" );
		WriteName( aFile, sIdl_BaseDirectory, i_rElement.Data(i), i_rElement.LinkType() );
	}	// end for
	FinishCell();

	FinishRow();
}

void
HtmlCreator::Write_SglText(	const Simstr &      i_sName,
							const Simstr &		i_sValue )
{
	StartRow();

	WriteElementName( i_sName, false );

	StartCell( "77%");
	WriteStr( i_sValue );
	FinishCell();

	FinishRow();
}

void
HtmlCreator::Write_ReferenceDocu( const Simstr &      i_sName,
								  const Simstr &	  i_sRef,
								  const Simstr &	  i_sRole,
								  const Simstr &	  i_sTitle )
{
	StartRow();

	StartCell( "23%" );
	WriteStr(i_sName);
	FinishCell();

	StartCell( "77%" );
	if ( !i_sRef.is_empty() )
	{
		WriteStr("<A href=\"");
		WriteStr(i_sRef);
		WriteStr("\">");
		if ( !i_sTitle.is_empty() )
			WriteStr( i_sTitle );
		else
			WriteStr(i_sRef);
		WriteStr("</A><BR>\n");
	}
	else if ( !i_sTitle.is_empty() )
	{
		WriteStr("Title: ");
		WriteStr( i_sTitle );
		WriteStr("<BR>\n");
	}
	if ( !i_sRole.is_empty() )
	{
		WriteStr("Role: ");
		WriteStr( i_sRole );
	}
	FinishCell();

	FinishRow();
}


void
HtmlCreator::StartRow()
{
	WriteStr( "		<TR VALIGN=TOP>\n" );
}

void
HtmlCreator::FinishRow()
{
	WriteStr( "		</TR>\n" );
}

void
HtmlCreator::StartCell( const char * i_pWidth)
{
	WriteStr( "			<TD WIDTH="	);
	WriteStr( i_pWidth );
	WriteStr( ">\n				<P>" );
}

void
HtmlCreator::FinishCell()
{
	WriteStr( "</P>\n			</TD>\n" );
}

void
HtmlCreator::WriteElementName( const Simstr & i_sName,
							   bool           i_bStrong )
{
	StartCell( "23%" );
	if (i_bStrong)
		WriteStr( "<H4>" );
	WriteStr(i_sName);
	if (i_bStrong)
		WriteStr( "</H4>" );
	FinishCell();
}



