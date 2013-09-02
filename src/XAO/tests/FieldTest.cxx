#include <vector>
#include <Utils_SALOME_Exception.hxx>

#include "FieldTest.hxx"
#include "../Xao.hxx"
#include "../Field.hxx"
#include "../Step.hxx"
#include "../IntegerStep.hxx"

using namespace XAO;


void FieldTest::setUp()
{
}

void FieldTest::tearDown()
{
}

void FieldTest::cleanUp()
{
}

void FieldTest::testField(XAO::Type type)
{
    Field* f = Field::createField(type, XAO::FACE, 10, 3);

    CPPUNIT_ASSERT_EQUAL(0, (int)f->getName().size());
    CPPUNIT_ASSERT_EQUAL(type, f->getType());
    CPPUNIT_ASSERT_EQUAL(XAO::FACE, f->getDimension());
    CPPUNIT_ASSERT_EQUAL(3, f->countComponents());
    CPPUNIT_ASSERT_EQUAL(10, f->countElements());
    CPPUNIT_ASSERT_EQUAL(30, f->countValues());

    f->setName("field1");
    CPPUNIT_ASSERT_EQUAL(std::string("field1"), f->getName());

    CPPUNIT_ASSERT_EQUAL(0, (int)f->getComponentName(0).size());
    f->setComponentName(0, "x");
    f->setComponentName(1, "y");
    f->setComponentName(2, "z");
    CPPUNIT_ASSERT_EQUAL(3, f->countComponents());
    CPPUNIT_ASSERT_EQUAL(std::string("x"), f->getComponentName(0));
    CPPUNIT_ASSERT_EQUAL(std::string("y"), f->getComponentName(1));
    CPPUNIT_ASSERT_EQUAL(std::string("z"), f->getComponentName(2));
    CPPUNIT_ASSERT_THROW(f->setComponentName(3, "a"), SALOME_Exception);
    CPPUNIT_ASSERT_THROW(f->getComponentName(3), SALOME_Exception);
}

void FieldTest::testBooleanField()
{
    testField(XAO::BOOLEAN);
}
void FieldTest::testIntegerField()
{
    testField(XAO::INTEGER);
}
void FieldTest::testDoubleField()
{
    testField(XAO::DOUBLE);
}
void FieldTest::testStringField()
{
    testField(XAO::STRING);
}

void FieldTest::testStep(XAO::Type type)
{
    Step* step = Step::createStep(type, 0, 0, 5, 3);
    CPPUNIT_ASSERT_EQUAL(type, step->getType());

    CPPUNIT_ASSERT_EQUAL(0, step->getStep());
    step->setStep(10);
    CPPUNIT_ASSERT_EQUAL(10, step->getStep());

    CPPUNIT_ASSERT_EQUAL(0, step->getStamp());
    step->setStamp(100);
    CPPUNIT_ASSERT_EQUAL(100, step->getStamp());

    CPPUNIT_ASSERT_EQUAL(5, step->countElements());
    CPPUNIT_ASSERT_EQUAL(3, step->countComponents());
    CPPUNIT_ASSERT_EQUAL(15, step->countValues());
}

void FieldTest::testBooleanStep()
{
    testStep(XAO::BOOLEAN);
}
void FieldTest::testIntegerStep()
{
    testStep(XAO::INTEGER);
}
void FieldTest::testDoubleStep()
{
    testStep(XAO::DOUBLE);
}
void FieldTest::testStringStep()
{
    testStep(XAO::STRING);
}

void FieldTest::testIntegerStepValues()
{
    int nbComponents = 3;
    int nbElements = 5;

    IntegerStep* istep = (IntegerStep*)Step::createStep(XAO::INTEGER, 0, 0, nbElements, nbComponents);
    for (int i = 0; i < istep->countElements(); ++i)
    {
        for (int j = 0; j < istep->countComponents(); ++j)
            istep->setValue(i, j, i*10 + j);
    }

    CPPUNIT_ASSERT_EQUAL(12, istep->getValue(1, 2));
    CPPUNIT_ASSERT_THROW(istep->getValue(nbElements, 2), SALOME_Exception);
    CPPUNIT_ASSERT_THROW(istep->getValue(1, nbComponents), SALOME_Exception);

    // get all values
    std::vector<int> values;
    values = istep->getValues();
    CPPUNIT_ASSERT_EQUAL(nbElements*nbComponents, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
    {
        for (int j = 0; j < nbComponents; ++j)
            CPPUNIT_ASSERT_EQUAL(10*i+j, values[i*nbComponents+j]);
    }

    // get one element
    values = istep->getElement(2);
    CPPUNIT_ASSERT_THROW(istep->getElement(nbElements), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbComponents, (int)values.size());
    for (int i = 0; i < nbComponents; ++i)
        CPPUNIT_ASSERT_EQUAL(20+i, values[i]);

    // get one component
    values = istep->getComponent(1);
    CPPUNIT_ASSERT_THROW(istep->getComponent(nbComponents), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbElements, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
        CPPUNIT_ASSERT_EQUAL(10*i+1, values[i]);

    // set one element
    std::vector<int> newEltValues;
    newEltValues.push_back(1);
    CPPUNIT_ASSERT_THROW(istep->setElements(2, newEltValues), SALOME_Exception);
    for (int i = 1; i < nbComponents; ++i)
        newEltValues.push_back(1);
    istep->setElements(2, newEltValues);

    // set one component
    std::vector<int> newCompValues;
    newCompValues.push_back(100);
    CPPUNIT_ASSERT_THROW(istep->setComponents(1, newCompValues), SALOME_Exception);
    for (int i = 1; i < nbElements; ++i)
        newCompValues.push_back(100);
    istep->setComponents(1, newCompValues);
}


