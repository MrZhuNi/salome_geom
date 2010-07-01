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

//  GEOM GEOMClient : tool to transfer BREP files from GEOM server to GEOM client
//  File   : GEOM_Client.cxx
//  Author : Yves FRICAUD/Lucien PIGNOLONI
//  Module : GEOM
//  $Header$
//
#include <Standard_Stream.hxx>

#include <Standard_Stream.hxx>

#include <sstream>

#include "GEOM_Client.hxx"
#include <SALOMEconfig.h>
#include "Basics_Utils.hxx"
#include "utilities.h"

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TCollection_AsciiString.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TopAbs.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#ifdef WNT
#include <process.h>
#else
#include <unistd.h>
#endif

#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(GEOM_Gen)

#define HST_CLIENT_LEN 256

#include <vector>

#include <sys/time.h>
static long tcount=0;
static long cumul;
#define START_TIMING long tt0; timeval tv; gettimeofday(&tv,0);tt0=tv.tv_usec+tv.tv_sec*1000000;
#define END_TIMING(NUMBER) \
  tcount=tcount+1;gettimeofday(&tv,0);cumul=cumul+tv.tv_usec+tv.tv_sec*1000000 -tt0; \
  if(tcount==NUMBER){ std::cerr << __FILE__ << __LINE__ << " temps CPU(mus): " << cumul << std::endl; tcount=0;cumul=0; }

GEOM_Client GEOM_Client::ShapeReader;

//=======================================================================
// function : Load()
// purpose  :
//=======================================================================
TopoDS_Shape GEOM_Client::Load( GEOM::GEOM_Gen_ptr geom, GEOM::GEOM_Object_ptr aShape )
{
  std::string hst_client = Kernel_Utils::GetHostname();

  Engines::Container_var ctn_server = geom->GetContainerRef();
  long                   pid_server = ctn_server->getPID();

  CORBA::String_var hostname = ctn_server->getHostName();
  if ( pid_client == pid_server && !strcmp(hst_client.c_str(), hostname.in()) ) {
    TopoDS_Shape* S = (TopoDS_Shape*)(aShape->getShape());
    return(*S);
  } else {
    /* get sequence of bytes of resulting brep shape from GEOM server */
    TopoDS_Shape S;
    SALOMEDS::TMPFile_var SeqFile = aShape->GetShapeStream();
    int sizebuf = SeqFile->length();
    char* buf;
    buf = (char*) &SeqFile[0];
    std::istringstream streamBrep(buf);
    BRep_Builder aBuilder;
    BRepTools::Read(S, streamBrep, aBuilder);
    return(S);
  }
}

//=======================================================================
// function : Create()
// purpose  : Create in client not in a container
//=======================================================================
GEOM_Client::GEOM_Client()
{
  pid_client =
#ifdef WNT
    (long)_getpid();
#else
    (long)getpid();
#endif
}

//=======================================================================
// function : Create()
// purpose  :
//=======================================================================
GEOM_Client::GEOM_Client(Engines::Container_ptr client)
{
  pid_client = client->getPID();
}

//=======================================================================
// function : Find()
// purpose  :
//=======================================================================
Standard_Integer GEOM_Client::Find( const TCollection_AsciiString& IOR, TopoDS_Shape& S )
{
  //CCAR
#if 1
  if(_myIndexes.count(IOR)==0)
    {
      return 0;
    }
  else
    {
      Standard_Integer i =_myIndexes[IOR];
      S = myShapes.Value(i);
      return i;
    }
#else
  for ( Standard_Integer i = 1; i<= myIORs.Length(); i++ ) {
    if (myIORs.Value(i).IsEqual(IOR)) {
      S = myShapes.Value(i);
      return i;
    }
  }
  return 0;
#endif
}

//=======================================================================
// function : Find()
// purpose  :
//=======================================================================
Standard_Integer GEOM_Client::Find( const TopoDS_Shape& S, TCollection_AsciiString& IOR )
{
  for ( Standard_Integer i = 1; i<= myShapes.Length(); i++ ) {
    if (myShapes.Value(i) == S) {
      IOR = myIORs.Value(i);
      return i;
    }
  }
  return 0;
}

//=======================================================================
// function : Bind()
// purpose  :
//=======================================================================
void GEOM_Client::Bind( const TCollection_AsciiString& IOR, const TopoDS_Shape& S )
{
  myIORs.Append(IOR);
  myShapes.Append(S);
  _myIndexes[IOR]=myIORs.Length();
}

//=======================================================================
// function : RemoveShapeFromBuffer()
// purpose  : Remove shape from Client Buffer
//=======================================================================
void GEOM_Client::RemoveShapeFromBuffer( const TCollection_AsciiString& IOR)
{
  if( myIORs.IsEmpty() )
    return;

  TopoDS_Shape S;
  Standard_Integer anIndex = Find( IOR, S );
  if( anIndex != 0 ) {
    myIORs.Remove(anIndex);
    myShapes.Remove(anIndex);
    _myIndexes.erase(IOR);
    _mySubShapes.erase(IOR);
  }
  return;
}

//=======================================================================
// function : ClearClientBuffer()
// purpose  : purge buffer
//=======================================================================
void GEOM_Client::ClearClientBuffer()
{
  if( myIORs.IsEmpty() )
    return;
  myIORs.Clear();
  myShapes.Clear();
  _myIndexes.clear();
  _mySubShapes.clear();
  return;
}

//=======================================================================
// function : BufferLength()
// purpose  :
//=======================================================================
unsigned int GEOM_Client::BufferLength()
{
  return myIORs.Length();
}

//=======================================================================
// function : GetShape()
// purpose  :
//=======================================================================
TopoDS_Shape GEOM_Client::GetShape( GEOM::GEOM_Gen_ptr geom, GEOM::GEOM_Object_ptr aShape )
{
  TopoDS_Shape S;
  CORBA::String_var anIOR = geom->GetStringFromIOR(aShape);
  TCollection_AsciiString IOR = (char*)anIOR.in();
  Standard_Integer anIndex = Find(IOR, S);

  if (anIndex != 0)
    {
      return S;
    }

  /******* in case of a MAIN GEOM::SHAPE ********/
  if (aShape->IsMainShape()) {
    S = Load(geom, aShape);
    Bind(IOR, S);
    return S;
  }

  /******* in case of SUB GEOM::SHAPE ***********/
  // Load and Explore the Main Shape
  TopoDS_Shape aMainShape = GetShape (geom, aShape->GetMainShape());
  GEOM::ListOfLong_var list = aShape->GetSubShapeIndices();

  START_TIMING;

  TopTools_IndexedMapOfShape anIndices;
  anIOR = geom->GetStringFromIOR(aShape->GetMainShape());
  IOR = (char*)anIOR.in();

  //find subshapes only one time
  if(_mySubShapes.count(IOR)==0)
    {
      std::cerr << "find sub shapes " << std::endl;
      TopExp::MapShapes(aMainShape, anIndices);
      Standard_Integer ii = 1, nbSubSh = anIndices.Extent();
      for (; ii <= nbSubSh; ii++) 
        {
          _mySubShapes[IOR].push_back(anIndices.FindKey(ii));
        }
    }

  /* Case of only one subshape */
  if (list->length() == 1 && list[0] > 0) {
    //S = anIndices.FindKey(list[0]);
    S = _mySubShapes[IOR][list[0]-1];
  }
  else {
    BRep_Builder B;
    TopoDS_Compound aCompound;
    B.MakeCompound(aCompound);
    for (int i = 0; i < list->length(); i++) {
      if (0 < list[i] && list[i] <= anIndices.Extent()) {
        TopoDS_Shape aSubShape = _mySubShapes[IOR][list[i]-1];
        //TopoDS_Shape aSubShape = anIndices.FindKey(list[i]);
        B.Add(aCompound, aSubShape);
      }
    }

    S = aCompound;
  }
  Bind(IOR, S);
  END_TIMING(100);
  return S;
}
