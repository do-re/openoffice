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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sw.hxx"
#include <SwXMLBlockImport.hxx>
#include <xmloff/nmspmap.hxx>
#include <xmloff/xmlnmspe.hxx>
#ifndef _SW_XMLBLOCKLIST_CONTEXT_HXX
#include <SwXMLBlockListContext.hxx>
#endif
#include <SwXMLTextBlocks.hxx>
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star;
using namespace ::xmloff::token;
using ::rtl::OUString;

sal_Char __READONLY_DATA sXML_np__block_list[] = "_block-list";
sal_Char __READONLY_DATA sXML_np__office[] = "_ooffice";
sal_Char __READONLY_DATA sXML_np__text[] = "_otext";

// #110680#
SwXMLBlockListImport::SwXMLBlockListImport(
    const uno::Reference< lang::XMultiServiceFactory > xServiceFactory,
	SwXMLTextBlocks &rBlocks )
:	SvXMLImport( xServiceFactory, 0 ),
	rBlockList (rBlocks)
{
	GetNamespaceMap().Add( OUString ( RTL_CONSTASCII_USTRINGPARAM ( sXML_np__block_list ) ),
						   GetXMLToken ( XML_N_BLOCK_LIST ),
						   XML_NAMESPACE_BLOCKLIST );
}

SwXMLBlockListImport::~SwXMLBlockListImport ( void )
	throw ()
{
}

SvXMLImportContext *SwXMLBlockListImport::CreateContext(
		sal_uInt16 nPrefix,
		const OUString& rLocalName,
		const uno::Reference< xml::sax::XAttributeList > & xAttrList )
{
	SvXMLImportContext *pContext = 0;
	if ( XML_NAMESPACE_BLOCKLIST == nPrefix &&
		 IsXMLToken ( rLocalName, XML_BLOCK_LIST ) )
		pContext = new SwXMLBlockListContext( *this, nPrefix, rLocalName,
											  xAttrList );
	else
		pContext = SvXMLImport::CreateContext( nPrefix, rLocalName, xAttrList );
	return pContext;
}

// #110680#
SwXMLTextBlockImport::SwXMLTextBlockImport(
    const uno::Reference< lang::XMultiServiceFactory > xServiceFactory,
	SwXMLTextBlocks &rBlocks,
	String & rNewText,
	sal_Bool bNewTextOnly )
:	SvXMLImport(xServiceFactory, IMPORT_ALL ),
	rBlockList ( rBlocks ),
	bTextOnly ( bNewTextOnly ),
	m_rText ( rNewText )
{
	GetNamespaceMap().Add( OUString( RTL_CONSTASCII_USTRINGPARAM ( sXML_np__office ) ),
							GetXMLToken(XML_N_OFFICE_OOO),
							XML_NAMESPACE_OFFICE );
	GetNamespaceMap().Add( OUString( RTL_CONSTASCII_USTRINGPARAM ( sXML_np__text ) ),
							GetXMLToken(XML_N_TEXT_OOO),
							XML_NAMESPACE_TEXT );
}

SwXMLTextBlockImport::~SwXMLTextBlockImport ( void )
	throw()
{
}

SvXMLImportContext *SwXMLTextBlockImport::CreateContext(
		sal_uInt16 nPrefix,
		const OUString& rLocalName,
		const uno::Reference< xml::sax::XAttributeList > & xAttrList )
{
	SvXMLImportContext *pContext = 0;
	if( XML_NAMESPACE_OFFICE == nPrefix &&
		IsXMLToken ( rLocalName, bTextOnly ? XML_DOCUMENT : XML_DOCUMENT_CONTENT ) )
		pContext = new SwXMLTextBlockDocumentContext( *this, nPrefix, rLocalName, xAttrList );
	else
		pContext = SvXMLImport::CreateContext( nPrefix, rLocalName, xAttrList );
	return pContext;
}
void SAL_CALL SwXMLTextBlockImport::endDocument(void)
        throw( xml::sax::SAXException, uno::RuntimeException )
{
}
