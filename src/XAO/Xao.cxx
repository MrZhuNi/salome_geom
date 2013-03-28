// Copyright (C) 2007-2012  CEA/DEN, EDF R&D
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
// Author : Nathalie Gore (OpenCascade)

#include "Xao.hxx"
#include "Geometry.hxx"
#include "Group.hxx"
#include "Field.hxx"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>

using namespace XAO;

Xao::Xao()
{
  _myAuthor = "Nathalie Gore";
  _myVersion = "1.0";
  _myGeometry = NULL;
  _myNbGroups = 0;
}

Xao::Xao(const char *author, const char *version)
{
  _myAuthor = author;
  _myVersion = version;
  _myGeometry = NULL;
  _myNbGroups = 0;
}


Xao::~Xao()
{
  if (_myGeometry != NULL)
  {
    delete _myGeometry;
    _myGeometry = NULL;
  }
}

bool Xao::Export(const char* fileName)
{
  if (_myGeometry == NULL)
    return false;
    
  xmlDocPtr master_doc = 0;
  xmlNodePtr xao_node = 0;
  
  //Creating the Xml document
  master_doc = xmlNewDoc(BAD_CAST "1.0");
  xao_node = xmlNewNode(0, BAD_CAST "XAO");
  xmlDocSetRootElement(master_doc,xao_node);
  
  xmlNodePtr version_node = xmlNewChild(xao_node,0, BAD_CAST "version", BAD_CAST _myVersion.c_str());
  xmlNodePtr author_node = xmlNewChild(xao_node,0, BAD_CAST "author", BAD_CAST _myAuthor.c_str());
  
  // Geometric part
  xmlNodePtr geom_node = xmlNewChild(xao_node,0, BAD_CAST "geometry",0);
  xmlNodePtr name_geom_node = xmlNewChild(geom_node,0, BAD_CAST "name",BAD_CAST _myGeometry->getName());
  xmlNodePtr format_geom_node = xmlNewChild(geom_node,0, BAD_CAST "name",BAD_CAST _myGeometry->getFormat());
  xmlNodePtr shape_geom_node = xmlNewChild(geom_node,0, BAD_CAST "shape",BAD_CAST _myGeometry->getBREP());
  
  /*xmlNodePtr topo_geom_node = xmlNewChild(geom_node,0, BAD_CAST "topology",0);
  ostringstream  stream;
  stream << _myGeometry->countVertices();
  xmlNodePtr count_topo_geom_node = xmlNewChild(count_topo_geom_node,0, BAD_CAST "topology", BAD_CAST stream.str().c_str());*/
  

}

