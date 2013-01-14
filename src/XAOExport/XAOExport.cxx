// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

// File:        XAOExport.cxx
// Created:     Thu Dec 04 08:00:00 2012
// Author:      Nathalie Gore
//              <nathalie.gore@opencascade.com>
//
#include "utilities.h"

#include <BRepTools.hxx>

#include <TCollection_AsciiString.hxx>
#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>

#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
using namespace std;

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <SUIT_Desktop.h>
#include <SUIT_Session.h>
#include <SUIT_MessageBox.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_ViewManager.h>
#include <SUIT_OverrideCursor.h>
#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <LightApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <SALOME_ListIO.hxx>
#include <SALOME_ListIteratorOfListIO.hxx>

#include <GeometryGUI.h>
#include <GEOMBase.h>
#include <GEOMBase_Helper.h>
#include <GEOMBase_Skeleton.h>
#include <GEOMImpl_Types.hxx>

#include <GEOM_Engine.hxx>

#include <GEOMImpl_Gen.hxx>
#include <GEOMImpl_IShapesOperations.hxx>

#ifdef WNT
 #if defined XAOEXPORT_EXPORTS || defined XAOExport_EXPORTS
  #if defined WIN32
   #define XAOEXPORT_EXPORT __declspec( dllexport )
  #else
   #define XAOEXPORT_EXPORT
  #endif
 #else
  #if defined WIN32
   #define XAOEXPORT_EXPORT __declspec( dllimport )
  #else
   #define XAOEXPORT_EXPORT
  #endif
 #endif
#else
 #define XAOEXPORT_EXPORT
#endif

//=============================================================================
/*!
 *
 */
//=============================================================================

extern "C"
{
XAOEXPORT_EXPORT
  int Export(const TopoDS_Shape& theShape, const TCollection_AsciiString& theFileName)
  {
    MESSAGE("Export XAO into file " << theFileName.ToCString());
    SalomeApp_Application* app = dynamic_cast<SalomeApp_Application*>(SUIT_Session::session()->activeApplication());
    if (!app) 
      return 0;
    
    //SalomeApp_Study* anActiveStudy = ( SalomeApp_Study* ) app->activeStudy();
    SalomeApp_Study* anActiveStudy = dynamic_cast<SalomeApp_Study*>( app->activeStudy() );
    if (!anActiveStudy) 
      return 0;

    LightApp_SelectionMgr* aSelMgr = app->selectionMgr();
    if (!aSelMgr) 
      return 0;
  
    // get selection
    SALOME_ListIO aList;
    aSelMgr->selectedObjects(aList, "ObjectBrowser", false);
    SALOME_ListIteratorOfListIO It (aList);
  
    SUIT_OverrideCursor();

    QString nameShape;
    GEOM::ListOfLong_var aSubShapesId;
    for (; It.More(); It.Next()) 
    {
      Handle(SALOME_InteractiveObject) anIObject = It.Value();
      if (anIObject->hasEntry()) 
      {
	_PTR(SObject) SO (anActiveStudy->studyDS()->FindObjectID(anIObject->getEntry()));
	  if (!SO)
	    return 0;
	  GEOM::GEOM_Object_var anObj = GEOM::GEOM_Object::_narrow(GeometryGUI::ClientSObjectToObject(SO));
	  if (CORBA::is_nil(anObj)) 
	    return 0;
	  //nameShape = GEOMBase::GetName( anObj.get() );
	  nameShape = anObj->GetName();
          
	  GEOM::GEOM_IShapesOperations_var aShOp = GeometryGUI::GetGeomGen()->GetIShapesOperations(anObj->GetStudyID());
          aSubShapesId = aShOp->SubShapeAllIDs(anObj, TopAbs_VERTEX, false);
      }
    }


    ostringstream  streamShape;
    BRepTools::Write(theShape, streamShape);

    xmlDocPtr master_doc = 0;
    xmlNodePtr geometry_node = 0;
  
    //Creating the Xml document
    master_doc = xmlNewDoc(BAD_CAST "1.0");
    geometry_node = xmlNewNode(0, BAD_CAST "geometry");
    xmlDocSetRootElement(master_doc,geometry_node);
  
    // Creating child nodes
    xmlNodePtr version_geom_node = xmlNewChild(geometry_node, 0, BAD_CAST "version",BAD_CAST "1.0");
    xmlNodePtr name_geom_node = xmlNewChild(geometry_node, 0, BAD_CAST "name",BAD_CAST nameShape.toStdString().c_str());
    xmlNodePtr format_geom_node = xmlNewChild(geometry_node, 0, BAD_CAST "format",BAD_CAST "BREP");
    xmlNodePtr shape_geom_node = xmlNewChild(geometry_node, 0, BAD_CAST "shape",BAD_CAST streamShape.str().c_str());
    xmlNodePtr topo_geom_node = xmlNewChild(geometry_node, 0, BAD_CAST "shape",BAD_CAST streamShape.str().c_str());
    
    // Topomogy description
    xmlNodePtr vertices_topo_geom_node = xmlNewChild(topo_geom_node,0, BAD_CAST "vertices",0);
    xmlNodePtr count_vertices_topo_geom_node = xmlNewChild(vertices_topo_geom_node,0, BAD_CAST "count",BAD_CAST "12");
    
    //std::string myfile(theFileName);
    xmlSaveFormatFileEnc(theFileName.ToCString(), master_doc, "UTF-8", 1);
    
    xmlFreeDoc(master_doc);
    
    xmlCleanupParser();
    return 1;
  }
}
