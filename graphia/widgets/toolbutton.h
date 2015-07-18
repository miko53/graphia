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
#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>


class CToolButton : public QToolButton
{
  Q_OBJECT

  public:
    enum STATE
    {
      STATE_1,
      STATE_2
    };
    
    CToolButton(QWidget* parent = 0);
    CToolButton(const QIcon& iconState1, const QIcon& iconState2, const QString& text);
    virtual ~CToolButton();
    
    void setIcon(const QIcon& icon, STATE s);
    STATE getState()
    {
      return m_currentState;
    }
    

  protected slots:
    void OnClickButton();
    void OnToggledButton(bool b);
    
  protected:
    QIcon m_icon[2];
    int m_nbClicks;
    STATE m_currentState;

  private:
    void configure();
};

#endif // TOOLBUTTON_H

