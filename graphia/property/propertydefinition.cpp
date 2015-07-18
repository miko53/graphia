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

#include "propertydefinition.h"


QStringList CPropertyDefinition::m_HorizAlignmentName = QStringList() << tr("Left") << tr("Right")
                                                                      << tr("Center") << tr("Justify");


QStringList CPropertyDefinition::m_VertAlignmentName = QStringList() << tr("Top") << tr("Center")
                                                                     << tr("Bottom") ;


int CPropertyDefinition::alignementHToInt(alignHorizontal value)
{
  int ret;

  switch (value)
  {
    case CPropertyDefinition::ALIGN_LEFT:
      ret = Qt::AlignLeft;
      break;

    case CPropertyDefinition::ALIGN_RIGHT:
      ret = Qt::AlignRight;
      break;

    case CPropertyDefinition::ALIGN_HCENTER:
      ret = Qt::AlignHCenter;
      break;

    case CPropertyDefinition::ALIGN_JUSTIFY:
      ret = Qt::AlignJustify;
      break;

    default:
      Q_ASSERT(false);
      break;
  }

  return ret;
}


CPropertyDefinition::alignHorizontal CPropertyDefinition::intToAlignmentH(int value)
{
  CPropertyDefinition::alignHorizontal ret;

  switch (value)
  {
    case Qt::AlignLeft:
      ret = CPropertyDefinition::ALIGN_LEFT;
      break;

    case Qt::AlignRight:
      ret = CPropertyDefinition::ALIGN_RIGHT;
      break;

    case Qt::AlignHCenter:
      ret = CPropertyDefinition::ALIGN_HCENTER;
      break;

    case Qt::AlignJustify:
      ret = CPropertyDefinition::ALIGN_JUSTIFY;
      break;

    default:
      Q_ASSERT(false);
      break;
  }

  return ret;
}


int CPropertyDefinition::alignementVToInt(alignVertical value)
{
  int ret;

  switch (value)
  {
    case CPropertyDefinition::ALIGN_TOP:
      ret = Qt::AlignTop;
      break;

    case CPropertyDefinition::ALIGN_BOTTOM:
      ret = Qt::AlignBottom;
      break;

    case CPropertyDefinition::ALIGN_VCENTER:
      ret = Qt::AlignVCenter;
      break;

    default:
      Q_ASSERT(false);
      break;
  }

  return ret;
}


CPropertyDefinition::alignVertical CPropertyDefinition::intToAlignmentV(int value)
{
  CPropertyDefinition::alignVertical ret;

  switch (value)
  {
    case Qt::AlignTop:
      ret = CPropertyDefinition::ALIGN_TOP;
      break;

    case Qt::AlignBottom:
      ret = CPropertyDefinition::ALIGN_BOTTOM;
      break;

    case Qt::AlignVCenter:
      ret = CPropertyDefinition::ALIGN_VCENTER;
      break;

    default:
      Q_ASSERT(false);
      break;
  }

  return ret;
}



/*CPropertyDefinition::CPropertyDefinition()
{

}*/


