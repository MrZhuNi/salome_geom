#include <vector>
#include <Utils_SALOME_Exception.hxx>

#include "TestUtils.hxx"
#include "BrepGeometryTest.hxx"
#include "../XAO_Xao.hxx"
#include "../XAO_BrepGeometry.hxx"

using namespace XAO;

void BrepGeometryTest::setUp()
{
}

void BrepGeometryTest::tearDown()
{
}

void BrepGeometryTest::cleanUp()
{
}

void readBrep(Geometry* geom, const std::string& fileName)
{
    char* txt = TestUtils::readTextFile(TestUtils::getTestFilePath(fileName));
    geom->setShape(txt);
}

void BrepGeometryTest::testGetIDs()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_1.brep");

    CPPUNIT_ASSERT_EQUAL(8, geom->countElements(XAO::VERTEX));
    CPPUNIT_ASSERT_EQUAL(8, geom->countVertices());
    int vertices[8] = { 6,7,9,11,16,17,19,21 };
    for (int i = 0; i < 8; ++i)
        CPPUNIT_ASSERT_EQUAL(vertices[i], geom->getVertexID(i));

    CPPUNIT_ASSERT_EQUAL(12, geom->countElements(XAO::EDGE));
    CPPUNIT_ASSERT_EQUAL(12, geom->countEdges());
    int edges[12] = { 5,8,10,12,15,18,20,22,25,26,29,30 };
    for (int i = 0; i < 12; ++i)
        CPPUNIT_ASSERT_EQUAL(edges[i], geom->getEdgeID(i));

    CPPUNIT_ASSERT_EQUAL(6, geom->countElements(XAO::FACE));
    CPPUNIT_ASSERT_EQUAL(6, geom->countFaces());
    int faces[6] = { 3,13,23,27,31,33 };
    for (int i = 0; i < 6; ++i)
        CPPUNIT_ASSERT_EQUAL(faces[i], geom->getFaceID(i));

    CPPUNIT_ASSERT_EQUAL(1, geom->countElements(XAO::SOLID));
    CPPUNIT_ASSERT_EQUAL(1, geom->countSolids());
    CPPUNIT_ASSERT_EQUAL(1, geom->getSolidID(0));

    delete geom;
}

void BrepGeometryTest::testGetReferences()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_1.brep");

    // vertex of index 1 has id = 7
    CPPUNIT_ASSERT_EQUAL(std::string("7"), geom->getElementReference(XAO::VERTEX, 1));
    CPPUNIT_ASSERT_EQUAL(std::string("7"), geom->getVertexReference(1));
    CPPUNIT_ASSERT_EQUAL(7, geom->getVertexID(1));
    CPPUNIT_ASSERT_EQUAL(1, geom->getElementIndexByReference(XAO::VERTEX, "7"));
    CPPUNIT_ASSERT_EQUAL(1, geom->findVertex(7));

    // edge of index 1 has id = 8
    CPPUNIT_ASSERT_EQUAL(std::string("8"), geom->getElementReference(XAO::EDGE, 1));
    CPPUNIT_ASSERT_EQUAL(std::string("8"), geom->getEdgeReference(1));
    CPPUNIT_ASSERT_EQUAL(8, geom->getEdgeID(1));
    CPPUNIT_ASSERT_EQUAL(1, geom->getElementIndexByReference(XAO::EDGE, "8"));
    CPPUNIT_ASSERT_EQUAL(1, geom->findEdge(8));

    // face of index 1 has id = 13
    CPPUNIT_ASSERT_EQUAL(std::string("13"), geom->getElementReference(XAO::FACE, 1));
    CPPUNIT_ASSERT_EQUAL(std::string("13"), geom->getFaceReference(1));
    CPPUNIT_ASSERT_EQUAL(13, geom->getFaceID(1));
    CPPUNIT_ASSERT_EQUAL(1, geom->getElementIndexByReference(XAO::FACE, "13"));
    CPPUNIT_ASSERT_EQUAL(1, geom->findFace(13));

    // solid of index 0 has id = 1
    CPPUNIT_ASSERT_EQUAL(std::string("1"), geom->getElementReference(XAO::SOLID, 0));
    CPPUNIT_ASSERT_EQUAL(std::string("1"), geom->getSolidReference(0));
    CPPUNIT_ASSERT_EQUAL(1, geom->getSolidID(0));
    CPPUNIT_ASSERT_EQUAL(0, geom->getElementIndexByReference(XAO::SOLID, "1"));
    CPPUNIT_ASSERT_EQUAL(0, geom->findSolid(1));

    delete geom;
}

void BrepGeometryTest::testGetEdgeVertices()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    // edge of index 23, id = #63
    // vertex are 47 (#12), 59 (#15)
    int v1, v2;
    geom->getEdgeVertices(23, v1, v2);
    CPPUNIT_ASSERT_EQUAL(12, v1);
    CPPUNIT_ASSERT_EQUAL(15, v2);

    delete geom;
}

void printVector(std::vector<int>& v)
{
    std::cout << "# ";
    for (int i = 0; i < v.size(); i++)
        std::cout << v[i] << ", ";
    std::cout << std::endl;
}

void BrepGeometryTest::testGetFaceEdges()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    CPPUNIT_ASSERT_EQUAL(2, geom->countFaceWires(1)); // face 13
    CPPUNIT_ASSERT_EQUAL(1, geom->countFaceWires(2)); // face 29

    // wire 0 of face 1 (#13) => edge 4 (#15), 5 (#17), 0 (#5), 6 (#19)
    std::vector<int> edges = geom->getFaceEdges(1, 0);
    CPPUNIT_ASSERT_EQUAL(4, (int)edges.size());
    int ids1[4] = { 4,5,0,6 };
    for (int i = 0; i < 4; ++i)
        CPPUNIT_ASSERT_EQUAL(ids1[i], edges[i]);

    // wire 1 of face 13 => edge 7 (#21) ,8 (#24), 9 (#26), 10 (#28)
    edges = geom->getFaceEdges(1, 1);
    CPPUNIT_ASSERT_EQUAL(4, (int)edges.size());
    int ids2[4] = { 7,8,9,10 };
    for (int i = 0; i < 4; ++i)
        CPPUNIT_ASSERT_EQUAL(ids2[i], edges[i]);

    delete geom;
}

void BrepGeometryTest::testSolidFaces()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Cut_2.brep");

    CPPUNIT_ASSERT_EQUAL(5, geom->countSolidShells(0));

    std::vector<int> faces = geom->getSolidFaces(0, 0);
    CPPUNIT_ASSERT_EQUAL(6, (int)faces.size());
    int ids[6] = { 0, 1, 2, 3, 4, 5 };
    for (int i = 0; i < 6; ++i)
        CPPUNIT_ASSERT_EQUAL(ids[i], faces[i]);

    faces = geom->getSolidFaces(0, 1);
    CPPUNIT_ASSERT_EQUAL(6, (int)faces.size());
    int ids2[6] = { 6, 7, 8, 9, 10, 11 };
    for (int i = 0; i < 6; ++i)
        CPPUNIT_ASSERT_EQUAL(ids2[i], faces[i]);

    delete geom;
}

void BrepGeometryTest::testGetVertex()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    double x, y, z;
    geom->getVertexXYZ(15, x, y, z);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(60., x, 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(80., y, 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(60., z, 1e-6);

    delete geom;
}

void BrepGeometryTest::testGetEdgeLength()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    // edges 0 (#5), 7 (#21)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(200., geom->getEdgeLength(0), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(80., geom->getEdgeLength(7), 0);

    delete geom;
}

void BrepGeometryTest::testGetFaceArea()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    // faces 0 (#3), 1 (#13)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(40000., geom->getFaceArea(0), 1e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(33600., geom->getFaceArea(1), 1e-9);

    delete geom;
}

void BrepGeometryTest::testGetSolidVolume()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    CPPUNIT_ASSERT_DOUBLES_EQUAL(7488000., geom->getSolidVolume(0), 1e-9);

    delete geom;
}
