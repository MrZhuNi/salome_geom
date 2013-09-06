#include <vector>
#include <Utils_SALOME_Exception.hxx>

#include "TestUtils.hxx"
#include "GeometryTest.hxx"
#include "../XAO_Geometry.hxx"

using namespace XAO;

void GeometryTest::setUp()
{
}

void GeometryTest::tearDown()
{
}

void GeometryTest::cleanUp()
{
}

void GeometryTest::testGeometry()
{
    Geometry* geom = Geometry::createGeometry(XAO::BREP, "cube");

    CPPUNIT_ASSERT_EQUAL(std::string("cube"), geom->getName());
    CPPUNIT_ASSERT_EQUAL(XAO::BREP, geom->getFormat());

    geom->setName("sphere");
    CPPUNIT_ASSERT_EQUAL(std::string("sphere"), geom->getName());
}

void GeometryTest::testGeometryErrors()
{
    CPPUNIT_ASSERT_THROW(Geometry::createGeometry(XAO::STEP), SALOME_Exception);
}
