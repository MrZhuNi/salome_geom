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

// File:        XAOImport.cxx
// Created:     Thu Dec 04 08:00:00 2012
// Author:      Nathalie Gore
//              <nathalie.gore@opencascade.com>
//
#include "utilities.h"

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>

#include <TCollection_AsciiString.hxx>
#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
using namespace std;


#ifdef WNT
 #if defined XAOIMPORT_EXPORTS || defined XAOImport_EXPORTS
  #if defined WIN32
   #define XAOIMPORT_EXPORT __declspec( dllexport )
  #else
   #define XAOIMPORT_EXPORT
  #endif
 #else
  #if defined WIN32
   #define XAOIMPORT_EXPORT __declspec( dllimport )
  #else
   #define XAOIMPORT_EXPORT
  #endif
 #endif
#else
 #define XAOIMPORT_EXPORT
#endif

//=============================================================================
/*!
 *
 */
//=============================================================================

extern "C"
{
XAOIMPORT_EXPORT
  TopoDS_Shape Import (const TCollection_AsciiString& theFileName,
                       const TCollection_AsciiString& /*theFormatName*/,
                       TCollection_AsciiString&       theError,
                       const TDF_Label&)
  {
    MESSAGE("Import XAO from file " << theFileName);
    
    // Read file
    ifstream is;
    is.open (theFileName.ToCString(), ios::binary );

    // ***** get length of file:
    is.seekg (0, std::ios::end);
    long length = is.tellg();
    is.seekg (0, std::ios::beg);

    // ***** allocate memory:
    char *buffer = new char [length];

    // ***** read data as a block:
    is.read (buffer,length);

    // ***** create string stream of memory contents
    // NOTE: this ends up copying the buffer!!!
    istringstream streamBrep( buffer );

    // ***** delete temporary buffer
    delete [] buffer;

    // ***** close filestream
    is.close();
    
    // Import file
    TopoDS_Shape aShape;
    BRep_Builder B;
    BRepTools::Read(aShape, streamBrep, B);
    if (aShape.IsNull()) {
      theError = "XAO Import failed";
    }
    return aShape;
  }
}
