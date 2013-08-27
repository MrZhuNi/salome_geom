
#ifndef __XAO_XAOEXPORTER_HXX__
#define __XAO_XAOEXPORTER_HXX__

#include <libxml/parser.h>

#include "Xao.hxx"
#include "Geometry.hxx"
#include "Group.hxx"
#include "Field.hxx"

namespace XAO
{
    class XaoExporter
    {
public:
        static bool saveToFile(Xao* xaoObject, const char* fileName);
        static const char* saveToXml(Xao* xaoObject);

        static bool readFromFile(const char* fileName, Xao* xaoObject);
        static bool setXML(const char* xml, Xao* xaoObject);
private:
        static xmlDocPtr exportXMLDoc(Xao* xaoObject);
        static void exportGeometry(Geometry* xaoGeometry, xmlDocPtr doc, xmlNodePtr xao);
        static void exportGeometricElements(Geometry* xaoGeometry, xmlNodePtr topology,
                Dimension dim, const xmlChar* colTag, const xmlChar* eltTag);
        static void exportGroups(Xao* xaoObject, xmlNodePtr xao);
        static void exportFields(Xao* xaoObject, xmlNodePtr xao);
        static void exportStep(Step* step, Field* field, xmlNodePtr nodeSteps);

        static void parseXMLDoc(xmlDocPtr doc, Xao* xaoObject);
        static void parseXaoNode(xmlDocPtr doc, xmlNodePtr xaoNode, Xao* xaoObject);
        static void parseGeometryNode(xmlDocPtr doc, xmlNodePtr geometryNode, Xao* xaoObject);
        static void parseShapeNode(xmlDocPtr doc, xmlNodePtr shapeNode, Geometry* geometry);
        static void parseTopologyNode(xmlNodePtr topologyNode, Geometry* geometry);
        static void parseVerticesNode(xmlNodePtr verticesNode, Geometry* geometry);
        static void parseEdgesNode(xmlNodePtr edgesNode, Geometry* geometry);
        static void parseFacesNode(xmlNodePtr facesNode, Geometry* geometry);
        static void parseSolidsNode(xmlNodePtr solidsNode, Geometry* geometry);
        static void parseGroupsNode(xmlNodePtr groupsNode, Xao* xaoObject);
        static void parseGroupNode(xmlNodePtr groupNode, Xao* xaoObject);
    };
}


#endif /* __XAO_XAOEXPORTER_HXX__ */
