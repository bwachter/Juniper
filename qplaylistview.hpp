/*
 * (C) Copyright 2009 Pål Driveklepp
 *
 * Written by: Pål Driveklepp <jalla2000@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef QPLAYLISTVIEW_H
#define QPLAYLISTVIEW_H

#include <QObject>
#include <QTableView>
#include <spotify/api.h>

class QPlayListView : public QTableView
{
    Q_OBJECT
	
 public:
    QPlayListView(QWidget *parent = 0);
 private:
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    
 protected:
    void contextMenuEvent(QContextMenuEvent *event);

};

#endif