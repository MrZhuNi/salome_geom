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

Xao::Xao(const std::string& author, const std::string& version)
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

const int Xao::countGroups()
{
    return m_groups.size();
}

Group* Xao::getGroup(const int& index)
{
    int i = 0;
    for (std::list<Group*>::iterator it = m_groups.begin(); it != m_groups.end(); ++it, ++i)
    {
        if (i == index)
            return (*it);
    }

    return NULL;
}

Group* Xao::addGroup(const XAO::Dimension& dim)
{
    return addGroup("", dim);
}

Group* Xao::addGroup(const std::string& name, const XAO::Dimension& dim)
{
    checkGeometry();
    Group* group = new Group(name, dim, m_geometry->countElements(dim));
    m_groups.push_back(group);
    return group;
}

void Xao::removeGroup(Group* group)
{
    m_groups.remove(group);
}

const int Xao::countFields()
{
    return m_fields.size();
}

Field* Xao::getField(const int& index)
{
    int i = 0;
    for (std::list<Field*>::iterator it = m_fields.begin(); it != m_fields.end(); ++it, ++i)
    {
        if (i == index)
            return (*it);
    }

    return NULL;
}

Field* Xao::addField(const XAO::Type& type, const XAO::Dimension& dim, const int& nbComponents)
{
    return addField(type, "", dim, nbComponents);
}

Field* Xao::addField(const XAO::Type& type, const std::string& name, const XAO::Dimension& dim, const int& nbComponents)
{
    checkGeometry();
    int nbElts = m_geometry->countElements(dim);
    Field* field = Field::createField(type, name, dim, nbElts, nbComponents);
    m_fields.push_back(field);
    return field;
}

void Xao::removeField(Field* field)
{
    m_fields.remove(field);
}

const bool Xao::exportXAO(const std::string& fileName)
{
//    xmlDocPtr doc = exportXMLDoc();
//    xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 2);
//    xmlFreeDoc(doc);
//
//    return true;
    return XaoExporter::saveToFile(this, fileName);
}

const std::string Xao::getXML()
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

const bool Xao::importXAO(const std::string& fileName)
{
    return XaoExporter::readFromFile(fileName, this);
}

const bool Xao::setXML(const std::string& xml)
{
    return XaoExporter::setXML(xml, this);
}

void Xao::checkGeometry()
{
    if (m_geometry == NULL)
        throw SALOME_Exception("Geometry is null"); // TODO
}
