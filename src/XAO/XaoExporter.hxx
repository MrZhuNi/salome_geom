
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
        static const bool saveToFile(Xao* xaoObject, const std::string& fileName);
        static const std::string saveToXml(Xao* xaoObject);

        static const bool readFromFile(const std::string& fileName, Xao* xaoObject);
        static const bool setXML(const std::string& xml, Xao* xaoObject);

    private:
        static xmlDocPtr exportXMLDoc(Xao* xaoObject);
        static void exportGeometry(Geometry* xaoGeometry, xmlDocPtr doc, xmlNodePtr xao);
        static void exportGeometricElements(Geometry* xaoGeometry, xmlNodePtr topology,
                XAO::Dimension dim, const xmlChar* colTag, const xmlChar* eltTag);
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

        static void parseFieldsNode(xmlNodePtr fieldsNode, Xao* xaoObject);
        static void parseFieldNode(xmlNodePtr fieldNode, Xao* xaoObject);
        static void parseStepNode(xmlNodePtr stepNode, Field* field);
        static void parseStepElementNode(xmlNodePtr eltNode, Step* step);

        static std::string readStringProp(xmlNodePtr node, const xmlChar* attribute,
                const bool& required, const std::string& defaultValue, const std::string& exception = std::string(""));
        static int readIntegerProp(xmlNodePtr node, const xmlChar* attribute,
                const bool& required, const int& defaultValue, const std::string& exception = std::string(""));
    };
}


#endif /* __XAO_XAOEXPORTER_HXX__ */
