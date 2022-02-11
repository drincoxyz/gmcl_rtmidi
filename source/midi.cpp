/*                           Modified BSD License

                      Copyright (C) 2022 Brandon Little

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <map>
#include <queue>
#include <string>
#include <vector>
#include "RtMidi.h"
#include "GarrysMod/Lua/Interface.h"

using namespace std;
using namespace GarrysMod::Lua;

RtMidiIn*  midiIn;
RtMidiOut* midiOut;

queue<vector<unsigned char>> inputQueue;

map<unsigned char, vector<const char*>> noteName
{
	{ 0,   { "MIDI_NOTE_C_1" } },
	{ 1,   { "MIDI_NOTE_Cs_1", "MIDI_NOTE_Db_1" } },
	{ 2,   { "MIDI_NOTE_D_1" } },
	{ 3,   { "MIDI_NOTE_Ds_1", "MIDI_NOTE_Eb_1" } },
	{ 4,   { "MIDI_NOTE_E_1" } },
	{ 5,   { "MIDI_NOTE_F_1" } },
	{ 6,   { "MIDI_NOTE_Fs_1", "MIDI_NOTE_Gb_1" } },
	{ 7,   { "MIDI_NOTE_G_1" } },
	{ 8,   { "MIDI_NOTE_Gs_1", "MIDI_NOTE_Ab_1" } },
	{ 9,   { "MIDI_NOTE_A_1" } },
	{ 10,  { "MIDI_NOTE_As_1", "MIDI_NOTE_Bb_1" } },
	{ 11,  { "MIDI_NOTE_B_1" } },
	{ 12,  { "MIDI_NOTE_C0" } },
	{ 13,  { "MIDI_NOTE_Cs0", "MIDI_NOTE_Db0" } },
	{ 14,  { "MIDI_NOTE_D0" } },
	{ 15,  { "MIDI_NOTE_Ds0", "MIDI_NOTE_Eb0" } },
	{ 16,  { "MIDI_NOTE_E0" } },
	{ 17,  { "MIDI_NOTE_F0" } },
	{ 18,  { "MIDI_NOTE_Fs0", "MIDI_NOTE_Gb0" } },
	{ 19,  { "MIDI_NOTE_G0" } },
	{ 20,  { "MIDI_NOTE_Gs0", "MIDI_NOTE_Ab0" } },
	{ 21,  { "MIDI_NOTE_A0" } },
	{ 22,  { "MIDI_NOTE_As0", "MIDI_NOTE_Bb0" } },
	{ 23,  { "MIDI_NOTE_B0" } },
	{ 24,  { "MIDI_NOTE_C1" } },
	{ 25,  { "MIDI_NOTE_Cs1", "MIDI_NOTE_Db1" } },
	{ 26,  { "MIDI_NOTE_D1" } },
	{ 27,  { "MIDI_NOTE_Ds1", "MIDI_NOTE_Eb1" } },
	{ 28,  { "MIDI_NOTE_E1" } },
	{ 29,  { "MIDI_NOTE_F1" } },
	{ 30,  { "MIDI_NOTE_Fs1", "MIDI_NOTE_Gb1" } },
	{ 31,  { "MIDI_NOTE_G1" } },
	{ 32,  { "MIDI_NOTE_Gs1", "MIDI_NOTE_Ab1" } },
	{ 33,  { "MIDI_NOTE_A1" } },
	{ 34,  { "MIDI_NOTE_As1", "MIDI_NOTE_Bb1" } },
	{ 35,  { "MIDI_NOTE_B1" } },
	{ 36,  { "MIDI_NOTE_C2" } },
	{ 37,  { "MIDI_NOTE_Cs2", "MIDI_NOTE_Db2" } },
	{ 38,  { "MIDI_NOTE_D2" } },
	{ 39,  { "MIDI_NOTE_Ds2", "MIDI_NOTE_Eb2" } },
	{ 40,  { "MIDI_NOTE_E2" } },
	{ 41,  { "MIDI_NOTE_F2" } },
	{ 42,  { "MIDI_NOTE_Fs2", "MIDI_NOTE_Gb2" } },
	{ 43,  { "MIDI_NOTE_G2" } },
	{ 44,  { "MIDI_NOTE_Gs2", "MIDI_NOTE_Ab2" } },
	{ 45,  { "MIDI_NOTE_A2" } },
	{ 46,  { "MIDI_NOTE_As2", "MIDI_NOTE_Bb2" } },
	{ 47,  { "MIDI_NOTE_B2" } },
	{ 48,  { "MIDI_NOTE_C3" } },
	{ 49,  { "MIDI_NOTE_Cs3", "MIDI_NOTE_Db3" } },
	{ 50,  { "MIDI_NOTE_D3" } },
	{ 51,  { "MIDI_NOTE_Ds3", "MIDI_NOTE_Eb3" } },
	{ 52,  { "MIDI_NOTE_E3" } },
	{ 53,  { "MIDI_NOTE_F3" } },
	{ 54,  { "MIDI_NOTE_Fs3", "MIDI_NOTE_Gb3" } },
	{ 55,  { "MIDI_NOTE_G3" } },
	{ 56,  { "MIDI_NOTE_Gs3", "MIDI_NOTE_Ab3" } },
	{ 57,  { "MIDI_NOTE_A3" } },
	{ 58,  { "MIDI_NOTE_As3", "MIDI_NOTE_Bb3" } },
	{ 59,  { "MIDI_NOTE_B3" } },
	{ 60,  { "MIDI_NOTE_C4" } },
	{ 61,  { "MIDI_NOTE_Cs4", "MIDI_NOTE_Db4" } },
	{ 62,  { "MIDI_NOTE_D4" } },
	{ 63,  { "MIDI_NOTE_Ds4", "MIDI_NOTE_Eb4" } },
	{ 64,  { "MIDI_NOTE_E4" } },
	{ 65,  { "MIDI_NOTE_F4" } },
	{ 66,  { "MIDI_NOTE_Fs4", "MIDI_NOTE_Gb4" } },
	{ 67,  { "MIDI_NOTE_G4" } },
	{ 68,  { "MIDI_NOTE_Gs4", "MIDI_NOTE_Ab4" } },
	{ 69,  { "MIDI_NOTE_A4" } },
	{ 70,  { "MIDI_NOTE_As4", "MIDI_NOTE_Bb4" } },
	{ 71,  { "MIDI_NOTE_B4" } },
	{ 72,  { "MIDI_NOTE_C5" } },
	{ 73,  { "MIDI_NOTE_Cs5", "MIDI_NOTE_Db5" } },
	{ 74,  { "MIDI_NOTE_D5" } },
	{ 75,  { "MIDI_NOTE_Ds5", "MIDI_NOTE_Eb5" } },
	{ 76,  { "MIDI_NOTE_E5" } },
	{ 77,  { "MIDI_NOTE_F5" } },
	{ 78,  { "MIDI_NOTE_Fs5", "MIDI_NOTE_Gb5" } },
	{ 79,  { "MIDI_NOTE_G5" } },
	{ 80,  { "MIDI_NOTE_Gs5", "MIDI_NOTE_Ab5" } },
	{ 81,  { "MIDI_NOTE_A5" } },
	{ 82,  { "MIDI_NOTE_As5", "MIDI_NOTE_Bb5" } },
	{ 83,  { "MIDI_NOTE_B5" } },
	{ 84,  { "MIDI_NOTE_C6" } },
	{ 85,  { "MIDI_NOTE_Cs6", "MIDI_NOTE_Db6" } },
	{ 86,  { "MIDI_NOTE_D6" } },
	{ 87,  { "MIDI_NOTE_Ds6", "MIDI_NOTE_Eb6" } },
	{ 88,  { "MIDI_NOTE_E6" } },
	{ 89,  { "MIDI_NOTE_F6" } },
	{ 90,  { "MIDI_NOTE_Fs6", "MIDI_NOTE_Gb6" } },
	{ 91,  { "MIDI_NOTE_G6" } },
	{ 92,  { "MIDI_NOTE_Gs6", "MIDI_NOTE_Ab6" } },
	{ 93,  { "MIDI_NOTE_A6" } },
	{ 94,  { "MIDI_NOTE_As6", "MIDI_NOTE_Bb6" } },
	{ 95,  { "MIDI_NOTE_B6" } },
	{ 96,  { "MIDI_NOTE_C7" } },
	{ 97,  { "MIDI_NOTE_Cs7", "MIDI_NOTE_Db7" } },
	{ 98,  { "MIDI_NOTE_D7" } },
	{ 99,  { "MIDI_NOTE_Ds7", "MIDI_NOTE_Eb7" } },
	{ 100, { "MIDI_NOTE_E7" } },
	{ 101, { "MIDI_NOTE_F7" } },
	{ 102, { "MIDI_NOTE_Fs7", "MIDI_NOTE_Gb7" } },
	{ 103, { "MIDI_NOTE_G7" } },
	{ 104, { "MIDI_NOTE_Gs7", "MIDI_NOTE_Ab7" } },
	{ 105, { "MIDI_NOTE_A7" } },
	{ 106, { "MIDI_NOTE_As7", "MIDI_NOTE_Bb7" } },
	{ 107, { "MIDI_NOTE_B7" } },
	{ 108, { "MIDI_NOTE_C8" } },
	{ 109, { "MIDI_NOTE_Cs8", "MIDI_NOTE_Db8" } },
	{ 110, { "MIDI_NOTE_D8" } },
	{ 111, { "MIDI_NOTE_Ds8", "MIDI_NOTE_Eb8" } },
	{ 112, { "MIDI_NOTE_E8" } },
	{ 113, { "MIDI_NOTE_F8" } },
	{ 114, { "MIDI_NOTE_Fs8", "MIDI_NOTE_Gb8" } },
	{ 115, { "MIDI_NOTE_G8" } },
	{ 116, { "MIDI_NOTE_Gs8", "MIDI_NOTE_Ab8" } },
	{ 117, { "MIDI_NOTE_A8" } },
	{ 118, { "MIDI_NOTE_As8", "MIDI_NOTE_Bb8" } },
	{ 119, { "MIDI_NOTE_B8" } },
	{ 120, { "MIDI_NOTE_C9" } },
	{ 121, { "MIDI_NOTE_Cs9", "MIDI_NOTE_Db9" } },
	{ 122, { "MIDI_NOTE_D9" } },
	{ 123, { "MIDI_NOTE_Ds9", "MIDI_NOTE_Eb9" } },
	{ 124, { "MIDI_NOTE_E9" } },
	{ 125, { "MIDI_NOTE_F9" } },
	{ 126, { "MIDI_NOTE_Fs9", "MIDI_NOTE_Gb9" } },
	{ 127, { "MIDI_NOTE_G9" } },
};

map<unsigned char, vector<const char*>> messageName
{
	{ 0x80, { "MIDI_MESSAGE_NOTE_OFF" } },
	{ 0x90, { "MIDI_MESSAGE_NOTE_ON" } },
	{ 0xA0, { "MIDI_MESSAGE_KEY_PRESSURE" } },
	{ 0xB0, { "MIDI_MESSAGE_CONTROLLER_CHANGE" } },
	{ 0xC0, { "MIDI_MESSAGE_PROGRAM_CHANGE" } },
	{ 0xD0, { "MIDI_MESSAGE_CHANNEL_PRESSURE" } },
	{ 0xE0, { "MIDI_MESSAGE_PITCH_BEND" } },
	{ 0xF0, { "MIDI_MESSAGE_SYSTEM_EXCLUSIVE" } },
	{ 0xF2, { "MIDI_MESSAGE_SONG_POSITION" } },
	{ 0xF3, { "MIDI_MESSAGE_SONG_SELECT" } },
	{ 0xF6, { "MIDI_MESSAGE_TUNE_REQUEST" } },
	{ 0xF8, { "MIDI_MESSAGE_TIMING_TICK" } },
	{ 0xFA, { "MIDI_MESSAGE_SONG_START" } },
	{ 0xFB, { "MIDI_MESSAGE_SONG_CONTINUE" } },
	{ 0xFC, { "MIDI_MESSAGE_SONG_STOP" } },
	{ 0xFE, { "MIDI_MESSAGE_ACTIVE_SENSE" } },
	{ 0xFF, { "MIDI_MESSAGE_SYSTEM_RESET" } },
};

map<unsigned char, vector<const char*>> controllerName
{
	{ 1,   { "MIDI_CONTROLLER_MODULATION" } },
	{ 2,   { "MIDI_CONTROLLER_BREATH" } },
	{ 4,   { "MIDI_CONTROLLER_FOOT" } },
	{ 5,   { "MIDI_CONTROLLER_PORTAMENTO_TIME" } },
	{ 6,   { "MIDI_CONTROLLER_DATA_ENTRY" } },
	{ 7,   { "MIDI_CONTROLLER_VOLUME" } },
	{ 8,   { "MIDI_CONTROLLER_BALANCE" } },
	{ 10,  { "MIDI_CONTROLLER_PAN" } },
	{ 11,  { "MIDI_CONTROLLER_EXPRESSION" } },
	{ 16,  { "MIDI_CONTROLLER_GENERAL_1" } },
	{ 17,  { "MIDI_CONTROLLER_GENERAL_2" } },
	{ 18,  { "MIDI_CONTROLLER_GENERAL_3" } },
	{ 19,  { "MIDI_CONTROLLER_GENERAL_4" } },
	{ 64,  { "MIDI_CONTROLLER_SUSTAIN" } },
	{ 65,  { "MIDI_CONTROLLER_PORTAMENTO" } },
	{ 66,  { "MIDI_CONTROLLER_SOSTENUTO" } },
	{ 67,  { "MIDI_CONTROLLER_SOFT" } },
	{ 69,  { "MIDI_CONTROLLER_HOLD_2" } },
	{ 80,  { "MIDI_CONTROLLER_GENERAL_5" } },
	{ 81,  { "MIDI_CONTROLLER_GENERAL_6" } },
	{ 82,  { "MIDI_CONTROLLER_GENERAL_7" } },
	{ 83,  { "MIDI_CONTROLLER_GENERAL_8" } },
	{ 92,  { "MIDI_CONTROLLER_TREMOLO_DEPTH" } },
	{ 93,  { "MIDI_CONTROLLER_CHORUS_DEPTH" } },
	{ 94,  { "MIDI_CONTROLLER_CELESTE_DEPTH" } },
	{ 95,  { "MIDI_CONTROLLER_PHASER_DEPTH" } },
	{ 96,  { "MIDI_CONTROLLER_DATA_INCREMENT" } },
	{ 97,  { "MIDI_CONTROLLER_DATA_DECREMENT" } },
	{ 99,  { "MIDI_CONTROLLER_NRPN" } },
	{ 101, { "MIDI_CONTROLLER_RPN" } },
};

void inputCallback(double deltaTime, vector<unsigned char>* message, void* userData)
{
	if (!message->empty())
	{
		inputQueue.push(*message);
	}
}

int openPort(ILuaBase* LUA, RtMidi* midi, const char* callback)
{
	unsigned int port = (unsigned int)LUA->CheckNumber(1);

	try
	{
		midi->openPort(port);
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString(callback);
				LUA->PushNumber((double)port);
			LUA->Call(2, 0);
		LUA->Pop();
	LUA->Pop();

	return 0;
}

int closePort(ILuaBase* LUA, RtMidi* midi, const char* callback)
{
	try
	{
		midi->closePort();
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString(callback);
			LUA->Call(1, 0);
		LUA->Pop();
	LUA->Pop();

	return 0;
}

int getPortName(ILuaBase* LUA, RtMidi* midi)
{
	unsigned int port = (unsigned int)LUA->CheckNumber(1);

	string name;

	try
	{
		name = midi->getPortName(port);
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushString(name.c_str());

	return 1;
}

int getPortCount(ILuaBase* LUA, RtMidi* midi)
{
	unsigned int count = 0;

	try
	{
		count = midi->getPortCount();
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushNumber((double)count);

	return 1;
}

int getEnumName(ILuaBase* LUA, map<unsigned char, vector<const char*>> map)
{
	unsigned char value = (unsigned char)LUA->CheckNumber(1);

	if (map.count(value) > 0)
	{
		const auto& name = map.at(value);

		for (const char* name : name)
		{
			LUA->PushString(name);
		}

		return (int)name.size();
	}

	return 0;
}

LUA_FUNCTION(MidiInputThink)
{
	while (!inputQueue.empty())
	{
		vector<unsigned char> message = inputQueue.front();

		inputQueue.pop();

		int size = (int)message.size();

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString("OnMidiInputMessage");

					vector<unsigned char>::iterator iterator = message.begin();

					while (iterator != message.end())
					{
						unsigned char number = message.front();

						iterator = message.erase(iterator);

						LUA->PushNumber((double)number);
					}
				LUA->Call(size + 1, 0);
			LUA->Pop();
		LUA->Pop();
	}

	return 0;
}

LUA_FUNCTION(OpenInputPort)
{
	return openPort(LUA, midiIn, "OnMidiInputPortOpened");
}

LUA_FUNCTION(OpenOutputPort)
{
	return openPort(LUA, midiOut, "OnMidiOutputPortOpened");
}

LUA_FUNCTION(CloseInputPort)
{
	return closePort(LUA, midiIn, "OnMidiInputPortClosed");
}

LUA_FUNCTION(CloseOutputPort)
{
	return closePort(LUA, midiOut, "OnMidiOutputPortClosed");
}

LUA_FUNCTION(GetInputPortName)
{
	return getPortName(LUA, midiIn);
}

LUA_FUNCTION(GetOutputPortName)
{
	return getPortName(LUA, midiOut);
}

LUA_FUNCTION(GetInputPortCount)
{
	return getPortCount(LUA, midiIn);
}

LUA_FUNCTION(GetOutputPortCount)
{
	return getPortCount(LUA, midiOut);
}

LUA_FUNCTION(GetNoteName)
{
	return getEnumName(LUA, noteName);
}

LUA_FUNCTION(GetMessageName)
{
	unsigned char message = (unsigned char)LUA->CheckNumber(1);

	if (message >= 0x80 && message < 0xF0)
	{
		message = 0x80 + ((unsigned char)floor((message - 0x80) / 16) * 16);

		LUA->Remove(1);
		LUA->PushNumber((double)message);
		LUA->Insert(1);
	}

	return getEnumName(LUA, messageName);
}

LUA_FUNCTION(GetControllerName)
{
	unsigned char controller = (unsigned char)LUA->CheckNumber(1);

	if (controller >= 32 && controller <= 63)
	{
		controller -= 32;

		LUA->Remove(1);
		LUA->PushNumber((double)controller);
		LUA->Insert(1);
	}
	else if (controller == 98 || controller == 100)
	{
		controller += 1;

		LUA->Remove(1);
		LUA->PushNumber((double)controller);
		LUA->Insert(1);
	}

	return getEnumName(LUA, controllerName);
}

LUA_FUNCTION(SendOutputMessage)
{
	vector<unsigned char> message;

	while (LUA->Top() > 0)
	{
		unsigned char number = (unsigned char)LUA->CheckNumber(1);

		LUA->Remove(1);

		message.push_back(number);
	}

	if (!message.empty())
	{
		try
		{
			midiOut->sendMessage(&message);
		}
		catch (RtMidiError& error)
		{
			LUA->ThrowError(error.getMessage().c_str());
		}

		int size = (int)message.size();

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString("OnMidiOutputMessage");

					vector<unsigned char>::iterator iterator = message.begin();

					while (iterator != message.end())
					{
						unsigned char number = message.front();

						iterator = message.erase(iterator);

						LUA->PushNumber((double)number);
					}
				LUA->Call(size + 1, 0);
			LUA->Pop();
		LUA->Pop();
	}

	return 0;
}

LUA_FUNCTION(GetMessageChannel)
{
	unsigned char message = (unsigned char)LUA->CheckNumber(1);

	if (message >= 0x80 && message < 0xF0)
	{
		LUA->PushNumber((message % 16) + 1);

		return 1;
	}

	return 0;
}

LUA_FUNCTION(IsControllerMSB)
{
	unsigned char controller = (unsigned char)LUA->CheckNumber(1);

	LUA->PushBool((controller < 32 || controller > 63) && controller != 98 && controller != 100);

	return 1;
}

GMOD_MODULE_OPEN()
{
	midiIn  = new RtMidiIn();
	midiIn->setCallback(&inputCallback);
	midiOut = new RtMidiOut();

	LUA->PushSpecial(SPECIAL_GLOB);
		for (const auto& pair : noteName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber((double)pair.first);
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : messageName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber((double)pair.first);
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : controllerName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber((double)pair.first);
				LUA->SetField(-2, name);
			}
		}

		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Add");
				LUA->PushString("Think");
				LUA->PushString("MidiInputThink");
				LUA->PushCFunction(MidiInputThink);
			LUA->Call(3, 0);
		LUA->Pop();

		LUA->CreateTable();
			LUA->PushCFunction(OpenInputPort);
			LUA->SetField(-2, "OpenInputPort");
			LUA->PushCFunction(OpenOutputPort);
			LUA->SetField(-2, "OpenOutputPort");
			LUA->PushCFunction(CloseInputPort);
			LUA->SetField(-2, "CloseInputPort");
			LUA->PushCFunction(CloseOutputPort);
			LUA->SetField(-2, "CloseOutputPort");
			LUA->PushCFunction(GetInputPortName);
			LUA->SetField(-2, "GetInputPortName");
			LUA->PushCFunction(GetOutputPortName);
			LUA->SetField(-2, "GetOutputPortName");
			LUA->PushCFunction(GetInputPortCount);
			LUA->SetField(-2, "GetInputPortCount");
			LUA->PushCFunction(GetOutputPortCount);
			LUA->SetField(-2, "GetOutputPortCount");
			LUA->PushCFunction(GetNoteName);
			LUA->SetField(-2, "GetNoteName");
			LUA->PushCFunction(GetMessageName);
			LUA->SetField(-2, "GetMessageName");
			LUA->PushCFunction(GetControllerName);
			LUA->SetField(-2, "GetControllerName");
			LUA->PushCFunction(SendOutputMessage);
			LUA->SetField(-2, "SendOutputMessage");
			LUA->PushCFunction(GetMessageChannel);
			LUA->SetField(-2, "GetMessageChannel");
			LUA->PushCFunction(IsControllerMSB);
			LUA->SetField(-2, "IsControllerMSB");
		LUA->SetField(-2, "midi");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	delete midiIn;
	delete midiOut;

	inputQueue = {};

	LUA->PushSpecial(SPECIAL_GLOB);
		for (const auto& pair : noteName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		noteName.clear();

		for (const auto& pair : messageName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		messageName.clear();

		for (const auto& pair : controllerName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}
		
		controllerName.clear();

		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Remove");
				LUA->PushString("Think");
				LUA->PushString("MidiInputThink");
			LUA->Call(2, 0);
		LUA->Pop();

		LUA->PushNil();
		LUA->SetField(-2, "midi");
	LUA->Pop();

	return 0;
}