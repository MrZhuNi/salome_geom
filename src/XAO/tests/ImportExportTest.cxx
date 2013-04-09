// Copyright (C) 2013  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author : Frederic Pons (OpenCascade)

#include <stdio.h>
#include <Utils_SALOME_Exception.hxx>

#include "ImportExportTest.hxx"
#include "../Geometry.hxx"
#include "../Group.hxx"

using namespace XAO;

std::string getTestFile(std::string fileName)
{
    std::string dataDir = getenv("GEOM_SRC_DIR");
    dataDir += "/src/XAO/tests/data/" + fileName;
    return dataDir;
}

void ImportExportTest::setUp()
{
}

void ImportExportTest::tearDown()
{
}

void ImportExportTest::cleanUp()
{
}

void ImportExportTest::testExportNoGeometry()
{
    Xao xao("me", "1.0");

    bool res = xao.exportToFile("empty.xao");
    CPPUNIT_ASSERT(res);
}

void ImportExportTest::testExportGeometry()
{
    Xao xao("me", "1.0");
    Geometry* geom = new Geometry();
    geom->setName("mygeom");
    xao.setGeometry(geom);

    geom->setCountVertices(4);
    geom->setVertex(0, "v1", "1");
    geom->setVertex(1, "", "2");
    geom->setVertex(2, "v3", "3");
    geom->setVertex(3, "", "4");

    geom->setCountEdges(3);
    geom->setEdge(0, "e1", "5");
    geom->setEdge(1, "", "6");
    geom->setEdge(2, "e3", "7");

    geom->setCountFaces(2);
    geom->setFace(0, "f1", "8");
    geom->setFace(1, "", "9");

    geom->setCountSolids(1);
    geom->setSolid(0, "s1", "10");

    Group* group = new Group();
    xao.addGroup(group);
    group->setName("boite1");
    group->setDimension(3);
    group->addElement("1");

    group = new Group();
    xao.addGroup(group);
    group->setName("faces");
    group->setDimension(2);
    group->addElement("5");
    group->addElement("8");
    group->addElement("9");

    bool res = xao.exportToFile("mygeom.xao");
    CPPUNIT_ASSERT(res);

    const char* xml = xao.getXML();
    //CPPUNIT_ASSERT(strlen(xml) == 1007);
}

void ImportExportTest::testGeometryError()
{
    Xao xao("me", "1.0");
    Geometry* geom = new Geometry();
    geom->setName("mygeom");
    xao.setGeometry(geom);

    geom->setCountVertices(2);
    CPPUNIT_ASSERT_THROW(geom->setVertex(3, "v4", "4"), SALOME_Exception);
}

void ImportExportTest::testImportXao()
{
    std::cout << std::endl;
    Xao xao;
    xao.importFromFile(getTestFile("test.xao").c_str());
    CPPUNIT_ASSERT(strcmp(xao.getAuthor(), "me") == 0);
    CPPUNIT_ASSERT(strcmp(xao.getVersion(), "1.0") == 0);

    Geometry* geom = xao.getGeometry();
    CPPUNIT_ASSERT(geom != NULL);
    CPPUNIT_ASSERT(strcmp(geom->getName(), "mygeom") == 0);

    CPPUNIT_ASSERT(geom->countVertices() == 4);
    CPPUNIT_ASSERT(strcmp(geom->getVertexName(0), "v1") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getVertexReference(0), "1") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getVertexName(1), "") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getVertexReference(1), "2") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getVertexName(2), "v3") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getVertexReference(2), "3") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getVertexName(3), "") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getVertexReference(3), "4") == 0);

    CPPUNIT_ASSERT(geom->countEdges() == 3);
    CPPUNIT_ASSERT(strcmp(geom->getEdgeName(0), "e1") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getEdgeReference(0), "5") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getEdgeName(1), "") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getEdgeReference(1), "6") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getEdgeName(2), "e3") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getEdgeReference(2), "7") == 0);

    CPPUNIT_ASSERT(geom->countFaces() == 2);
    CPPUNIT_ASSERT(strcmp(geom->getFaceName(0), "f1") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getFaceReference(0), "8") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getFaceName(1), "") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getFaceReference(1), "9") == 0);

    CPPUNIT_ASSERT(geom->countSolids() == 1);
    CPPUNIT_ASSERT(strcmp(geom->getSolidName(0), "s1") == 0);
    CPPUNIT_ASSERT(strcmp(geom->getSolidReference(0), "10") == 0);

    CPPUNIT_ASSERT(xao.countGroups() == 2);
    Group* group = xao.getGroup(0);
    CPPUNIT_ASSERT(group->getCount() == 1);
    CPPUNIT_ASSERT(strcmp(group->getName(), "boite_1") == 0);
    CPPUNIT_ASSERT(group->getDimension() == 3);
    CPPUNIT_ASSERT(strcmp(group->getElement(0), "1") == 0);
    group = xao.getGroup(1);
    CPPUNIT_ASSERT(group->getCount() == 3);
    CPPUNIT_ASSERT(strcmp(group->getName(), "") == 0);
    CPPUNIT_ASSERT(group->getDimension() == 2);
    CPPUNIT_ASSERT(strcmp(group->getElement(0), "5") == 0);
    CPPUNIT_ASSERT(strcmp(group->getElement(1), "8") == 0);
    CPPUNIT_ASSERT(strcmp(group->getElement(2), "9") == 0);
}
