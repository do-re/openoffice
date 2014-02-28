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

#ifndef PPTEXOOXML_EXPORT_HXX
#define PPTEXOOXML_EXPORT_HXX

#include <oox/core/xmlfilterbase.hxx>
#include <com/sun/star/uno/XComponentContext.hpp>
#include "pptexpbase.hxx"

namespace com { namespace sun { namespace star {
    namespace animations {
        class XAnimate;
        class XAnimationNode;
    }
} } }

class PPTXExport : public oox::core::XmlFilterBase, PPTEXPBase
{
public:

    PPTXExport( const com::sun::star::uno::Reference< com::sun::star::uno::XComponentContext >& xCtx );

    virtual ~PPTXExport();

    // from FilterBase
    virtual bool importDocument() throw();
    virtual bool exportDocument() throw();

   
    virtual sal_Int32 getSchemeClr( sal_Int32 /* nColorSchemeToken */ ) const { return 0; }
    virtual oox::vml::Drawing* getVmlDrawing() { return NULL; }
    virtual const oox::drawingml::Theme* getCurrentTheme() const { return NULL; }
    virtual const oox::drawingml::table::TableStyleListPtr getTableStyles() { return oox::drawingml::table::TableStyleListPtr(); }
    virtual oox::drawingml::chart::ChartConverter& getChartConverter() {  return * (oox::drawingml::chart::ChartConverter*) NULL; }

private:
    virtual ::rtl::OUString implGetImplementationName() const;

    ::sax_fastparser::FSHelperPtr mPresentationFS;

    static const char* GetSideDirection( sal_uInt8 nDirection );
    static const char* GetCornerDirection( sal_uInt8 nDirection );
    static const char* Get8Direction( sal_uInt8 nDirection );
    static       int   GetPPTXLayoutId( int nOffset );

    /** Derived classes create a VBA project manager object. */
    virtual ::oox::ole::VbaProject* implCreateVbaProject() const;

protected:

    virtual void ImplWriteSlide( sal_uInt32 nPageNum, sal_uInt32 nMasterNum, sal_uInt16 nMode,
                                 sal_Bool bHasBackground, ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet > aXBackgroundPropSet );
    virtual void ImplWriteNotes( sal_uInt32 nPageNum ){}
//    virtual void ImplWriteSlideMaster( sal_uInt32 nPageNum, ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet > aXBackgroundPropSet );
    virtual void ImplWriteLayout( sal_Int32 nOffset, sal_uInt32 nMasterNum ){}
    void ImplWritePPTXLayout( sal_Int32 nOffset, sal_uInt32 nMasterNum );
    void WriteTheme( sal_Int32 nThemeNum );
    virtual sal_Bool ImplCreateMainNotes(){return sal_True;}
    virtual sal_Bool ImplCreateDocument();
//    virtual sal_Bool ImplCreateMainNotes();
    sal_Bool WriteNotesMaster();
	
    void WriteAnimateTo( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Any aValue, const ::rtl::OUString& rAttributeName );
    void WriteAnimateValues( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Reference< ::com::sun::star::animations::XAnimate >& rXAnimate );
    void WriteAnimationCondition( ::sax_fastparser::FSHelperPtr pFS, ::com::sun::star::uno::Any& rAny, sal_Bool bWriteEvent, sal_Bool bMainSeqChild );
    void WriteAnimationCondition( ::sax_fastparser::FSHelperPtr pFS, const char* pDelay, const char* pEvent, double fDelay, sal_Bool bHasFDelay );
    void WriteAnimations( ::sax_fastparser::FSHelperPtr pFS );
    void WriteAnimationAttributeName( ::sax_fastparser::FSHelperPtr pFS, const ::rtl::OUString& rAttributeName );
    void WriteAnimationNode( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Reference< ::com::sun::star::animations::XAnimationNode >& rXNode, sal_Bool bMainSeqChild );
    void WriteAnimationNodeAnimate( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Reference< ::com::sun::star::animations::XAnimationNode >& rXNode, sal_Int32 nXmlNodeType, sal_Bool bMainSeqChild );
    void WriteAnimationNodeAnimateInside( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Reference< ::com::sun::star::animations::XAnimationNode >& rXNode, sal_Bool bMainSeqChild, sal_Bool bSimple );
    void WriteAnimationNodeSeq( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Reference< ::com::sun::star::animations::XAnimationNode >& rXNode, sal_Int32 nXmlNodeType, sal_Bool bMainSeqChild );
    void WriteAnimationNodeEffect( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Reference< ::com::sun::star::animations::XAnimationNode >& rXNode, sal_Int32 nXmlNodeType, sal_Bool bMainSeqChild );
    void WriteAnimationNodeCommonPropsStart( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Reference< ::com::sun::star::animations::XAnimationNode >& rXNode, sal_Bool bSingle, sal_Bool bMainSeqChild );
    void WriteAnimationNodeCommonPropsEnd( ::sax_fastparser::FSHelperPtr pFS );
    void WriteAnimationProperty( ::sax_fastparser::FSHelperPtr pFS, const ::com::sun::star::uno::Any& rAny );
    void WriteAnimationTarget( ::sax_fastparser::FSHelperPtr pFS, ::com::sun::star::uno::Any aTarget );
    void WriteTextStyles( ::sax_fastparser::FSHelperPtr pFS );
    void WriteTextStyle( ::sax_fastparser::FSHelperPtr pFS, int nInstance, sal_Int32 xmlToken );
    void WriteTextStyleLevel( ::sax_fastparser::FSHelperPtr pFS, int nInstance, int nLevel );
    void ImplWriteBackground( ::sax_fastparser::FSHelperPtr pFS, ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet > aXBackgroundPropSet );
    void WriteTransition( ::sax_fastparser::FSHelperPtr pFS );

    sal_Int32 GetLayoutFileId( sal_Int32 nOffset, sal_uInt32 nMasterNum );

    // shapes
    void WriteShapeTree( ::sax_fastparser::FSHelperPtr pFS, PageType ePageType, sal_Bool bMaster );

    sal_uInt32 GetNewSlideId() { return mnSlideIdMax ++; }
    sal_uInt32 GetNewSlideMasterId() { return mnSlideMasterIdMax ++; }

private:
//    LayoutInfo mLayoutInfo[EPP_LAYOUT_SIZE];
    std::vector< ::sax_fastparser::FSHelperPtr > mpSlidesFSArray;
    std::vector< ::sax_fastparser::FSHelperPtr > mpMasterFSArray;
//    sal_Int32 mnLayoutFileIdMax;

    sal_uInt32 mnSlideIdMax;
    sal_uInt32 mnSlideMasterIdMax;
    sal_uInt32 mnAnimationNodeIdMax;

    sal_Bool mbCreateNotes;

    static sal_Int32 nStyleLevelToken[5];
};

#endif
