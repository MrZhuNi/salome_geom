#include <vector>
#include <Utils_SALOME_Exception.hxx>

#include "FieldTest.hxx"
#include "../XAO_Xao.hxx"
#include "../XAO_XaoUtils.hxx"
#include "../XAO_Field.hxx"
#include "../XAO_Step.hxx"
#include "../XAO_BooleanField.hxx"
#include "../XAO_IntegerField.hxx"
#include "../XAO_DoubleField.hxx"
#include "../XAO_StringField.hxx"

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

Field* FieldTest::testField(XAO::Type type)
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

    CPPUNIT_ASSERT_EQUAL(0, f->countSteps());
    Step* step = f->addNewStep(0);
    CPPUNIT_ASSERT_EQUAL(1, f->countSteps());
    step = f->addNewStep(1);
    step = f->addNewStep(2);
    CPPUNIT_ASSERT_EQUAL(3, f->countSteps());
    CPPUNIT_ASSERT_THROW(f->addNewStep(2), SALOME_Exception); // step already exists

    CPPUNIT_ASSERT_EQUAL(true, f->removeStep(step));
    CPPUNIT_ASSERT_EQUAL(2, f->countSteps());
    CPPUNIT_ASSERT_EQUAL(false, f->removeStep(step)); // remove same
    CPPUNIT_ASSERT_EQUAL(2, f->countSteps());

    return f;
}

void FieldTest::testBooleanField()
{
    BooleanField* f = (BooleanField*)testField(XAO::BOOLEAN);

    BooleanStep* step = f->getStep(0);
    CPPUNIT_ASSERT_EQUAL(XAO::BOOLEAN, step->getType());
    CPPUNIT_ASSERT_MESSAGE("step is NULL", step != NULL);
    CPPUNIT_ASSERT_THROW(f->getStep(2), SALOME_Exception);

    step = f->addStep(10);
    CPPUNIT_ASSERT_EQUAL(XAO::BOOLEAN, step->getType());
    CPPUNIT_ASSERT_EQUAL(3, f->countSteps());
    CPPUNIT_ASSERT_THROW(f->addStep(10), SALOME_Exception); // step already exists
}

void FieldTest::testIntegerField()
{
    IntegerField* f = (IntegerField*)testField(XAO::INTEGER);

    IntegerStep* step = f->getStep(0);
    CPPUNIT_ASSERT_EQUAL(XAO::INTEGER, step->getType());
    CPPUNIT_ASSERT_MESSAGE("step is NULL", step != NULL);
    CPPUNIT_ASSERT_THROW(f->getStep(2), SALOME_Exception);

    step = f->addStep(10);
    CPPUNIT_ASSERT_EQUAL(XAO::INTEGER, step->getType());
    CPPUNIT_ASSERT_EQUAL(3, f->countSteps());
    CPPUNIT_ASSERT_THROW(f->addStep(10), SALOME_Exception); // step already exists
}
void FieldTest::testDoubleField()
{
    DoubleField* f = (DoubleField*)testField(XAO::DOUBLE);

    DoubleStep* step = f->getStep(0);
    CPPUNIT_ASSERT_EQUAL(XAO::DOUBLE, step->getType());
    CPPUNIT_ASSERT_MESSAGE("step is NULL", step != NULL);
    CPPUNIT_ASSERT_THROW(f->getStep(2), SALOME_Exception);

    step = f->addStep(10);
    CPPUNIT_ASSERT_EQUAL(XAO::DOUBLE, step->getType());
    CPPUNIT_ASSERT_EQUAL(3, f->countSteps());
    CPPUNIT_ASSERT_THROW(f->addStep(10), SALOME_Exception); // step already exists
}
void FieldTest::testStringField()
{
    StringField* f = (StringField*)testField(XAO::STRING);

    StringStep* step = f->getStep(0);
    CPPUNIT_ASSERT_EQUAL(XAO::STRING, step->getType());
    CPPUNIT_ASSERT_MESSAGE("step is NULL", step != NULL);
    CPPUNIT_ASSERT_THROW(f->getStep(2), SALOME_Exception);

    step = f->addStep(10);
    CPPUNIT_ASSERT_EQUAL(XAO::STRING, step->getType());
    CPPUNIT_ASSERT_EQUAL(3, f->countSteps());
    CPPUNIT_ASSERT_THROW(f->addStep(10), SALOME_Exception); // step already exists
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

void FieldTest::testBooleanStepValues()
{
    int nbComponents = 3; // > 1
    int nbElements = 5;   // > 1

    BooleanStep* step = (BooleanStep*)Step::createStep(XAO::BOOLEAN, 0, 0, nbElements, nbComponents);
    for (int i = 0; i < step->countElements(); ++i)
    {
        for (int j = 0; j < step->countComponents(); ++j)
        {
            step->setValue(i, j, j % 2 == 0);
        }
    }

    CPPUNIT_ASSERT_EQUAL(true, step->getValue(1, 2));
    CPPUNIT_ASSERT_EQUAL(std::string("true"), step->getStringValue(1, 2));
    CPPUNIT_ASSERT_THROW(step->getValue(nbElements, 2), SALOME_Exception);
    CPPUNIT_ASSERT_THROW(step->getValue(1, nbComponents), SALOME_Exception);

    // get all values
    std::vector<bool> values;
    values = step->getValues();
    CPPUNIT_ASSERT_EQUAL(nbElements*nbComponents, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
    {
        for (int j = 0; j < nbComponents; ++j)
            CPPUNIT_ASSERT((j % 2 == 0) == values[i*nbComponents+j]);
    }

    // get one element
    values = step->getElement(2);
    CPPUNIT_ASSERT_THROW(step->getElement(nbElements), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbComponents, (int)values.size());
    for (int i = 0; i < nbComponents; ++i)
        CPPUNIT_ASSERT((i % 2 == 0) == values[i]);

    // get one component
    values = step->getComponent(1);
    CPPUNIT_ASSERT_THROW(step->getComponent(nbComponents), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbElements, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
        CPPUNIT_ASSERT(false == values[i]);

    // set one element
    std::vector<bool> newEltValues;
    // only one value
    newEltValues.push_back(true);
    CPPUNIT_ASSERT_THROW(step->setElements(2, newEltValues), SALOME_Exception);
    // all values
    for (int i = 1; i < nbComponents; ++i)
        newEltValues.push_back(true);
    step->setElements(2, newEltValues);

    // set one component
    std::vector<bool> newCompValues;
    // only one value
    newCompValues.push_back(true);
    CPPUNIT_ASSERT_THROW(step->setComponents(1, newCompValues), SALOME_Exception);
    // all values
    for (int i = 1; i < nbElements; ++i)
        newCompValues.push_back(true);
    step->setComponents(1, newCompValues);

    // set string value
    step->setStringValue(0, 0, "true");
    CPPUNIT_ASSERT_NO_THROW(step->setStringValue(0, 0, "aa")); // set false

    // set all values
    std::vector<bool> allValues;
    // only one value
    allValues.push_back(true);
    CPPUNIT_ASSERT_THROW(step->setValues(allValues), SALOME_Exception);
    // all values
    for (int i = 1; i < nbElements*nbComponents; ++i)
        allValues.push_back(true);
    step->setValues(allValues);
}

void FieldTest::testIntegerStepValues()
{
    int nbComponents = 3;
    int nbElements = 5;

    IntegerStep* step = (IntegerStep*)Step::createStep(XAO::INTEGER, 0, 0, nbElements, nbComponents);
    for (int i = 0; i < step->countElements(); ++i)
    {
        for (int j = 0; j < step->countComponents(); ++j)
            step->setValue(i, j, i*10 + j);
    }

    CPPUNIT_ASSERT_EQUAL(12, step->getValue(1, 2));
    CPPUNIT_ASSERT_THROW(step->getValue(nbElements, 2), SALOME_Exception);
    CPPUNIT_ASSERT_THROW(step->getValue(1, nbComponents), SALOME_Exception);

    // get all values
    std::vector<int> values;
    values = step->getValues();
    CPPUNIT_ASSERT_EQUAL(nbElements*nbComponents, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
    {
        for (int j = 0; j < nbComponents; ++j)
            CPPUNIT_ASSERT_EQUAL(10*i+j, values[i*nbComponents+j]);
    }

    // get one element
    values = step->getElement(2);
    CPPUNIT_ASSERT_THROW(step->getElement(nbElements), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbComponents, (int)values.size());
    for (int i = 0; i < nbComponents; ++i)
        CPPUNIT_ASSERT_EQUAL(20+i, values[i]);

    // get one component
    values = step->getComponent(1);
    CPPUNIT_ASSERT_THROW(step->getComponent(nbComponents), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbElements, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
        CPPUNIT_ASSERT_EQUAL(10*i+1, values[i]);

    // set one element
    std::vector<int> newEltValues;
    newEltValues.push_back(1);
    CPPUNIT_ASSERT_THROW(step->setElements(2, newEltValues), SALOME_Exception);
    for (int i = 1; i < nbComponents; ++i)
        newEltValues.push_back(1);
    step->setElements(2, newEltValues);

    // set one component
    std::vector<int> newCompValues;
    newCompValues.push_back(100);
    CPPUNIT_ASSERT_THROW(step->setComponents(1, newCompValues), SALOME_Exception);
    for (int i = 1; i < nbElements; ++i)
        newCompValues.push_back(100);
    step->setComponents(1, newCompValues);

    // set string value
    step->setStringValue(0, 0, "0");
    CPPUNIT_ASSERT_THROW(step->setStringValue(0, 0, "aa"), SALOME_Exception);

    // set all values
    std::vector<int> allValues;
    // only one value
    allValues.push_back(11);
    CPPUNIT_ASSERT_THROW(step->setValues(allValues), SALOME_Exception);
    // all values
    for (int i = 1; i < nbElements*nbComponents; ++i)
        allValues.push_back(11);
    step->setValues(allValues);
}

void FieldTest::testDoubleStepValues()
{
    int nbComponents = 3;
    int nbElements = 5;

    DoubleStep* step = (DoubleStep*)Step::createStep(XAO::DOUBLE, 0, 0, nbElements, nbComponents);
    for (int i = 0; i < step->countElements(); ++i)
    {
        for (int j = 0; j < step->countComponents(); ++j)
            step->setValue(i, j, i*10 + j*0.1);
    }

    CPPUNIT_ASSERT_EQUAL(10.2, step->getValue(1, 2));
    CPPUNIT_ASSERT_THROW(step->getValue(nbElements, 2), SALOME_Exception);
    CPPUNIT_ASSERT_THROW(step->getValue(1, nbComponents), SALOME_Exception);

    // get all values
    std::vector<double> values;
    values = step->getValues();
    CPPUNIT_ASSERT_EQUAL(nbElements*nbComponents, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
    {
        for (int j = 0; j < nbComponents; ++j)
            CPPUNIT_ASSERT_EQUAL(10*i+j*0.1, values[i*nbComponents+j]);
    }

    // get one element
    values = step->getElement(2);
    CPPUNIT_ASSERT_THROW(step->getElement(nbElements), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbComponents, (int)values.size());
    for (int i = 0; i < nbComponents; ++i)
        CPPUNIT_ASSERT_EQUAL(20+i*0.1, values[i]);

    // get one component
    values = step->getComponent(1);
    CPPUNIT_ASSERT_THROW(step->getComponent(nbComponents), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbElements, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
        CPPUNIT_ASSERT_EQUAL(10*i+0.1, values[i]);

    // set one element
    std::vector<double> newEltValues;
    newEltValues.push_back(1.);
    CPPUNIT_ASSERT_THROW(step->setElements(2, newEltValues), SALOME_Exception);
    for (int i = 1; i < nbComponents; ++i)
        newEltValues.push_back(1.);
    step->setElements(2, newEltValues);

    // set one component
    std::vector<double> newCompValues;
    newCompValues.push_back(100.0);
    CPPUNIT_ASSERT_THROW(step->setComponents(1, newCompValues), SALOME_Exception);
    for (int i = 1; i < nbElements; ++i)
        newCompValues.push_back(100.0);
    step->setComponents(1, newCompValues);

    // set string value
    step->setStringValue(0, 0, "0.2");
    CPPUNIT_ASSERT_THROW(step->setStringValue(0, 0, "aa"), SALOME_Exception);

    std::vector<double> allValues;
    // only one value
    allValues.push_back(1.1);
    CPPUNIT_ASSERT_THROW(step->setValues(allValues), SALOME_Exception);
    // all values
    for (int i = 1; i < nbElements*nbComponents; ++i)
        allValues.push_back(1.1);
    step->setValues(allValues);}

void FieldTest::testStringStepValues()
{
    int nbComponents = 3;
    int nbElements = 5;

    StringStep* step = (StringStep*)Step::createStep(XAO::STRING, 0, 0, nbElements, nbComponents);
    for (int i = 0; i < step->countElements(); ++i)
    {
        for (int j = 0; j < step->countComponents(); ++j)
            step->setValue(i, j, XaoUtils::intToString(i*10 + j));
    }

    CPPUNIT_ASSERT_EQUAL(std::string("12"), step->getValue(1, 2));
    CPPUNIT_ASSERT_THROW(step->getValue(nbElements, 2), SALOME_Exception);
    CPPUNIT_ASSERT_THROW(step->getValue(1, nbComponents), SALOME_Exception);

    // get all values
    std::vector<std::string> values;
    values = step->getValues();
    CPPUNIT_ASSERT_EQUAL(nbElements*nbComponents, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
    {
        for (int j = 0; j < nbComponents; ++j)
            CPPUNIT_ASSERT_EQUAL(XaoUtils::intToString(10*i+j), values[i*nbComponents+j]);
    }

    // get one element
    values = step->getElement(2);
    CPPUNIT_ASSERT_THROW(step->getElement(nbElements), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbComponents, (int)values.size());
    for (int i = 0; i < nbComponents; ++i)
        CPPUNIT_ASSERT_EQUAL(XaoUtils::intToString(20+i), values[i]);

    // get one component
    values = step->getComponent(1);
    CPPUNIT_ASSERT_THROW(step->getComponent(nbComponents), SALOME_Exception);
    CPPUNIT_ASSERT_EQUAL(nbElements, (int)values.size());
    for (int i = 0; i < nbElements; ++i)
        CPPUNIT_ASSERT_EQUAL(XaoUtils::intToString(10*i+1), values[i]);

    // set one element
    std::vector<std::string> newEltValues;
    newEltValues.push_back("1");
    CPPUNIT_ASSERT_THROW(step->setElements(2, newEltValues), SALOME_Exception);
    for (int i = 1; i < nbComponents; ++i)
        newEltValues.push_back("1");
    step->setElements(2, newEltValues);

    // set one component
    std::vector<std::string> newCompValues;
    newCompValues.push_back("100");
    CPPUNIT_ASSERT_THROW(step->setComponents(1, newCompValues), SALOME_Exception);
    for (int i = 1; i < nbElements; ++i)
        newCompValues.push_back("100");
    step->setComponents(1, newCompValues);

    // set string value
    std::cout << "set string value" << std::endl;
    step->setStringValue(0, 0, "0");

    std::vector<std::string> allValues;
    // only one value
    allValues.push_back("abc");
    CPPUNIT_ASSERT_THROW(step->setValues(allValues), SALOME_Exception);
    // all values
    for (int i = 1; i < nbElements*nbComponents; ++i)
        allValues.push_back("abc");
    step->setValues(allValues);}
