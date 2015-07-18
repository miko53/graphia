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
#include "toolbutton.h"


CToolButton::CToolButton(QWidget* parent): QToolButton(parent)
{
  configure();
}

CToolButton::~CToolButton()
{

}

void CToolButton::configure()
{
  connect(this, SIGNAL(clicked(bool)), this, SLOT(OnClickButton()));
  connect(this, SIGNAL(toggled(bool)), this, SLOT(OnToggledButton(bool)));
  m_nbClicks = 0;
  m_currentState = STATE_1;
  setIconSize(QSize(30, 30));
  setCheckable(true);
  setAutoRaise(true);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}


void CToolButton::setIcon(const QIcon& icon, CToolButton::STATE s)
{
  m_icon[s] = icon;
  if (s == STATE_1)
    QToolButton::setIcon(icon);
}


CToolButton::CToolButton(const QIcon& iconState1, const QIcon& iconState2, const QString& text)
{
  setIcon(iconState1, STATE_1);
  setIcon(iconState2, STATE_2);
  //setText(text);
  setStatusTip(text);
  configure();
}


void CToolButton::OnClickButton()
{
  if (isChecked())
  {
    switch (m_nbClicks)
    {
      case 0:
      default:
        m_nbClicks++;
        break;
        
      case 1:
        QToolButton::setIcon(m_icon[STATE_2]);
        m_nbClicks++;
        m_currentState = STATE_2;
        break;
        
      case 2:
        QToolButton::setIcon(m_icon[STATE_1]);
        m_nbClicks = 1;
        m_currentState = STATE_1;
        break;
    }
  }
}


void CToolButton::OnToggledButton(bool b)
{
  if (!b)
  {
    QToolButton::setIcon(m_icon[STATE_1]);
    m_nbClicks = 0;
    m_currentState = STATE_1;
  }
}
