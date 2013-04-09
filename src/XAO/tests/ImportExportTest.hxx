#ifndef __BUILDING_IMPORT_TEST_HXX__
#define __BUILDING_IMPORT_TEST_HXX__

#include <cppunit/extensions/HelperMacros.h>

#include "../Xao.hxx"

namespace XAO
{
    class ImportExportTest: public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE(ImportExportTest);
        CPPUNIT_TEST(testExportNoGeometry);
        CPPUNIT_TEST(testExportGeometry);
        CPPUNIT_TEST(testGeometryError);
        CPPUNIT_TEST(testImportXao);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();
        void cleanUp();

        void testExportNoGeometry();
        void testExportGeometry();
        void testGeometryError();
        void testImportXao();
    };
}

#endif
