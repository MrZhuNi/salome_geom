// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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

// File   : GEOMGUI_ShapeAnnotations.cxx
// Author : Anton POLETAEV, Open CASCADE S.A.S.
//

#include <GEOMGUI_ShapeAnnotations.h>
#include <GEOM_Annotation.hxx>

// OCCT includes
#include <Standard_ProgramError.hxx>
#include <gp_Trsf.hxx>

#include <SalomeApp_Study.h>

// REGEXP pattern for converting array of entries into plain text string.
// The pattern has the following structure:
// ENTRY: { text[string] : visibility[bool] : screen fixed[bool] : position[xyz] : attach[xyz] }
namespace
{
  static const QString PATTERN_ITEM_GROUP = "\\{ (Text=(?::{2,}|.)*:(?!:)Visible=.*:Screen=.*:Position=\\{(.*):(.*):(.*)\\}:Attach=\\{(.*):(.*):(.*)\\}:ShapeIdx=.*:ShapeType=.*) \\}";
  static const QString PATTERN_ITEM = "Text=((?::{2,}|.)*):(?!:)Visible=(\\d{1}):Screen=(\\d{1}):Position=\\{(.*):(.*):(.*)\\}:Attach=\\{(.*):(.*):(.*)\\}:ShapeIdx=(\\d{1,*}):ShapeType=(\\d{1})";
  static QString toPattern (const QString& theText,
                            const bool theIsVisible,
                            const bool theIsFixed,
                            const gp_Pnt& thePosition,
                            const gp_Pnt& theAttach,
                            const int theShapeIndex,
                            const int theShapeType)
  {
    return QString( "{ Text=" ) + theText +
           QString( ":" ) + QString( "Visible=" ) + QString::number( theIsVisible ? 1 : 0 ) +
           QString( ":" ) + QString( "Screen=" ) + QString::number( theIsFixed ? 1 : 0 ) +
           QString( ":" ) + QString( "Position={" ) +
             QString::number( thePosition.X() ) + QString( ":" ) + 
             QString::number( thePosition.Y() ) + QString( ":" ) + 
             QString::number( thePosition.Z() ) + QString( "}" ) + 
           QString( ":" ) + QString( "Attach={" ) +
             QString::number( theAttach.X() ) + QString( ":" ) + 
             QString::number( theAttach.Y() ) + QString( ":" ) + 
             QString::number( theAttach.Z() ) + QString( "}" ) +
           QString( ":" ) + QString( "ShapeIdx=" ) + QString::number( theShapeIndex ) +
           QString( ":" ) + QString( "ShapeType=" ) + QString::number( theShapeType ) +
           QString( " }" );
  }
};

//=================================================================================
// function : ToPresentation
// purpose  :
//=================================================================================
void GEOMGUI_ShapeAnnotations::ShapeAnnotation::ToPresentation(
                                               const Handle(GEOM_Annotation)& theShapeAnnotation,
                                               const gp_Ax3& theLCS ) const
{
  gp_Trsf aToLCS;
  aToLCS.SetTransformation( theLCS, gp_Ax3() );

  //
  TCollection_ExtendedString aText;
  for (int i = 0; i < (int)Text.length(); i++ )
    aText.Insert( i + 1, Text[ i ].unicode() );
  //
  theShapeAnnotation->SetScreenFixed( IsScreenFixed );
  theShapeAnnotation->SetText( aText );
  theShapeAnnotation->SetPosition( Position );
  theShapeAnnotation->SetAttachPoint( Attach.Transformed( aToLCS ) );
}


//=================================================================================
// function : Constructor
// purpose  : 
//=================================================================================
GEOMGUI_ShapeAnnotations::GEOMGUI_ShapeAnnotations()
{
}

//=================================================================================
// function : Copy constructor
// purpose  : 
//=================================================================================
GEOMGUI_ShapeAnnotations::GEOMGUI_ShapeAnnotations( const GEOMGUI_ShapeAnnotations& theOther )
{
  myAnnotations = theOther.myAnnotations;
}

//=================================================================================
// function : Init constructor
// purpose  : 
//=================================================================================
GEOMGUI_ShapeAnnotations::GEOMGUI_ShapeAnnotations( SalomeApp_Study* theStudy, const std::string& theEntry )
{
  LoadFromAttribute( theStudy, theEntry );
}

//=================================================================================
// function : Init constructor
// purpose  : 
//=================================================================================
GEOMGUI_ShapeAnnotations::GEOMGUI_ShapeAnnotations( const QString& theProperty )
{
  QRegExp aRegExpItemGroups( PATTERN_ITEM_GROUP );
  QRegExp aRegExpItem( "^" + PATTERN_ITEM + "$" );
  aRegExpItemGroups.setMinimal( true );
  aRegExpItem.setMinimal( true );

  int aPos = 0;
  while ( ( aPos = aRegExpItemGroups.indexIn( theProperty, aPos ) ) != -1 )
  {
    aPos += aRegExpItemGroups.matchedLength();

    QString aStrItem = aRegExpItemGroups.cap(1);

    if ( aRegExpItem.indexIn( aStrItem ) < 0 )
    {
      continue;
    }

    QString aStrText       = aRegExpItem.cap( 1 );
    QString aStrVisible    = aRegExpItem.cap( 2 );
    QString aStrFixed      = aRegExpItem.cap( 3 );
    QString aStrPosX       = aRegExpItem.cap( 4 );
    QString aStrPosY       = aRegExpItem.cap( 5 );
    QString aStrPosZ       = aRegExpItem.cap( 6 );
    QString aStrAttX       = aRegExpItem.cap( 7 );
    QString aStrAttY       = aRegExpItem.cap( 8 );
    QString aStrAttZ       = aRegExpItem.cap( 9 );
    QString aStrShapeIdx   = aRegExpItem.cap( 10 );
    QString aStrShapeType  = aRegExpItem.cap( 11 );
    aStrText.replace( "::", ":" );

    ShapeAnnotation aEntry;
    aEntry.Text = aStrText;
    aEntry.IsVisible = aStrVisible.toInt() != 0;
    aEntry.IsScreenFixed = aStrFixed.toInt() != 0;
    aEntry.Position.SetX( aStrPosX.toDouble() );
    aEntry.Position.SetY( aStrPosY.toDouble() );
    aEntry.Position.SetZ( aStrPosZ.toDouble() );
    aEntry.Attach.SetX( aStrAttX.toDouble() );
    aEntry.Attach.SetY( aStrAttY.toDouble() );
    aEntry.Attach.SetZ( aStrAttZ.toDouble() );
    aEntry.ShapeIndex = aStrShapeIdx.toInt();
    aEntry.ShapeType = aStrShapeType.toInt();

    myAnnotations.append( aEntry );
  }
}

//=================================================================================
// function : Destructor
// purpose  : 
//=================================================================================
GEOMGUI_ShapeAnnotations::~GEOMGUI_ShapeAnnotations()
{
}

//=================================================================================
// function : operator QVariant()
// purpose  : 
//=================================================================================
GEOMGUI_ShapeAnnotations::operator QVariant() const
{
  QVariant aQVariant;
  aQVariant.setValue( *this );
  return aQVariant;
}

//=================================================================================
// function : operator QString()
// purpose  : 
//=================================================================================
GEOMGUI_ShapeAnnotations::operator QString() const
{
  QStringList anItems;

  for ( int i = 0; i < myAnnotations.size(); ++i )
  {
    const ShapeAnnotation& aEntry = myAnnotations[i];
    //
    anItems.append( toPattern( aEntry.Text,
                               aEntry.IsVisible,
                               aEntry.IsScreenFixed,
                               aEntry.Position,
                               aEntry.Attach,
                               aEntry.ShapeIndex,
                               aEntry.ShapeType ) );
  }

  return anItems.join( ":" );
}

//=================================================================================
// function : operator ==
// purpose  : 
//=================================================================================
bool GEOMGUI_ShapeAnnotations::operator == (const GEOMGUI_ShapeAnnotations& theOther) const
{
  if ( myAnnotations.size() != theOther.myAnnotations.size() )
  {
    return false;
  }

  for ( int i = 0; i < myAnnotations.size(); ++i )
  {
    if ( myAnnotations[i] != theOther.myAnnotations[i] )
    {
      return false;
    }
  }

  return true;
}

//=================================================================================
// function : FromPresentation
// purpose  : 
//=================================================================================
void GEOMGUI_ShapeAnnotations::FromPresentation( const int theIndex,
                                                 const Handle(GEOM_Annotation)& theShapeAnnotation,
                                                 const gp_Ax3& theLCS )
{
  gp_Trsf aFromLCS;
  aFromLCS.SetTransformation( gp_Ax3(), theLCS );
  //
  ShapeAnnotation& aChangeEntry = myAnnotations[theIndex];
  aChangeEntry.IsScreenFixed = theShapeAnnotation->GetIsScreenFixed();
  aChangeEntry.Text          = QString( (QChar*) theShapeAnnotation->GetText().ToExtString(), theShapeAnnotation->GetText().Length() );
  aChangeEntry.Attach        = theShapeAnnotation->GetAttachPoint().Transformed( aFromLCS );
  aChangeEntry.Position      = theShapeAnnotation->GetPosition();
}

//=================================================================================
// function : ToPresentation
// purpose  : 
//=================================================================================
void GEOMGUI_ShapeAnnotations::ToPresentation( const int theIndex,
                                               const Handle(GEOM_Annotation)& theShapeAnnotation,
                                               const gp_Ax3& theLCS )
{
  //gp_Trsf aToLCS;
  //aToLCS.SetTransformation( theLCS, gp_Ax3() );
  //
  const ShapeAnnotation& aEntry = myAnnotations[theIndex];
  aEntry.ToPresentation(theShapeAnnotation, theLCS);
  ////
  //TCollection_ExtendedString aText;
  //for (int i = 0; i < (int)aEntry.Text.length(); i++ )
  //  aText.Insert( i + 1, aEntry.Text[ i ].unicode() );
  ////
  //theShapeAnnotation->SetScreenFixed( aEntry.IsScreenFixed );
  //theShapeAnnotation->SetText( aText );
  //theShapeAnnotation->SetPosition( aEntry.Position );
  //theShapeAnnotation->SetAttachPoint( aEntry.Attach.Transformed( aToLCS ) );
}

bool GEOMGUI_ShapeAnnotations::IsVisible( const int theIndex ) const
{
  return myAnnotations[theIndex].IsVisible;
}

void GEOMGUI_ShapeAnnotations::SetVisible( const int theIndex, const bool theIsVisible )
{
  ShapeAnnotation& aChangeEntry = myAnnotations[theIndex];
  aChangeEntry.IsVisible = theIsVisible;
}

QString GEOMGUI_ShapeAnnotations::GetName( const int theIndex ) const
{
  return "Name";
}

void GEOMGUI_ShapeAnnotations::SetName( const int theIndex, const QString& theName )
{
}

//=================================================================================
// function : LoadFromAttribute
// purpose  : 
//=================================================================================
void GEOMGUI_ShapeAnnotations::LoadFromAttribute( SalomeApp_Study* theStudy, const std::string& theEntry )
{
  Clear();

  _PTR(SObject) aSObj = theStudy->studyDS()->FindObjectID( theEntry );
  if ( !aSObj )
  {
    return;
  }

  _PTR(StudyBuilder) aBuilder = theStudy->studyDS()->NewBuilder();

  _PTR(GenericAttribute) aSeekAtt;
  _PTR(AttributeTableOfReal) aDataAtt;

  if ( !aSObj->FindAttribute( aSeekAtt, "AttributeTableOfReal" ) )
  {
    return;
  }

  aDataAtt = aSeekAtt;

  for ( int i = 1; i <= aDataAtt->GetNbColumns(); ++i )
  {
    std::vector<double> aPropertyArray = aDataAtt->GetColumn( i );

    ShapeAnnotation aEntry;
    aEntry.Text = aDataAtt->GetColumnTitle( i ).c_str();
    aEntry.IsVisible = static_cast<bool>( aPropertyArray[i++] );
    aEntry.IsScreenFixed = static_cast<bool>( aPropertyArray[i++] );
    aEntry.Position.SetX( static_cast<double>( aPropertyArray[i++] ) );
    aEntry.Position.SetY( static_cast<double>( aPropertyArray[i++] ) );
    aEntry.Position.SetZ( static_cast<double>( aPropertyArray[i++] ) );
    aEntry.Attach.SetX( static_cast<double>( aPropertyArray[i++] ) );
    aEntry.Attach.SetY( static_cast<double>( aPropertyArray[i++] ) );
    aEntry.Attach.SetZ( static_cast<double>( aPropertyArray[i++] ) );
    aEntry.ShapeIndex = static_cast<int>( aPropertyArray[i++] );
    aEntry.ShapeType = static_cast<int>( aPropertyArray[i++] );

    myAnnotations.append( aEntry );
  }
}

//=================================================================================
// function : SaveToAttribute
// purpose  : 
//=================================================================================
void GEOMGUI_ShapeAnnotations::SaveToAttribute( SalomeApp_Study *theStudy, const std::string &theEntry )
{
  _PTR(SObject) aSObj = theStudy->studyDS()->FindObjectID( theEntry );
  if ( !aSObj )
  {
    return;
  }

  _PTR(StudyBuilder) aBuilder = theStudy->studyDS()->NewBuilder();

  _PTR(AttributeTableOfReal) aDataAtt;

  aDataAtt = aBuilder->FindOrCreateAttribute( aSObj, "AttributeTableOfReal" );
  aDataAtt->SetNbColumns( 0 );

  for ( int i = 0; i < myAnnotations.size(); ++i )
  {
    const ShapeAnnotation& aEntry = myAnnotations[i];

    std::vector<double> aPropertyArray;
    aPropertyArray.push_back( static_cast<double>(aEntry.IsVisible) );
    aPropertyArray.push_back( static_cast<double>(aEntry.IsScreenFixed) );
    aPropertyArray.push_back( aEntry.Position.X() );
    aPropertyArray.push_back( aEntry.Position.Y() );
    aPropertyArray.push_back( aEntry.Position.Z() );
    aPropertyArray.push_back( aEntry.Attach.X() );
    aPropertyArray.push_back( aEntry.Attach.Y() );
    aPropertyArray.push_back( aEntry.Attach.Z() );
    aPropertyArray.push_back( static_cast<double>( aEntry.ShapeIndex ) );
    aPropertyArray.push_back( static_cast<double>( aEntry.ShapeType ) );

    aDataAtt->AddColumn( aPropertyArray );
    aDataAtt->SetColumnTitle( i + 1, aEntry.Text.toStdString() );
  }
}
