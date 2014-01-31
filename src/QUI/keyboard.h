/*
  ZynAddSubFX - a software synthesizer

  keyboard.h - Keyboard widget for running notes to the synth
  Copyright (c) 2002-2009 Nasca Octavian Paul
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink, Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QGraphicsPolygonItem>

namespace Ui {
class Keyboard;
}

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    class Octave;
    class Key : public QGraphicsPolygonItem
    {
    public:
        Key(Octave* parent, unsigned char n);
        virtual ~Key();

        Octave* parent;
        unsigned char note;
    };

    class Octave : public QGraphicsItemGroup
    {
    public:
        Octave(unsigned char o);
        virtual ~Octave();

        Key* keys[12];
        unsigned char octave;
    };

    static int KeyWidth;

public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();

    void selectKeyByMouse(int key);
    void selectCharacter(char c, bool on);

private:
    Ui::Keyboard *ui;
    int keywidth, keyheight, octavecount;
    bool selectedNotes[128];
    bool sendnotes;
    QGraphicsScene* scene;
    Octave* octaves[10];

    bool eventFilter(QObject* watched, QEvent* event);

    unsigned char keyToNote(int key);

    unsigned char characterToNote(char c);
    static unsigned char characterNoteMapping[];
};

#endif // KEYBOARD_H
