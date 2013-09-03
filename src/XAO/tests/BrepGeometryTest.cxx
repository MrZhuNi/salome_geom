#include <vector>
#include <Utils_SALOME_Exception.hxx>

#include "TestUtils.hxx"
#include "BrepGeometryTest.hxx"
#include "../Xao.hxx"
#include "../BrepGeometry.hxx"

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
    geom->setBREP(txt);
}

void BrepGeometryTest::testGetIDs()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_1.brep");

    int vertices[8] = { 6,7,9,11,16,17,19,21 };
    for (int i = 0; i < 8; ++i)
        CPPUNIT_ASSERT_EQUAL(vertices[i], geom->getVertexID(i));

    int edges[12] = { 5,8,10,12,15,18,20,22,25,26,29,30 };
    for (int i = 0; i < 12; ++i)
        CPPUNIT_ASSERT_EQUAL(edges[i], geom->getEdgeID(i));

    int faces[6] = { 3,13,23,27,31,33 };
    for (int i = 0; i < 6; ++i)
        CPPUNIT_ASSERT_EQUAL(faces[i], geom->getFaceID(i));

    CPPUNIT_ASSERT_EQUAL(1, geom->getSolidID(0));

    delete geom;
}

void BrepGeometryTest::testGetEdgeVertices()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    int v1, v2;
    geom->getEdgeVertices(63, v1, v2);
    std::cout << "# " << v1 << ", " << v2 << std::endl;
    CPPUNIT_ASSERT_EQUAL(47, v1);
    CPPUNIT_ASSERT_EQUAL(59, v2);

    delete geom;
}

void BrepGeometryTest::testGetFaceWires()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    CPPUNIT_ASSERT_EQUAL(2, geom->countFaceWires(13));
    CPPUNIT_ASSERT_EQUAL(1, geom->countFaceWires(29));

    std::vector<int> wires = geom->getFaceWires(13);
    CPPUNIT_ASSERT_EQUAL(2, (int)wires.size());
    CPPUNIT_ASSERT_EQUAL(2, wires[0]);
    CPPUNIT_ASSERT_EQUAL(11, wires[1]);

    delete geom;
}

void BrepGeometryTest::testSolidShells()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Cut_2.brep");

    CPPUNIT_ASSERT_EQUAL(5, geom->countSolidShells(1));

    std::vector<int> shells = geom->getSolidShells(1);
    CPPUNIT_ASSERT_EQUAL(5, (int)shells.size());
    int ids[5] = { 2, 35, 68, 76, 84 };
    for (int i = 0; i < 5; ++i)
        CPPUNIT_ASSERT_EQUAL(ids[i], shells[i]);

    delete geom;
}

void BrepGeometryTest::testGetVertex()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    double x, y, z;
    geom->getVertexXYZ(59, x, y, z);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(60., x, 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(80., y, 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(60., z, 1e-6);

    delete geom;
}

void BrepGeometryTest::testGetEdgeLength()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    CPPUNIT_ASSERT_DOUBLES_EQUAL(200., geom->getEdgeLength(5), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(80., geom->getEdgeLength(21), 0);

    delete geom;
}

void BrepGeometryTest::testGetFaceArea()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    CPPUNIT_ASSERT_DOUBLES_EQUAL(40000., geom->getFaceArea(3), 1e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(33600., geom->getFaceArea(13), 1e-9);

    delete geom;
}

void BrepGeometryTest::testGetSolidVolume()
{
    BrepGeometry* geom = new BrepGeometry("box");
    readBrep(geom, "Box_2.brep");

    CPPUNIT_ASSERT_DOUBLES_EQUAL(7488000., geom->getSolidVolume(1), 1e-9);

    delete geom;
}
