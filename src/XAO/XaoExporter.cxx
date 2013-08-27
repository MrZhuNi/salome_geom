
#include <libxml/parser.h>
#include <Utils_SALOME_Exception.hxx>

#include "XaoExporter.hxx"
#include "Xao.hxx"
#include "Geometry.hxx"
#include "Group.hxx"
#include "Field.hxx"
#include "XaoUtils.hxx"

namespace XAO
{
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
    const xmlChar* C_TAG_VALUE = (xmlChar*)"value";
    const xmlChar* C_ATTR_VALUE_COMPONENT = (xmlChar*)"component";

    const xmlChar* C_TAG_FIELDS = (xmlChar*)"fields";
    const xmlChar* C_TAG_FIELD = (xmlChar*)"field";
    const xmlChar* C_ATTR_FIELD_NAME = (xmlChar*)"name";
    const xmlChar* C_ATTR_FIELD_TYPE = (xmlChar*)"type";
    const xmlChar* C_ATTR_FIELD_DIMENSION = (xmlChar*)"dimension";
    const xmlChar* C_TAG_COMPONENTS = (xmlChar*)"components";
    const xmlChar* C_TAG_COMPONENT = (xmlChar*)"component";
    const xmlChar* C_ATTR_COMPONENT_COLUMN = (xmlChar*)"column";
    const xmlChar* C_ATTR_COMPONENT_NAME = (xmlChar*)"name";

    const xmlChar* C_TAG_STEPS = (xmlChar*)"steps";
    const xmlChar* C_TAG_STEP = (xmlChar*)"step";
    const xmlChar* C_ATTR_STEP_NUMBER = (xmlChar*)"number";
    const xmlChar* C_ATTR_STEP_STAMP = (xmlChar*)"stamp";
}

using namespace XAO;

bool XaoExporter::saveToFile(Xao* xaoObject, const char* fileName)
{
    xmlDocPtr doc = exportXMLDoc(xaoObject);
    xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 2);
    xmlFreeDoc(doc);

    return true;
}

const char* XaoExporter::saveToXml(Xao* xaoObject)
{
    xmlDocPtr doc = exportXMLDoc(xaoObject);

    xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    xmlFreeDoc(doc);
    xmlCleanupGlobals();

    return (char*)xmlbuff;
}

xmlDocPtr XaoExporter::exportXMLDoc(Xao* xaoObject)
{
    // Creating the Xml document
    xmlDocPtr masterDocument = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr xao = xmlNewNode(0, C_TAG_XAO);
    xmlDocSetRootElement(masterDocument, xao);

    xmlNewProp(xao, C_ATTR_XAO_VERSION, BAD_CAST xaoObject->getVersion());
    xmlNewProp(xao, C_ATTR_XAO_AUTHOR, BAD_CAST xaoObject->getAuthor());

    if (xaoObject->getGeometry() != NULL)
    {
        exportGeometry(xaoObject->getGeometry(), masterDocument, xao);
    }

    exportGroups(xaoObject, xao);
    exportFields(xaoObject, xao);

    return masterDocument;
}

void XaoExporter::exportGeometricElements(Geometry* xaoGeometry,
        xmlNodePtr topology, Dimension dim, const xmlChar* colTag, const xmlChar* eltTag)
{
    xmlNodePtr vertices = xmlNewChild(topology, 0, colTag, 0);
    xmlNewProp(vertices, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(xaoGeometry->countVertices()).c_str());
    GeometricElementList::iterator it = xaoGeometry->begin(dim);
    for (; it != xaoGeometry->end(dim); it++)
    {
        int index = it->first;
        GeometricElement elt = it->second;
        xmlNodePtr vertex = xmlNewChild(vertices, 0, eltTag, 0);
        xmlNewProp(vertex, C_ATTR_ELT_INDEX, BAD_CAST XaoUtils::intToString(index).c_str());
        //xmlNewProp(vertex, C_ATTR_ELT_NAME, BAD_CAST xaoGeometry->getVertexName(i));
        //xmlNewProp(vertex, C_ATTR_ELT_REFERENCE, BAD_CAST xaoGeometry->getVertexReference(i));
        xmlNewProp(vertex, C_ATTR_ELT_NAME, BAD_CAST elt.getName());
        xmlNewProp(vertex, C_ATTR_ELT_REFERENCE, BAD_CAST elt.getReference());
    }
}

void XaoExporter::exportGeometry(Geometry* xaoGeometry, xmlDocPtr doc, xmlNodePtr xao)
{
    // Geometric part
    xmlNodePtr geometry = xmlNewChild(xao, 0, C_TAG_GEOMETRY, 0);
    xmlNewProp(geometry, C_ATTR_GEOMETRY_NAME, BAD_CAST xaoGeometry->getName());

    xmlNodePtr shape = xmlNewChild(geometry, 0, C_TAG_SHAPE, 0);
    xmlNewProp(shape, C_ATTR_SHAPE_FORMAT, BAD_CAST xaoGeometry->getFormat());
    const char* brep = xaoGeometry->getBREP();
    xmlNodePtr cdata = xmlNewCDataBlock(doc, BAD_CAST brep, strlen(brep));
    xmlAddChild(shape, cdata);

    xmlNodePtr topology = xmlNewChild(geometry, 0, C_TAG_TOPOLOGY, 0);

    exportGeometricElements(xaoGeometry, topology, VERTEX, C_TAG_VERTICES, C_TAG_VERTEX);
    exportGeometricElements(xaoGeometry, topology, EDGE, C_TAG_EDGES, C_TAG_EDGE);
    exportGeometricElements(xaoGeometry, topology, FACE, C_TAG_FACES, C_TAG_FACE);
    exportGeometricElements(xaoGeometry, topology, SOLID, C_TAG_SOLIDS, C_TAG_SOLID);
}

void XaoExporter::exportGroups(Xao* xaoObject, xmlNodePtr xao)
{
    xmlNodePtr groups = xmlNewChild(xao, 0, C_TAG_GROUPS, 0);
    xmlNewProp(groups, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(xaoObject->countGroups()).c_str());

    //for (std::list<Group*>::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    for (int i = 0; i < xaoObject->countGroups(); i++)
    {
        //Group* grp = (*it);
        Group* grp = xaoObject->getGroup(i);
        xmlNodePtr group = xmlNewChild(groups, 0, C_TAG_GROUP, 0);
        xmlNewProp(group, C_ATTR_GROUP_NAME, BAD_CAST grp->getName());
        xmlNewProp(group, C_ATTR_GROUP_DIM, BAD_CAST XaoUtils::dimensionToString(grp->getDimension()));
        xmlNewProp(group, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(grp->getCount()).c_str());

        for (int j = 0; j < grp->getCount(); j++)
        {
            xmlNodePtr elt = xmlNewChild(group, 0, C_TAG_ELEMENT, 0);
            xmlNewProp(elt, C_ATTR_ELEMENT_INDEX, BAD_CAST XaoUtils::intToString(grp->getElement(j)).c_str());
        }
    }
}

void XaoExporter::exportFields(Xao* xaoObject, xmlNodePtr xao)
{
    xmlNodePtr fields = xmlNewChild(xao, 0, C_TAG_FIELDS, 0);
    xmlNewProp(fields, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(xaoObject->countFields()).c_str());

    for (int i = 0; i < xaoObject->countFields(); i++)
    {
        Field* field = xaoObject->getField(i);
        xmlNodePtr nodeField = xmlNewChild(fields, 0, C_TAG_FIELD, 0);
        xmlNewProp(nodeField, C_ATTR_FIELD_NAME, BAD_CAST field->getName().c_str());
        xmlNewProp(nodeField, C_ATTR_FIELD_TYPE, BAD_CAST XaoUtils::fieldTypeToString(field->getType()));
        xmlNewProp(nodeField, C_ATTR_FIELD_DIMENSION, BAD_CAST XaoUtils::dimensionToString(field->getDimension()));

        int nbComponents = field->countComponents();
        xmlNodePtr components = xmlNewChild(nodeField, 0, C_TAG_COMPONENTS, 0);
        xmlNewProp(components, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(nbComponents).c_str());

        for (int j = 0; j < nbComponents; j++)
        {
            xmlNodePtr nodeComponent = xmlNewChild(components, 0, C_TAG_COMPONENT, 0);
            xmlNewProp(nodeComponent, C_ATTR_COMPONENT_COLUMN, BAD_CAST XaoUtils::intToString(j).c_str());
            xmlNewProp(nodeComponent, C_ATTR_COMPONENT_NAME, BAD_CAST field->getComponentName(j).c_str());
        }

        int nbSteps = field->countSteps();
        xmlNodePtr nodeSteps = xmlNewChild(nodeField, 0, C_TAG_STEPS, 0);
        xmlNewProp(nodeSteps, C_ATTR_COUNT, BAD_CAST XaoUtils::intToString(nbSteps).c_str());
        //for (int j = 0; j < nbSteps; j++)
//        for (stepIterator itStep = field->begin(); itStep != field->end(); itStep++)
//        {
//            Step* step = (*itStep).second;
//            exportStep(step, field, nodeSteps);
//        }
    }
}

void XaoExporter::exportStep(Step* step, Field* field, xmlNodePtr nodeSteps)
{
    xmlNodePtr nodeStep = xmlNewChild(nodeSteps, 0, C_TAG_STEP, 0);
    xmlNewProp(nodeStep, C_ATTR_STEP_NUMBER, BAD_CAST XaoUtils::intToString(step->getStep()).c_str());
    if (step->getStamp() >= 0)
    {
        xmlNewProp(nodeStep, C_ATTR_STEP_STAMP, BAD_CAST XaoUtils::intToString(step->getStamp()).c_str());
    }

    /*
    for(stepElementIterator itElt = step->begin(); itElt != step->end(); itElt++)
    {
        StepElement* elt = (*itElt);
        xmlNodePtr nodeElt = xmlNewChild(nodeStep, 0, C_TAG_ELEMENT, 0);
        xmlNewProp(nodeElt, C_ATTR_ELEMENT_INDEX, BAD_CAST XaoUtils::intToString(elt->getElement()).c_str());

        for (int i = 0; i < elt->getNbComponents(); i++)
        {
            // TODO: do other type
            std::string content;
            switch (field->getType())
            {
                case FIELD_BOOLEAN:
                {
                    BooleanStepElement* boolElt = (BooleanStepElement*)elt;
                    content = (boolElt->getValue(i) ? "true" : "false");
                    break;
                }
                case FIELD_STRING:
                {
                    StringStepElement* strElt = (StringStepElement*)elt;
                    content = strElt->getValue(i);
                    break;
                }
                case FIELD_INTEGER:
                {
                    IntegerStepElement* intElt = (IntegerStepElement*)elt;
                    content = XaoUtils::intToString(intElt->getValue(i));
                    break;
                }
                case FIELD_DOUBLE:
                {
                    DoubleStepElement* dElt = (DoubleStepElement*)elt;
                    content = XaoUtils::doubleToString(dElt->getValue(i));
                    break;
                }
            }

            xmlNodePtr nodeValue= xmlNewTextChild(nodeElt, 0, C_TAG_VALUE, BAD_CAST content.c_str());
            xmlNewProp(nodeValue, C_ATTR_VALUE_COMPONENT, BAD_CAST XaoUtils::intToString(i).c_str());
        }
    }*/
}

bool XaoExporter::readFromFile(const char* fileName, Xao* xaoObject)
{
    // parse the file and get the DOM
    int options = XML_PARSE_HUGE || XML_PARSE_NOCDATA;
    xmlDocPtr doc = xmlReadFile(fileName, NULL, options);
    if (doc == NULL)
    {
        throw SALOME_Exception("Cannot read XAO file");
    }

    parseXMLDoc(doc, xaoObject);
    return true;
}

bool XaoExporter::setXML(const char* xml, Xao* xaoObject)
{
    int options = XML_PARSE_HUGE || XML_PARSE_NOCDATA;
    xmlDocPtr doc = xmlReadDoc(BAD_CAST xml, "", NULL, options);
    if (doc == NULL)
    {
        throw SALOME_Exception("Cannot read XAO stream");
    }

    parseXMLDoc(doc, xaoObject);
    return true;
}

void XaoExporter::parseXMLDoc(xmlDocPtr doc, Xao* xaoObject)
{
    // Get the root element node
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (xmlStrcmp(root->name , C_TAG_XAO) != 0)
        throw SALOME_Exception("Cannot read XAO file: invalid format XAO node not found");

    parseXaoNode(doc, root, xaoObject);

    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // free globals
}

void XaoExporter::parseXaoNode(xmlDocPtr doc, xmlNodePtr xaoNode, Xao* xaoObject)
{
    xmlChar* version = xmlGetProp(xaoNode, C_ATTR_XAO_VERSION);
    if (version != NULL)
    {
        xaoObject->setVersion((char*)version);
        xmlFree(version);
    }

    xmlChar* author = xmlGetProp(xaoNode, C_ATTR_XAO_AUTHOR);
    if (author != NULL)
    {
        xaoObject->setAuthor((char*)author);
        xmlFree(author);
    }

    for (xmlNodePtr node = xaoNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_GEOMETRY) == 0)
            parseGeometryNode(doc, node, xaoObject);
        else if (xmlStrcmp(node->name, C_TAG_GROUPS) == 0)
            parseGroupsNode(node, xaoObject);
    }
}

void XaoExporter::parseGeometryNode(xmlDocPtr doc, xmlNodePtr geometryNode, Xao* xaoObject)
{
    Geometry* geometry = new Geometry();

    xmlChar* name = xmlGetProp(geometryNode, C_ATTR_GEOMETRY_NAME);
    if (name != NULL)
    {
        geometry->setName((char*)name);
        xmlFree(name);
    }

    for (xmlNodePtr node = geometryNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_SHAPE) == 0)
            parseShapeNode(doc, node, geometry);
        else if (xmlStrcmp(node->name, C_TAG_TOPOLOGY) == 0)
            parseTopologyNode(node, geometry);
    }

    xaoObject->setGeometry(geometry);
}

void XaoExporter::parseShapeNode(xmlDocPtr doc, xmlNodePtr shapeNode, Geometry* geometry)
{
    xmlChar* shapeType = xmlGetProp(shapeNode, C_ATTR_SHAPE_FORMAT);
    if (xmlStrcmp(shapeType, (xmlChar*)"BREP") == 0)
    {
        xmlChar* data = xmlNodeGetContent(shapeNode->children);
        if (data == NULL)
            throw SALOME_Exception("Missing BREP");
        geometry->setShape((char*)data);
        xmlFree(data);
    }
    else
    {
        throw SALOME_Exception("Shape format not supported");
    }
}

void XaoExporter::parseTopologyNode(xmlNodePtr topologyNode, Geometry* geometry)
{
    for (xmlNodePtr node = topologyNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_VERTICES) == 0)
            parseVerticesNode(node, geometry);
        else if (xmlStrcmp(node->name, C_TAG_EDGES) == 0)
            parseEdgesNode(node, geometry);
        else if (xmlStrcmp(node->name, C_TAG_FACES) == 0)
            parseFacesNode(node, geometry);
        else if (xmlStrcmp(node->name, C_TAG_SOLIDS) == 0)
            parseSolidsNode(node, geometry);
    }
}

void XaoExporter::parseVerticesNode(xmlNodePtr verticesNode, Geometry* geometry)
{
    xmlChar* count = xmlGetProp(verticesNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for vertices");

    geometry->setCountVertices(atoi((char*)count));
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

            geometry->setVertex(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void XaoExporter::parseEdgesNode(xmlNodePtr edgesNode, Geometry* geometry)
{
    xmlChar* count = xmlGetProp(edgesNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for edges");

    geometry->setCountEdges(atoi((char*)count));
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

            geometry->setEdge(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void XaoExporter::parseFacesNode(xmlNodePtr facesNode, Geometry* geometry)
{
    xmlChar* count = xmlGetProp(facesNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for faces");

    geometry->setCountFaces(atoi((char*)count));
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

            geometry->setFace(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void XaoExporter::parseSolidsNode(xmlNodePtr solidsNode, Geometry* geometry)
{
    xmlChar* count = xmlGetProp(solidsNode, C_ATTR_COUNT);
    if (count == NULL)
        throw SALOME_Exception("No count attribute for solids");

    geometry->setCountSolids(atoi((char*)count));
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

            geometry->setSolid(atoi((char*)index), (char*)name, (char*)reference);
        }
    }
}

void XaoExporter::parseGroupsNode(xmlNodePtr groupsNode, Xao* xaoObject)
{
    for (xmlNodePtr node = groupsNode->children; node; node = node->next)
    {
        if (xmlStrcmp(node->name, C_TAG_GROUP) == 0)
        {
            parseGroupNode(node, xaoObject);
        }
    }
}

void XaoExporter::parseGroupNode(xmlNodePtr groupNode, Xao* xaoObject)
{
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

    xaoObject->addGroup(group);

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
