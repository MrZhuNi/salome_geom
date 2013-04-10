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
#include <Utils_SALOME_Exception.hxx>

using namespace XAO;


const xmlChar* C_XAO_VERSION = (xmlChar*)"1.0";

const xmlChar* C_TAG_XAO = (xmlChar*)"XAO";
const xmlChar* C_ATTR_XAO_AUTHOR = (xmlChar*)"author";
const xmlChar* C_ATTR_XAO_VERSION = (xmlChar*)"version";

const xmlChar* C_TAG_GEOMETRY = (xmlChar*)"geometry";
const xmlChar* C_ATTR_GEOMETRY_NAME = (xmlChar*)"name";

const xmlChar* C_TAG_SHAPE = (xmlChar*)"shape";
const xmlChar* C_ATTR_SHAPE_FORMAT = (xmlChar*)"format";

const xmlChar* C_TAG_TOPOLOGY = (xmlChar*)"topology";
const xmlChar* C_TAG_VERTICES = (xmlChar*)"vertices";
const xmlChar* C_TAG_VERTEX = (xmlChar*)"vertex";
const xmlChar* C_TAG_EDGES = (xmlChar*)"edges";
const xmlChar* C_TAG_EDGE = (xmlChar*)"edge";
const xmlChar* C_TAG_FACES = (xmlChar*)"faces";
const xmlChar* C_TAG_FACE = (xmlChar*)"face";
const xmlChar* C_TAG_SOLIDS = (xmlChar*)"solids";
const xmlChar* C_TAG_SOLID = (xmlChar*)"solid";
const xmlChar* C_ATTR_COUNT = (xmlChar*)"count";
const xmlChar* C_ATTR_ELT_INDEX = (xmlChar*)"index";
const xmlChar* C_ATTR_ELT_NAME = (xmlChar*)"name";
const xmlChar* C_ATTR_ELT_REFERENCE = (xmlChar*)"reference";

const xmlChar* C_TAG_GROUPS = (xmlChar*)"groups";
const xmlChar* C_TAG_GROUP = (xmlChar*)"group";
const xmlChar* C_ATTR_GROUP_NAME = (xmlChar*)"name";
const xmlChar* C_ATTR_GROUP_DIM = (xmlChar*)"dimension";

const xmlChar* C_TAG_ELEMENT = (xmlChar*)"element";
const xmlChar* C_ATTR_ELEMENT_INDEX = (xmlChar*)"index";

const xmlChar* C_TAG_FIELDS = (xmlChar*)"fields";
const xmlChar* C_TAG_FIELD = (xmlChar*)"field";
const xmlChar* C_TAG_COMPONENTS = (xmlChar*)"components";
const xmlChar* C_TAG_COMPONENT = (xmlChar*)"component";
const xmlChar* C_TAG_STEPS = (xmlChar*)"steps";
const xmlChar* C_TAG_STEP = (xmlChar*)"step";

Xao::Xao()
{
    m_author = "";
    m_version = (char*)C_XAO_VERSION;
    m_geometry = NULL;
    m_nbGroups = 0;
}

Xao::Xao(const char* author, const char* version)
{
    m_author = author;
    m_version = version;
    m_geometry = NULL;
    m_nbGroups = 0;
}

Xao::~Xao()
{
    if (m_geometry != NULL)
    {
        delete m_geometry;
        m_geometry = NULL;
    }

//    for (std::list<Group*>::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
//    {
//        delete (*it);
//    }
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


bool Xao::exportToFile(const char* fileName)
{
    xmlDocPtr doc = exportXMLDoc();
    xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 2);
    xmlFreeDoc(doc);

    return true;
}

const char* Xao::getXML()
{
    xmlDocPtr doc = exportXMLDoc();

    xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    xmlFreeDoc(doc);
    xmlCleanupGlobals();

    return (char*)xmlbuff;
}

xmlDocPtr Xao::exportXMLDoc()
{
    // Creating the Xml document
    xmlDocPtr masterDocument = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr xao = xmlNewNode(0, C_TAG_XAO);
    xmlDocSetRootElement(masterDocument, xao);

    xmlNewProp(xao, C_ATTR_XAO_VERSION, BAD_CAST getVersion());
    xmlNewProp(xao, C_ATTR_XAO_AUTHOR, BAD_CAST getAuthor());

    if (m_geometry != NULL)
    {
        exportGeometry(masterDocument, xao);
    }

    exportGroups(xao);

    return masterDocument;
}

void Xao::exportGeometry(xmlDocPtr doc, xmlNodePtr xao)
{
    // Geometric part
    xmlNodePtr geometry = xmlNewChild(xao, 0, C_TAG_GEOMETRY, 0);
    xmlNewProp(geometry, C_ATTR_GEOMETRY_NAME, BAD_CAST m_geometry->getName());

    xmlNodePtr shape = xmlNewChild(geometry, 0, C_TAG_SHAPE, 0);
    xmlNewProp(shape, C_ATTR_SHAPE_FORMAT, BAD_CAST m_geometry->getFormat());
    const char* brep = m_geometry->getBREP();
    xmlNodePtr cdata = xmlNewCDataBlock(doc, BAD_CAST brep, strlen(brep));
    xmlAddChild(shape, cdata);

    xmlNodePtr topology = xmlNewChild(geometry, 0, C_TAG_TOPOLOGY, 0);

    // vertices
    xmlNodePtr vertices = xmlNewChild(topology, 0, C_TAG_VERTICES, 0);
    xmlNewProp(vertices, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(m_geometry->countVertices()));
    for (int i = 0; i < m_geometry->countVertices(); ++i)
    {
        xmlNodePtr vertex = xmlNewChild(vertices, 0, C_TAG_VERTEX, 0);
        xmlNewProp(vertex, C_ATTR_ELT_INDEX, BAD_CAST XaoUtils::intToString(i));
        xmlNewProp(vertex, C_ATTR_ELT_NAME, BAD_CAST m_geometry->getVertexName(i));
        xmlNewProp(vertex, C_ATTR_ELT_REFERENCE, BAD_CAST m_geometry->getVertexReference(i));
    }

    // edges
    xmlNodePtr edges = xmlNewChild(topology, 0, C_TAG_EDGES, 0);
    xmlNewProp(edges, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(m_geometry->countEdges()));
    for (int i = 0; i < m_geometry->countEdges(); ++i)
    {
        xmlNodePtr edge = xmlNewChild(edges, 0, C_TAG_EDGE, 0);
        xmlNewProp(edge, C_ATTR_ELT_INDEX, BAD_CAST XaoUtils::intToString(i));
        xmlNewProp(edge, C_ATTR_ELT_NAME, BAD_CAST m_geometry->getEdgeName(i));
        xmlNewProp(edge, C_ATTR_ELT_REFERENCE, BAD_CAST m_geometry->getEdgeReference(i));
    }

    // faces
    xmlNodePtr faces = xmlNewChild(topology, 0, C_TAG_FACES, 0);
    xmlNewProp(faces, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(m_geometry->countFaces()));
    for (int i = 0; i < m_geometry->countFaces(); ++i)
    {
        xmlNodePtr face = xmlNewChild(faces, 0, C_TAG_FACE, 0);
        xmlNewProp(face, C_ATTR_ELT_INDEX, BAD_CAST XaoUtils::intToString(i));
        xmlNewProp(face, C_ATTR_ELT_NAME, BAD_CAST m_geometry->getFaceName(i));
        xmlNewProp(face, C_ATTR_ELT_REFERENCE, BAD_CAST m_geometry->getFaceReference(i));
    }

    // soldids
    xmlNodePtr solids = xmlNewChild(topology, 0, C_TAG_SOLIDS, 0);
    xmlNewProp(solids, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(m_geometry->countSolids()));
    for (int i = 0; i < m_geometry->countSolids(); ++i)
    {
        xmlNodePtr solid = xmlNewChild(solids, 0, C_TAG_SOLID, 0);
        xmlNewProp(solid, C_ATTR_ELT_INDEX, BAD_CAST XaoUtils::intToString(i));
        xmlNewProp(solid, C_ATTR_ELT_NAME, BAD_CAST m_geometry->getSolidName(i));
        xmlNewProp(solid, C_ATTR_ELT_REFERENCE, BAD_CAST m_geometry->getSolidReference(i));
    }
}

void Xao::exportGroups(xmlNodePtr xao)
{
    xmlNodePtr groups = xmlNewChild(xao, 0, C_TAG_GROUPS, 0);
    xmlNewProp(groups, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(countGroups()));

    for (std::list<Group*>::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        Group* grp = (*it);
        xmlNodePtr group = xmlNewChild(groups, 0, C_TAG_GROUP, 0);
        xmlNewProp(group, C_ATTR_GROUP_NAME, BAD_CAST grp->getName());
        xmlNewProp(group, C_ATTR_GROUP_DIM, BAD_CAST XaoUtils::dimensionToString(grp->getDimension()));
        xmlNewProp(group, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(grp->getCount()));

        for (int i = 0; i < grp->getCount(); ++i)
        {
            xmlNodePtr elt = xmlNewChild(group, 0, C_TAG_ELEMENT, 0);
            xmlNewProp(elt, C_ATTR_ELEMENT_INDEX, BAD_CAST XaoUtils::intToString(grp->getElement(i)));
        }
    }
}

bool Xao::importFromFile(const char* fileName)
{
    // parse the file and get the DOM
    int options = 16384; // merge cdata as text node
    xmlDocPtr doc = xmlReadFile(fileName, NULL, options);
    if (doc == NULL)
    {
        throw SALOME_Exception("Cannot read XAO file");
    }

    parseXMLDoc(doc);
    return true;
}

void Xao::parseXMLDoc(xmlDocPtr doc)
{
    // Get the root element node
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (xmlStrcmp(root->name , C_TAG_XAO) != 0)
        throw SALOME_Exception("Cannot read XAO file: invalid format XAO node not found");

    parseXaoNode(doc, root);

    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // Free globals
}

void Xao::parseXaoNode(xmlDocPtr doc, xmlNodePtr xaoNode)
{
    xmlChar* version = xmlGetProp(xaoNode, C_ATTR_XAO_VERSION);
    if (version != NULL)
    {
        setVersion((char*)version);
        xmlFree(version);
    }

    xmlChar* author = xmlGetProp(xaoNode, C_ATTR_XAO_AUTHOR);
    if (author != NULL)
    {
        setAuthor((char*)author);
        xmlFree(author);
    }

    for (xmlNodePtr node = xaoNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_GEOMETRY) == 0)
            parseGeometryNode(doc, node);
        else if (xmlStrcmp(node->name, C_TAG_GROUPS) == 0)
            parseGroupsNode(node);
    }
}

void Xao::parseGeometryNode(xmlDocPtr doc, xmlNodePtr geometryNode)
{
    //std::cout << ">> Geometry" << std::endl;
    if (m_geometry == NULL)
        m_geometry = new Geometry();

    xmlChar* name = xmlGetProp(geometryNode, C_ATTR_GEOMETRY_NAME);
    if (name != NULL)
    {
        m_geometry->setName((char*)name);
        xmlFree(name);
    }

    for (xmlNodePtr node = geometryNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_SHAPE) == 0)
            parseShapeNode(doc, node);
        else if (xmlStrcmp(node->name, C_TAG_TOPOLOGY) == 0)
            parseTopologyNode(node);
//        else
//            std::cout << "skip:" << node->name << std::endl;
    }

}

void Xao::parseShapeNode(xmlDocPtr doc, xmlNodePtr shapeNode)
{
    xmlChar* shapeType = xmlGetProp(shapeNode, C_ATTR_SHAPE_FORMAT);
    if (xmlStrcmp(shapeType, (xmlChar*)"BREP") == 0)
    {
        xmlChar* data = xmlNodeGetContent(shapeNode->children);
        if (data == NULL)
            throw SALOME_Exception("Missing BREP");
        m_geometry->setShape((char*)data);
        xmlFree(data);
    }
    else
    {
        throw SALOME_Exception("Shape format not supported");
    }
}

void Xao::parseTopologyNode(xmlNodePtr topologyNode)
{
//    std::cout << ">> Topology" << std::endl;
    for (xmlNodePtr node = topologyNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_VERTICES) == 0)
            parseVerticesNode(node);
        else if (xmlStrcmp(node->name, C_TAG_EDGES) == 0)
            parseEdgesNode(node);
        else if (xmlStrcmp(node->name, C_TAG_FACES) == 0)
            parseFacesNode(node);
        else if (xmlStrcmp(node->name, C_TAG_SOLIDS) == 0)
            parseSolidsNode(node);
    }
}
void Xao::parseVerticesNode(xmlNodePtr verticesNode)
{
    xmlChar* count = xmlGetProp(verticesNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for vertices");

    m_geometry->setCountVertices(atoi((char*)count));
    xmlFree(count);

    for (xmlNodePtr node = verticesNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_VERTEX) == 0)
        {
            xmlChar* index = xmlGetProp(node, C_ATTR_ELT_INDEX);
            if (index == NULL)
                throw SALOME_Exception("Bad index for vertex");

            xmlChar* name = xmlGetProp(node, C_ATTR_ELT_NAME);
            if (name == NULL)
                name = (xmlChar*)"";

            xmlChar* reference = xmlGetProp(node, C_ATTR_ELT_REFERENCE);
            if (reference == NULL)
                throw SALOME_Exception("Bad reference for vertex");

            m_geometry->setVertex(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void Xao::parseEdgesNode(xmlNodePtr edgesNode)
{
    xmlChar* count = xmlGetProp(edgesNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for edges");

    m_geometry->setCountEdges(atoi((char*)count));
    xmlFree(count);

    for (xmlNodePtr node = edgesNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_EDGE) == 0)
        {
            xmlChar* index = xmlGetProp(node, C_ATTR_ELT_INDEX);
            if (index == NULL)
                throw SALOME_Exception("Bad index for edge");

            xmlChar* name = xmlGetProp(node, C_ATTR_ELT_NAME);
            if (name == NULL)
                name = (xmlChar*)"";

            xmlChar* reference = xmlGetProp(node, C_ATTR_ELT_REFERENCE);
            if (reference == NULL)
                throw SALOME_Exception("Bad reference for edge");

            m_geometry->setEdge(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void Xao::parseFacesNode(xmlNodePtr facesNode)
{
    xmlChar* count = xmlGetProp(facesNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for faces");

    m_geometry->setCountFaces(atoi((char*)count));
    xmlFree(count);

    for (xmlNodePtr node = facesNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_FACE) == 0)
        {
            xmlChar* index = xmlGetProp(node, C_ATTR_ELT_INDEX);
            if (index == NULL)
                throw SALOME_Exception("Bad index for face");

            xmlChar* name = xmlGetProp(node, C_ATTR_ELT_NAME);
            if (name == NULL)
                name = (xmlChar*)"";

            xmlChar* reference = xmlGetProp(node, C_ATTR_ELT_REFERENCE);
            if (reference == NULL)
                throw SALOME_Exception("Bad reference for face");

            m_geometry->setFace(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void Xao::parseSolidsNode(xmlNodePtr solidsNode)
{
    xmlChar* count = xmlGetProp(solidsNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for solids");

    m_geometry->setCountSolids(atoi((char*)count));
    xmlFree(count);

    for (xmlNodePtr node = solidsNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_SOLID) == 0)
        {
            xmlChar* index = xmlGetProp(node, C_ATTR_ELT_INDEX);
            if (index == NULL)
                throw SALOME_Exception("Bad index for solid");

            xmlChar* name = xmlGetProp(node, C_ATTR_ELT_NAME);
            if (name == NULL)
                name = (xmlChar*)"";

            xmlChar* reference = xmlGetProp(node, C_ATTR_ELT_REFERENCE);
            if (reference == NULL)
                throw SALOME_Exception("Bad reference for solid");

            m_geometry->setSolid(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void Xao::parseGroupsNode(xmlNodePtr groupsNode)
{
//    xmlChar* count = xmlGetProp(groupsNode, C_ATTR_COUNT);
//    if (count == NULL)
//        throw SALOME_Exception("No count attribute for groups");

    for (xmlNodePtr node = groupsNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_GROUP) == 0)
        {
            parseGroupNode(node);
        }
    }
}

void Xao::parseGroupNode(xmlNodePtr groupNode)
{
//    xmlChar* count = xmlGetProp(groupNode, C_ATTR_COUNT);
//    if (count == NULL)
//        throw SALOME_Exception("Bad count for group");

    xmlChar* dimension = xmlGetProp(groupNode, C_ATTR_GROUP_DIM);
    if (dimension == NULL)
        throw SALOME_Exception("Bad dimension for group");

    Group* group = new Group();

    xmlChar* name = xmlGetProp(groupNode, C_ATTR_GROUP_NAME);
    if (name == NULL) name = (xmlChar*)"";
    group->setName((char*)name);
    xmlFree(name);

    group->setDimension(XaoUtils::stringToDimension((char*)dimension));
    xmlFree(dimension);

    addGroup(group);

    for (xmlNodePtr node = groupNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_ELEMENT) == 0)
        {
            xmlChar* index = xmlGetProp(node, C_ATTR_ELEMENT_INDEX);
            if (index == NULL)
                throw SALOME_Exception("Bad index for group element");
            group->addElement(atoi((char*)index));
            xmlFree(index);
        }
    }

}
