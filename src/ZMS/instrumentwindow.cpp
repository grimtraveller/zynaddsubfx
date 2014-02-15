/*
  ZynAddSubFX - a software synthesizer

  instrumentwindow.cpp - Instrument widget for editing instrument settings
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink

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
#include "instrumentwindow.h"
#include "ui_instrumentwindow.h"
#include "../Misc/Master.h"

InstrumentWindow::InstrumentWindow(int partindex, InstrumentContainer* c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentWindow),
    _container(c),
    _part(Master::getInstance().part[partindex]), _partIndex(partindex)
{
    for (int i = 0; i < NUM_KIT_ITEMS; i++)
        this->_kitWindows[i] = 0;

    ui->setupUi(this);

    this->ui->kits->clear();
    this->UpdateUI();

    this->setStyleSheet("background-color:#666;");
    this->ui->chkActive->setChecked(_part->Penabled);
    this->ui->lblName->setText(QString((const char*)_part->Pname));
    this->ui->cmbKitmode->addItem("No kit");
    this->ui->cmbKitmode->addItem("Multi instrument per note");
    this->ui->cmbKitmode->addItem("Single instrument per note");

    connect(this->ui->collapse, SIGNAL(clicked()), this, SLOT(OnToggleCollapse()));
    connect(this->ui->chkActive, SIGNAL(stateChanged(int)), this, SLOT(OnChangeEnabled(int)));
    connect(this->ui->btnAddItem, SIGNAL(clicked()), this, SLOT(OnAddKititem()));
    connect(this->ui->btnRemoveItem, SIGNAL(clicked()), this, SLOT(OnRemoveKititem()));
    connect(this->ui->cmbKitmode, SIGNAL(currentIndexChanged(int)), this, SLOT(OnKitModeChanged(int)));
}

InstrumentWindow::~InstrumentWindow()
{
    for (int i = 0; i < NUM_KIT_ITEMS; i++)
    {
        if (this->_kitWindows[i] != 0)
        {
            this->ui->kits->removeTab(this->ui->kits->indexOf(this->_kitWindows[i]));
            delete this->_kitWindows[i];
        }
    }
    delete ui;
}

void InstrumentWindow::UpdateUI()
{
    for (int i = 0; i < NUM_KIT_ITEMS; i++)
    {
        if (this->_part->kit[i].Penabled)
        {
            if (this->_kitWindows[i] == 0)
            {
                this->_kitWindows[i] = new KitWindow(this->_part, i);
                this->ui->kits->addTab(this->_kitWindows[i], this->_kitWindows[i]->GetIcon(), "est");
                connect(this->_kitWindows[i], SIGNAL(KeyRangeChanged()), this, SLOT(OnKeyRangeChanged()));
            }
        }
        else
        {
            if (this->_kitWindows[i] != 0)
            {
                int remove = this->ui->kits->layout()->indexOf(this->_kitWindows[i]);
                this->ui->kits->layout()->removeItem(this->ui->kits->layout()->itemAt(remove));
                delete this->_kitWindows[i];
                this->_kitWindows[i] = 0;
            }
        }
    }
}

void InstrumentWindow::OnToggleCollapse()
{
    this->_container->SelectInstrument(this);
    if (this->ui->content->isVisible())
    {
        this->ui->content->hide();
        this->ui->collapse->setText("+");
    }
    else
    {
        this->ui->content->show();
        this->ui->collapse->setText("-");
    }
}

void InstrumentWindow::OnChangeEnabled(int value)
{
    this->_part->Penabled = value;
}

void InstrumentWindow::OnAddKititem()
{
    for (int i = 0; i < NUM_KIT_ITEMS; i++)
    {
        if (this->_part->kit[i].Penabled == false)
        {
            this->_part->setkititemstatus(i, true);

            if (this->_part->Pkitmode == 2 && i > 0)
            {
                int len = (this->_part->kit[i-1].Pmaxkey - this->_part->kit[i-1].Pminkey);
                int half = len / 2;
                this->_part->kit[i-1].Pmaxkey = this->_part->kit[i-1].Pminkey + half;
                this->_part->kit[i].Pminkey = this->_part->kit[i-1].Pminkey + half;
                this->_part->kit[i].Pmaxkey = this->_part->kit[i-1].Pminkey + len;
                if (this->_kitWindows[i-1] != 0)
                    this->_kitWindows[i-1]->UpdateUI();
            }
            break;
        }
    }
    this->_container->SelectInstrument(this);
    this->UpdateUI();
}

void InstrumentWindow::OnRemoveKititem()
{
    for (int i = NUM_KIT_ITEMS - 1; i > 0; i--)
    {
        if (this->_part->kit[i].Penabled)
        {
            this->_part->setkititemstatus(i, 0);

            if (this->_part->Pkitmode == 2)
            {
                this->_part->kit[i-1].Pmaxkey = this->_part->kit[i].Pmaxkey;
                if (this->_kitWindows[i-1] != 0)
                    this->_kitWindows[i-1]->UpdateUI();
            }
            break;
        }
    }
    this->_container->SelectInstrument(this);
    this->UpdateUI();
}

void InstrumentWindow::OnKeyRangeChanged()
{
    for (int i = 0; i < NUM_KIT_ITEMS; i++)
    {
        if (this->_part->kit[i].Penabled == false)
        {
            if (this->_kitWindows[i] != 0)
            {
                this->_kitWindows[i]->UpdateUI();
            }
        }
    }
    this->_container->SelectInstrument(this);
    this->UpdateUI();
}

void InstrumentWindow::OnKitModeChanged(int value)
{
    this->_part->Pkitmode = value;
    if (value == 0)
    {
        this->ui->btnAddItem->setEnabled(false);
        this->ui->btnRemoveItem->setEnabled(false);
    }
    else
    {
        this->ui->btnAddItem->setEnabled(true);
        this->ui->btnRemoveItem->setEnabled(true);
    }
}

void InstrumentWindow::Select()
{
}

void InstrumentWindow::Unselect()
{
}

int InstrumentWindow::PartIndex()
{
    return this->_partIndex;
}
