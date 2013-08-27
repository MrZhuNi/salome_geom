// Copyright (C) 2013  CEA/DEN, EDF R&D
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
// Author : Nathalie Gore (OpenCascade), Frederic Pons (OpenCascade)

#include <libxml/parser.h>

#include "XaoUtils.hxx"
#include "Xao.hxx"
#include "Geometry.hxx"
#include "Group.hxx"
#include "Field.hxx"
#include "XaoExporter.hxx"
#include <Utils_SALOME_Exception.hxx>

using namespace XAO;

const xmlChar* C_XAO_VERSION = (xmlChar*)"1.0";

Xao::Xao()
{
    m_author = "";
    m_version = (char*)C_XAO_VERSION;
    m_geometry = NULL;
}

Xao::Xao(const char* author, const char* version)
{
    m_author = author;
    m_version = version;
    m_geometry = NULL;
}

Xao::~Xao()
{
    if (m_geometry != NULL)
    {
        delete m_geometry;
        m_geometry = NULL;
    }

    for (std::list<Group*>::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        delete (*it);
    }

    for (std::list<Field*>::iterator it = m_fields.begin(); it != m_fields.end(); ++it)
    {
        delete (*it);
    }
}

int Xao::countGroups()
{
    return m_groups.size();
}

Group* Xao::getGroup(const int index)
{
    int i = 0;
    for (std::list<Group*>::iterator it = m_groups.begin(); it != m_groups.end(); ++it, ++i)
    {
        if (i == index)
            return (*it);
    }

    return NULL;
}

void Xao::addGroup(Group* group)
{
    m_groups.push_back(group);
}

void Xao::removeGroup(Group* group)
{
    m_groups.remove(group);
}

int Xao::countFields()
{
    return m_fields.size();
}

Field* Xao::getField(const int index)
{
    int i = 0;
    for (std::list<Field*>::iterator it = m_fields.begin(); it != m_fields.end(); ++it, ++i)
    {
        if (i == index)
            return (*it);
    }

    return NULL;
}

void Xao::addField(Field* field)
{
    m_fields.push_back(field);
}

void Xao::removeField(Field* field)
{
    m_fields.remove(field);
}

bool Xao::exportXAO(const char* fileName)
{
//    xmlDocPtr doc = exportXMLDoc();
//    xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 2);
//    xmlFreeDoc(doc);
//
//    return true;
    return XaoExporter::saveToFile(this, fileName);
}

const char* Xao::getXML()
{
//    xmlDocPtr doc = exportXMLDoc();
//
//    xmlChar *xmlbuff;
//    int buffersize;
//    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
//    xmlFreeDoc(doc);
//    xmlCleanupGlobals();
//
//    return (char*)xmlbuff;
    return XaoExporter::saveToXml(this);
}

bool Xao::importXAO(const char* fileName)
{
    return XaoExporter::readFromFile(fileName, this);
}

bool Xao::setXML(const char* xml)
{
    return XaoExporter::setXML(xml, this);
}
