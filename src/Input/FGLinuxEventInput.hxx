// FGEventInput.hxx -- handle event driven input devices for the Linux O/S
//
// Written by Torsten Dreyer, started July 2009
//
// Copyright (C) 2009 Torsten Dreyer, Torsten (at) t3r _dot_ de
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// $Id$

#ifndef __FGLINUXEVENTINPUT_HXX
#define __FGLINUXEVENTINPUT_HXX

#include "FGEventInput.hxx"
#include <linux/input.h>
#include <hal/libhal.h>

struct FGLinuxEventData : public FGEventData {
  FGLinuxEventData( struct input_event & event, double dt, int modifiers ) :
    FGEventData( (double)event.value, dt, modifiers ),
    type(event.type),
    code(event.code) {
  }
  unsigned type;
  unsigned code;
};

/*
 * A implementation for linux event devices
 */
class FGLinuxInputDevice : public FGInputDevice {
public:
  FGLinuxInputDevice();
  FGLinuxInputDevice( string name, string devname );
  virtual ~FGLinuxInputDevice();

  virtual void Open();
  virtual void Close();
  virtual void Send( const char * eventName, double value );
  virtual const char * TranslateEventName( FGEventData & eventData );

  void SetDevname( const string name );
  string GetDevname() const { return devname; }

  int GetFd() { return fd; }

private:
  string devname;
  int fd;
};

class FGLinuxEventInput : public FGEventInput {
public:
  FGLinuxEventInput();
  virtual ~ FGLinuxEventInput();
  virtual void update (double dt);
  virtual void postinit();

  void AddHalDevice( const char * udi );
protected:
  LibHalContext *halcontext;
};

#endif