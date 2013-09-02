#include <vector>
#include <Utils_SALOME_Exception.hxx>

#include "TestUtils.hxx"
#include "GroupTest.hxx"
#include "../Xao.hxx"
#include "../Group.hxx"

using namespace XAO;

void GroupTest::setUp()
{
}

void GroupTest::tearDown()
{
}

void GroupTest::cleanUp()
{
}

void GroupTest::testGroup()
{
    Group* group = new Group(XAO::FACE, 20);

    CPPUNIT_ASSERT_EQUAL(XAO::FACE, group->getDimension());
    CPPUNIT_ASSERT_EQUAL(20, group->getNbElements());

    CPPUNIT_ASSERT_EQUAL(std::string(""), group->getName());
    group->setName("the Group");
    CPPUNIT_ASSERT_EQUAL(std::string("the Group"), group->getName());

    CPPUNIT_ASSERT_EQUAL(0, group->count());
    group->add(10);
    CPPUNIT_ASSERT_EQUAL(1, group->count());
    group->add(12);
    CPPUNIT_ASSERT_EQUAL(2, group->count());
    group->add(12);
    CPPUNIT_ASSERT_EQUAL(2, group->count());

    CPPUNIT_ASSERT_EQUAL(10, group->get(0));
    CPPUNIT_ASSERT_EQUAL(12, group->get(1));
    CPPUNIT_ASSERT_THROW(group->get(2), SALOME_Exception);

    group->remove(15);
    CPPUNIT_ASSERT_EQUAL(2, group->count());
    group->remove(10);
    CPPUNIT_ASSERT_EQUAL(1, group->count());
    CPPUNIT_ASSERT_EQUAL(12, group->get(0));

    delete group;
}







