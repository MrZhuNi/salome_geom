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
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();
        void cleanUp();

        void testGetIDs();
    };
}

#endif // __XAO_BREPGEOMETRY_TEST_HXX__
