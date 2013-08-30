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

    CPPUNIT_ASSERT(f->getName().size() == 0);
    CPPUNIT_ASSERT(f->getType() == type);
    CPPUNIT_ASSERT(f->getDimension() == XAO::FACE);
    CPPUNIT_ASSERT(f->countComponents() == 3);
    CPPUNIT_ASSERT(f->countElements() == 10);
    CPPUNIT_ASSERT(f->countValues() == 30);

    f->setName("field1");
    CPPUNIT_ASSERT(f->getName() == "field1");

    CPPUNIT_ASSERT(f->getComponentName(0).size() == 0);
    f->setComponentName(0, "x");
    f->setComponentName(1, "y");
    f->setComponentName(2, "z");
    CPPUNIT_ASSERT(f->countComponents() == 3);
    CPPUNIT_ASSERT(f->getComponentName(0) == "x");
    CPPUNIT_ASSERT(f->getComponentName(1) == "y");
    CPPUNIT_ASSERT(f->getComponentName(2) == "z");
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
    CPPUNIT_ASSERT(step->getType() == type);

    CPPUNIT_ASSERT(step->getStep() == 0);
    step->setStep(10);
    CPPUNIT_ASSERT(step->getStep() == 10);

    CPPUNIT_ASSERT(step->getStamp() == 0);
    step->setStamp(100);
    CPPUNIT_ASSERT(step->getStamp() == 100);

    CPPUNIT_ASSERT(step->countElements() == 5);
    CPPUNIT_ASSERT(step->countComponents() == 3);
    CPPUNIT_ASSERT(step->countValues() == 15);
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

    CPPUNIT_ASSERT(istep->getValue(1, 2) == 12);
    CPPUNIT_ASSERT_THROW(istep->getValue(nbElements, 2), SALOME_Exception);
    CPPUNIT_ASSERT_THROW(istep->getValue(1, nbComponents), SALOME_Exception);

    // get all values
    std::vector<int> values;
    values = istep->getValues();
    CPPUNIT_ASSERT(values.size() == nbElements * nbComponents);
    for (int i = 0; i < nbElements; ++i)
    {
        for (int j = 0; j < nbComponents; ++j)
            CPPUNIT_ASSERT(values[i*nbComponents+j] == 10*i+j);
    }

    // get one element
    values = istep->getElement(2);
    CPPUNIT_ASSERT_THROW(istep->getElement(nbElements), SALOME_Exception);
    CPPUNIT_ASSERT(values.size() == nbComponents);
    for (int i = 0; i < nbComponents; ++i)
        CPPUNIT_ASSERT(values[i] == 20+i);

    // get one component
    values = istep->getComponent(1);
    CPPUNIT_ASSERT_THROW(istep->getComponent(nbComponents), SALOME_Exception);
    CPPUNIT_ASSERT(values.size() == nbElements);
    for (int i = 0; i < nbElements; ++i)
        CPPUNIT_ASSERT(values[i] == 10*i+1);

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


