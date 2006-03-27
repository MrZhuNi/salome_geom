// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
// File:	IGESExport.cxx
// Created:	Wed May 19 14:49:45 2004
// Author:	Pavel TELKOV
//		<ptv@mutex.nnov.opencascade.com>

#include "utilities.h"

#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <Interface_Static.hxx>

#include <TCollection_AsciiString.hxx>
#include <TopoDS_Shape.hxx>

#ifdef WNT
#include <SALOME_WNT.hxx>
#else
#define SALOME_WNT_EXPORT
#endif

//=============================================================================
/*!
 *
 */
//=============================================================================

extern "C"
{
SALOME_WNT_EXPORT
  int Export(const TopoDS_Shape& theShape,
             const TCollection_AsciiString& theFileName,
             const TCollection_AsciiString& theFormatName)
  {
    MESSAGE("Export IGES into file " << theFileName.ToCString());
    try
    {
      // define, whether to write only faces (5.1 IGES format)
      // or shells and solids also (5.3 IGES format)
      int aBrepMode = 0;
      if (theFormatName.IsEqual("IGES_5_3"))
        aBrepMode = 1;

      // initialize writer
      IGESControl_Controller::Init();
      //IGESControl_Writer ICW (Interface_Static::CVal("write.iges.unit"),
      //			Interface_Static::IVal("write.iges.brep.mode"));
      IGESControl_Writer ICW (Interface_Static::CVal("write.iges.unit"), aBrepMode);

      // perform shape writing
      ICW.AddShape( theShape );
      ICW.ComputeModel();
      if ( ICW.Write( theFileName.ToCString() ) )
        return 1;
    }
    catch(Standard_Failure)
    {
      //THROW_SALOME_CORBA_EXCEPTION("Exception catched in IGESExport", SALOME::BAD_PARAM);
    }
    return 0;
  }
}
