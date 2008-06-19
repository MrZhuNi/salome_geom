//  GEOM GEOM : implementaion of GEOM_Gen.idl
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : geom.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#include <Standard_Stream.hxx>

#include <CORBA.h>
#include <GEOM_Gen_i.hh>
#include <SALOME_NamingService.hxx>

#include "utilities.h"

//==================================================================================
// function : main()                      MAIN 
// purpose  :
//==================================================================================
int main(int argc, char** argv)
{
  try {
    // Create and initialise the ORB.
    //    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "omniORB4");
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "omniORB3");
    
    // Obtain a reference to the root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

    // We allocate the objects on the heap.  Since these are reference
    // counted objects, they will be deleted by the POA when they are no
    // longer needed.    
#ifdef WNT
    GEOM_Gen_i * myGEOM_Gen_i = new GEOM_Gen_i((CORBA::ORB_ptr)orb, poa, 0, 0, 0 );
#else
    GEOM_Gen_i * myGEOM_Gen_i = new GEOM_Gen_i(orb);
#endif

    // Activate the objects.  This tells the POA that the objects are
    // ready to accept requests.
    PortableServer::ObjectId_var myGEOM_Gen_iid = poa->activate_object(myGEOM_Gen_i);
    myGEOM_Gen_i->register_name("/myGEOM_Gen");
 
    // Obtain a POAManager, and tell the POA to start accepting
    // requests on its objects.
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    orb->run();
    orb->destroy();
  }
  catch(CORBA::SystemException&) {
    INFOS ( "Caught CORBA::SystemException." );
  }
  catch(CORBA::Exception&) {
    INFOS ( "Caught CORBA::Exception." );
  }
  catch(omniORB::fatalException& fe) {
    INFOS ( "Caught omniORB::fatalException:" );
    INFOS ( "  file: " << fe.file() );
    INFOS ( "  line: " << fe.line() );
    INFOS ( "  msg: " << fe.errmsg() );
  }
  catch(...) {
    INFOS ( "Caught unknown exception." );
  }
  return 0;
}
