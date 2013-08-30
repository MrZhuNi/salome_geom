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
        CPPUNIT_ASSERT(geom->getVertexID(i) == vertices[i]);

    int edges[12] = { 5,8,10,12,15,18,20,22,25,26,29,30 };
    for (int i = 0; i < 12; ++i)
        CPPUNIT_ASSERT(geom->getEdgeID(i) == edges[i]);

    int faces[6] = { 3,13,23,27,31,33 };
    for (int i = 0; i < 6; ++i)
        CPPUNIT_ASSERT(geom->getFaceID(i) == faces[i]);

    CPPUNIT_ASSERT(geom->getSolidID(0) == 1);
}



