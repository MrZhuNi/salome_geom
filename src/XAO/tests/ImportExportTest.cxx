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

    bool res = xao.exportXAO("empty.xao");
    CPPUNIT_ASSERT(res);
}

void ImportExportTest::testExportGeometry()
{
    Xao xao("me", "1.0");
    Geometry* geom = new Geometry();
    geom->setName("mygeom");
    xao.setGeometry(geom);

    // add elements
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

    // groups
    Group* group = xao.addGroup(XAO::SOLID);
    group->setName("boite1");
    group->addElement(0);

    group = xao.addGroup(XAO::FACE);
    group->setName("faces");
    group->addElement(0);
    group->addElement(1);

    bool res = xao.exportXAO("mygeom.xao");
    CPPUNIT_ASSERT(res);

    std::string xml = xao.getXML();
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
    //std::cout << std::endl;
    Xao xao;
    xao.importXAO(getTestFile("test.xao").c_str());
    checkImport(xao);
}

void ImportExportTest::checkImport(Xao& xao)
{
//    std::string zz = "toto";
//    std::cout << "?? " << (zz == "toto" ? "yes" : "no" ) << std::endl;
//
//    std::cout << std::endl << "Author = " << xao.getAuthor()
//              << " : " << (xao.getAuthor() == "me" ? "yes" : "no")  << std::endl;
//    std::cout << " : " << ("me" == xao.getAuthor() ? "yes" : "no")  << std::endl;
//    std::cout << " : " << (xao.getAuthor().compare("me") ? "yes" : "no")  << std::endl;

    CPPUNIT_ASSERT(xao.getAuthor() == "me");
    CPPUNIT_ASSERT(xao.getVersion() == "1.0");

    Geometry* geom = xao.getGeometry();
    CPPUNIT_ASSERT(geom != NULL);
    CPPUNIT_ASSERT(geom->getName() == "mygeom");

    CPPUNIT_ASSERT(geom->countVertices() == 4);
    CPPUNIT_ASSERT(geom->getVertexName(0) == "v1");
    CPPUNIT_ASSERT(geom->getVertexReference(0) == "1");
    CPPUNIT_ASSERT(geom->getVertexName(1) == "");
    CPPUNIT_ASSERT(geom->getVertexReference(1) == "2");
    CPPUNIT_ASSERT(geom->getVertexName(2) == "v3");
    CPPUNIT_ASSERT(geom->getVertexReference(2) == "3");
    CPPUNIT_ASSERT(geom->getVertexName(3) == "");
    CPPUNIT_ASSERT(geom->getVertexReference(3) == "4");

    CPPUNIT_ASSERT(geom->countEdges() == 3);
    CPPUNIT_ASSERT(geom->getEdgeName(0) == "e1");
    CPPUNIT_ASSERT(geom->getEdgeReference(0) == "5");
    CPPUNIT_ASSERT(geom->getEdgeName(1) == "");
    CPPUNIT_ASSERT(geom->getEdgeReference(1) == "6");
    CPPUNIT_ASSERT(geom->getEdgeName(2) == "e3");
    CPPUNIT_ASSERT(geom->getEdgeReference(2) == "7");

    CPPUNIT_ASSERT(geom->countFaces() == 2);
    CPPUNIT_ASSERT(geom->getFaceName(0) == "f1");
    CPPUNIT_ASSERT(geom->getFaceReference(0) == "8");
    CPPUNIT_ASSERT(geom->getFaceName(1) == "");
    CPPUNIT_ASSERT(geom->getFaceReference(1) == "9");

    CPPUNIT_ASSERT(geom->countSolids() == 1);
    CPPUNIT_ASSERT(geom->getSolidName(0) == "s1");
    CPPUNIT_ASSERT(geom->getSolidReference(0) == "10");

    CPPUNIT_ASSERT(xao.countGroups() == 2);
    Group* group = xao.getGroup(0);
    CPPUNIT_ASSERT(group->getCount() == 1);
    CPPUNIT_ASSERT(group->getName() == "boite_1");
    CPPUNIT_ASSERT(group->getDimension() == XAO::SOLID);
    CPPUNIT_ASSERT(group->getElement(0) == 0);
    group = xao.getGroup(1);
    CPPUNIT_ASSERT(group->getCount() == 2);
    CPPUNIT_ASSERT(group->getName() == "");
    CPPUNIT_ASSERT(group->getDimension() == XAO::FACE);
    CPPUNIT_ASSERT(group->getElement(0) == 0);
    CPPUNIT_ASSERT(group->getElement(1) == 1);
}

void ImportExportTest::testImportXaoFromText()
{
    std::ifstream rstr;
    int length;
    rstr.open(getTestFile("test.xao").c_str());
    rstr.seekg(0, rstr.end);        // go to the end
    length = rstr.tellg();          // report location (this is the length)
    rstr.seekg(0, rstr.beg);        // go back to the beginning
    char* txt = new char[length];   // allocate memory for a buffer of appropriate dimension
    rstr.read(txt, length);         // read the whole file into the buffer
    rstr.close();

    Xao xao;
    xao.setXML(txt);
    checkImport(xao);
}
