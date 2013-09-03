#ifndef __XAO_GEOMETRY_TEST_HXX__
#define __XAO_GEOMETRY_TEST_HXX__

#include <cppunit/extensions/HelperMacros.h>

#include "../Xao.hxx"

namespace XAO
{
    class GeometryTest: public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE(GeometryTest);
        CPPUNIT_TEST(testGeometry);
        CPPUNIT_TEST(testGeometryErrors);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();
        void cleanUp();

        void testGeometry();
        void testGeometryErrors();
    };
}

#endif // __XAO_GEOMETRY_TEST_HXX__
