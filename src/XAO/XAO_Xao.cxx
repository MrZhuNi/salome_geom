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

#include <iostream>
#include "XAO_XaoUtils.hxx"
#include "XAO_Xao.hxx"
#include "XAO_Geometry.hxx"
#include "XAO_Group.hxx"
#include "XAO_Field.hxx"
#include "XAO_IntegerField.hxx"
#include "XAO_XaoExporter.hxx"

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

const int Xao::countGroups() const
{
    return m_groups.size();
}

Group* Xao::getGroup(const int& index)
throw (XAO_Exception)
{
    checkGroupIndex(index);

    int i = 0;
    for (std::list<Group*>::iterator it = m_groups.begin(); it != m_groups.end(); ++it, ++i)
    {
        if (i == index)
            return (*it);
    }

    return NULL;
}

Group* Xao::addGroup(const XAO::Dimension& dim, const std::string& name)
throw (XAO_Exception)
{
    checkGeometry();
    checkGroupDimension(dim);

    Group* group = new Group(dim, m_geometry->countElements(dim), name);
    m_groups.push_back(group);
    return group;
}

bool Xao::removeGroup(Group* group)
{
    int nb = countGroups();
    m_groups.remove(group);

    bool res = (nb-1 == countGroups());
    if (res)
    {
        delete group;
        group = NULL;
    }

    return res;
}

const int Xao::countFields() const
{
    return m_fields.size();
}

Field* Xao::getField(const int& index)
throw (XAO_Exception)
{
    checkFieldIndex(index);

    int i = 0;
    for (std::list<Field*>::iterator it = m_fields.begin(); it != m_fields.end(); ++it, ++i)
    {
        if (i == index)
            return (*it);
    }

    return NULL;
}

Field* Xao::addField(const XAO::Type& type, const XAO::Dimension& dim, const int& nbComponents, const std::string& name)
throw (XAO_Exception)
{
    checkGeometry();
    int nbElts = m_geometry->countElements(dim);
    Field* field = Field::createField(type, dim, nbElts, nbComponents, name);
    m_fields.push_back(field);
    return field;
}

bool Xao::removeField(Field* field)
{
    int nb = countFields();
    m_fields.remove(field);

    bool res = (nb-1 == countFields());
    if (res)
    {
        delete field;
        field = NULL;
    }

    return res;
}

const bool Xao::exportXAO(const std::string& fileName)
{
    return XaoExporter::saveToFile(this, fileName);
}

const std::string Xao::getXML()
{
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

void Xao::checkGeometry() const
throw(XAO_Exception)
{
    if (m_geometry == NULL)
        throw XAO_Exception("Geometry is null");
}

void Xao::checkGroupIndex(const int& index) const
throw(XAO_Exception)
{
    if (index >= 0 && index < countGroups())
        return;

    throw XAO_Exception(MsgBuilder() << "Group index is out of range [0, "
                                     << countGroups()-1 << "]: " << index);
}

void Xao::checkFieldIndex(const int& index) const
throw(XAO_Exception)
{
    if (index >= 0 && index < countFields())
        return;

    throw XAO_Exception(MsgBuilder() << "Field index is out of range [0, "
                                     << countFields()-1 << "]: " << index);
}

void Xao::checkGroupDimension(const XAO::Dimension& dim) const
throw(XAO_Exception)
{
    if (dim == XAO::WHOLE)
        throw XAO_Exception(MsgBuilder() << "Invalid dimension for group: " << dim);
}
