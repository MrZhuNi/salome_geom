#ifndef __XAO_BREPGEOMETRY_TEST_HXX__
#define __XAO_BREPGEOMETRY_TEST_HXX__

#include <cppunit/extensions/HelperMacros.h>

#include "../Xao.hxx"

namespace XAO
{
    class BrepGeometryTest: public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE(BrepGeometryTest);
        CPPUNIT_TEST(testGetIDs);
        CPPUNIT_TEST(testGetReferences);
        CPPUNIT_TEST(testGetEdgeVertices);
        CPPUNIT_TEST(testGetFaceWires);
        CPPUNIT_TEST(testSolidShells);
        CPPUNIT_TEST(testGetVertex);
        CPPUNIT_TEST(testGetEdgeLength);
        CPPUNIT_TEST(testGetFaceArea);
        CPPUNIT_TEST(testGetSolidVolume);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();
        void cleanUp();

        void testGetIDs();
        void testGetReferences();
        void testGetEdgeVertices();
        void testGetFaceWires();
        void testSolidShells();

        void testGetVertex();
        void testGetEdgeLength();
        void testGetFaceArea();
        void testGetSolidVolume();
    };
}

#endif // __XAO_BREPGEOMETRY_TEST_HXX__
