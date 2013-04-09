
include $(top_srcdir)/adm_local/unix/make_common_starter.am

bin_PROGRAMS= TestXAO

TestXAO_CPPFLAGS= \
    @CPPUNIT_INCLUDES@ @PTHREAD_CFLAGS@  \
    $(GEOM_CXXFLAGS)                     \
    $(CAS_CPPFLAGS)                      \
    $(LIBXML_INCLUDES)                   \
    -I$(srcdir)

TestXAO_LDFLAGS = \
    @CPPUNIT_LIBS@  \
    $(CAS_LDPATH)   \
    $(LIBXML_LIBS)  \
    $(GEOM_LDFLAGS) \
    -lxml2 \
    ../../GEOMBase/libGEOMBase.la \
    ../libXAO.la

dist_TestXAO_SOURCES = \
    ImportExportTest.cxx \
    XAOTests.cxx

UNIT_TEST_PROG = TestXAO

check : tests

CLEANFILES = \
        UnitTestsResult

