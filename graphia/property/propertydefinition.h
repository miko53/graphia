/*
    graphia - little tool for building designer diagram
    Copyright (C) 2012  Mickael Sergent

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROPERTYDEFINITION_H
#define PROPERTYDEFINITION_H

#include <QCoreApplication>
#include <QStringList>
#include <QMetaEnum>


class CPropertyDefinition
{
  Q_DECLARE_TR_FUNCTIONS(CPropertyDefinition);

public:
  typedef enum
  {
    NAME,
    POS,
    POS_X,
    POX_Y,
    RADIUS,
    COLOR,
    FONT,
    FONT_COLOR,
    LINE_COLOR,
    BACKGND_COLOR,
    ALIGNMENT_H,
    ALIGNMENT_V,
    OFFSET_X,
    OFFSET_Y,
    OFFSET_Xb,
    HEIGHT,
    WIDTH,
    CLASS,
    FLOW_NAME,
    DISPLAY_LABEL
  } Type;
  
  enum alignHorizontal
  {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_HCENTER,
    ALIGN_JUSTIFY
  };

  enum alignVertical
  {
    ALIGN_TOP,
    ALIGN_VCENTER,
    ALIGN_BOTTOM
  };

  static QStringList m_HorizAlignmentName;
  static QStringList m_VertAlignmentName;

  static enum alignHorizontal intToAlignmentH(int value);
  static int alignementHToInt(enum alignHorizontal value);

  static enum alignVertical intToAlignmentV(int value);
  static int alignementVToInt(enum alignVertical value);

protected:

private:
  
  
};


Q_DECLARE_METATYPE(CPropertyDefinition::alignHorizontal)
Q_DECLARE_METATYPE(CPropertyDefinition::alignVertical)
Q_ENUMS(alignHorizontal);
Q_ENUMS(alignVertical);


#endif // PROPERTYDEFINITION_H
