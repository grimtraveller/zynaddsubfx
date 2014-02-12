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

#define MAX_OCTAVES 12

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    class Octave;
    class Key : public QGraphicsPolygonItem
    {
    public:
        Key(Octave* _parent, unsigned char n);
        virtual ~Key();

        void setOn(bool on);
        void setEnabled(bool enabled);

        Octave* _parent;
        unsigned char _note;
        bool _isSharp;
        bool _isEnabled;

    protected:
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    };

    class Octave : public QGraphicsItemGroup
    {
    public:
        Octave(unsigned char o);
        virtual ~Octave();

        Key* _keys[12];
        unsigned char _octave;
    };

    static int KeyWidth, KeyHeight;

public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();

    void selectCharacter(char c, bool on);
    void setNoteOn(unsigned char note, bool on);
    void setNoteEnabled(unsigned char note, bool enabled);
    void setNoteColor(unsigned char note, const QColor& color);

private:
    Ui::Keyboard *ui;
    bool _selectedNotes[128];
    bool _sendnotes;
    QGraphicsScene* _scene;
    Octave* _octaves[MAX_OCTAVES];

    unsigned char keyToNote(int key);

    static unsigned char _characterNoteMapping[];
};

#endif // KEYBOARD_H