//  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File:        IGESImport.cxx
//  Created:     Wed May 19 14:36:35 2004
//  Author:      Pavel TELKOV
//               <ptv@mutex.nnov.opencascade.com>

#include "utilities.h"

#include <Basics_Utils.hxx>

#include <IFSelect_ReturnStatus.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESData_IGESModel.hxx>

#include <TCollection_HAsciiString.hxx>
#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>

#include <TopoDS_Vertex.hxx>
#include <BRep_Builder.hxx>
#include <gp_Pnt.hxx>
#include <Interface_Static.hxx>

#ifdef WNT
 #if defined IGESIMPORT_EXPORTS || defined IGESImport_EXPORTS
  #if defined WIN32
   #define IGESIMPORT_EXPORT __declspec( dllexport )
  #else
   #define IGESIMPORT_EXPORT
  #endif
 #else
  #if defined WIN32
   #define IGESIMPORT_EXPORT __declspec( dllimport )
  #else
   #define IGESIMPORT_EXPORT
  #endif
 #endif
#else
 #define IGESIMPORT_EXPORT
#endif

//=============================================================================
/*!
 *
 */
//=============================================================================

extern "C"
{
IGESIMPORT_EXPORT
  TopoDS_Shape Import (const TCollection_AsciiString& theFileName,
                       const TCollection_AsciiString& theFormatName,
                       TCollection_AsciiString&       theError,
                       const TDF_Label&)
  {
    // Set "C" numeric locale to save numbers correctly
    Kernel_Utils::Localizer loc;

    IGESControl_Reader aReader;
    TopoDS_Shape aResShape;
    Interface_Static::SetCVal("xstep.cascade.unit","M");
    try {
      IFSelect_ReturnStatus status = aReader.ReadFile(theFileName.ToCString());

      if (status == IFSelect_RetDone) {

        if( theFormatName == "IGES_UNIT" ) {
          Handle(IGESData_IGESModel) aModel = 
            Handle(IGESData_IGESModel)::DownCast(aReader.Model());
          gp_Pnt P(1.0,0.0,0.0);
          if(!aModel.IsNull()) {
            Handle(TCollection_HAsciiString) aUnitName = 
              aModel->GlobalSection().UnitName();
            //cout<<"aUnitName = "<<aUnitName->ToCString()<<endl;
            //cout<<"aUnitFlag = "<<aModel->GlobalSection().UnitFlag()<<endl;
            if( aUnitName->String()=="MM" ) {
              P = gp_Pnt(0.001,0.0,0.0);
            }
            else if( aUnitName->String()=="CM" ) {
              P = gp_Pnt(0.01,0.0,0.0);
            }
          }
          BRep_Builder B;
          TopoDS_Vertex V;
          B.MakeVertex(V,P,1.e-7);
          aResShape = V;
          return aResShape;
        }
        if( theFormatName == "IGES_SCALE" ) {
          //cout<<"need re-scale a model"<<endl;
          // set UnitFlag to 'meter'
          Handle(IGESData_IGESModel) aModel = 
            Handle(IGESData_IGESModel)::DownCast(aReader.Model());
          if(!aModel.IsNull()) {
            IGESData_GlobalSection aGS = aModel->GlobalSection();
            aGS.SetUnitFlag(6);
            aModel->SetGlobalSection(aGS);
          }
        }

        MESSAGE("ImportIGES : all Geometry Transfer");
        //OCC 5.1.2 porting
        //     aReader.Clear();
        //     aReader.TransferRoots(false);
        aReader.ClearShapes();
        aReader.TransferRoots();

        MESSAGE("ImportIGES : count of shapes produced = " << aReader.NbShapes());
        aResShape = aReader.OneShape();

      } else {
//        switch (status) {
//        case IFSelect_RetVoid:
//          theError = "Nothing created or No data to process";
//          break;
//        case IFSelect_RetError:
//          theError = "Error in command or input data";
//          break;
//        case IFSelect_RetFail:
//          theError = "Execution was run, but has failed";
//          break;
//        case IFSelect_RetStop:
//          theError = "Execution has been stopped. Quite possible, an exception was raised";
//          break;
//        default:
//          break;
//        }
        theError = "Wrong format of the imported file. Can't import file.";
        aResShape.Nullify();
      }
    }
    catch(Standard_Failure) {
      Handle(Standard_Failure) aFail = Standard_Failure::Caught();
      theError = aFail->GetMessageString();
      aResShape.Nullify();
    }
    return aResShape;
  }
}
