/*
	Modified BSD License

	Copyright (C) 2022 Brandon Little

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived
	   from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
	THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <map>	
#include <queue>
#include <string>
#include "RtMidi.h"
#include "GarrysMod/Lua/Interface.h"

using namespace std;
using namespace GarrysMod::Lua;

RtMidiIn*  input;
RtMidiOut* output;

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
	{ 0xF0, { "MIDI_MESSAGE_SYSEX_START" } },
	{ 0xF2, { "MIDI_MESSAGE_SONG_POSITION" } },
	{ 0xF3, { "MIDI_MESSAGE_SONG_SELECT" } },
	{ 0xF5, { "MIDI_MESSAGE_BUS_SELECT" } },
	{ 0xF6, { "MIDI_MESSAGE_TUNE_REQUEST" } },
	{ 0xF7, { "MIDI_MESSAGE_SYSEX_END" } },
	{ 0xF8, { "MIDI_MESSAGE_TIMING_TICK" } },
	{ 0xFA, { "MIDI_MESSAGE_START_SONG" } },
	{ 0xFB, { "MIDI_MESSAGE_CONTINUE_SONG" } },
	{ 0xFC, { "MIDI_MESSAGE_STOP_SONG" } },
	{ 0xFE, { "MIDI_MESSAGE_ACTIVE_SENSING" } },
	{ 0xFF, { "MIDI_MESSAGE_SYSTEM_RESET" } },
};

map<unsigned char, vector<const char*>> controllerName
{
	{ 1,   { "MIDI_CONTROLLER_MODULATION_WHEEL_MSB" } },
	{ 2,   { "MIDI_CONTROLLER_BREATH_CONTROLLER_MSB" } },
	{ 4,   { "MIDI_CONTROLLER_FOOT_PEDAL_MSB" } },
	{ 5,   { "MIDI_CONTROLLER_PORTAMENTO_TIME_MSB" } },
	{ 6,   { "MIDI_CONTROLLER_DATA_ENTRY_MSB" } },
	{ 7,   { "MIDI_CONTROLLER_VOLUME_MSB" } },
	{ 8,   { "MIDI_CONTROLLER_BALANCE_MSB" } },
	{ 10,  { "MIDI_CONTROLLER_PAN_MSB" } },
	{ 11,  { "MIDI_CONTROLLER_EXPRESSION_MSB" } },
	{ 16,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_1_MSB" } },
	{ 17,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_2_MSB" } },
	{ 18,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_3_MSB" } },
	{ 19,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_4_MSB" } },
	{ 33,  { "MIDI_CONTROLLER_MODULATION_WHEEL_LSB" } },
	{ 34,  { "MIDI_CONTROLLER_BREATH_CONTROLLER_LSB" } },
	{ 36,  { "MIDI_CONTROLLER_FOOT_PEDAL_LSB" } },
	{ 37,  { "MIDI_CONTROLLER_PORTAMENTO_TIME_LSB" } },
	{ 38,  { "MIDI_CONTROLLER_DATA_ENTRY_LSB" } },
	{ 39,  { "MIDI_CONTROLLER_VOLUME_LSB" } },
	{ 40,  { "MIDI_CONTROLLER_BALANCE_LSB" } },
	{ 42,  { "MIDI_CONTROLLER_PAN_LSB" } },
	{ 43,  { "MIDI_CONTROLLER_EXPRESSION_LSB" } },
	{ 48,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_1_LSB" } },
	{ 49,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_2_LSB" } },
	{ 50,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_3_LSB" } },
	{ 51,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_4_LSB" } },
	{ 64,  { "MIDI_CONTROLLER_DAMPER_PEDAL" } },
	{ 65,  { "MIDI_CONTROLLER_PORTAMENTO" } },
	{ 66,  { "MIDI_CONTROLLER_SOSTENUTO" } },
	{ 67,  { "MIDI_CONTROLLER_SOFT_PEDAL" } },
	{ 69,  { "MIDI_CONTROLLER_HOLD_2" } },
	{ 80,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_5" } },
	{ 81,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_6" } },
	{ 82,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_7" } },
	{ 83,  { "MIDI_CONTROLLER_GENERAL_PURPOSE_8" } },
	{ 92,  { "MIDI_CONTROLLER_TEMOLO_DEPTH" } },
	{ 93,  { "MIDI_CONTROLLER_CHORUS_DEPTH" } },
	{ 94,  { "MIDI_CONTROLLER_CELESTE_DEPTH" } },
	{ 95,  { "MIDI_CONTROLLER_PHASER_DEPTH" } },
	{ 96,  { "MIDI_CONTROLLER_DATA_INCREMENT" } },
	{ 97,  { "MIDI_CONTROLLER_DATA_DECREMENT" } },
	{ 98,  { "MIDI_CONTROLLER_NRPN_LSB" } },
	{ 99,  { "MIDI_CONTROLLER_NRPN_MSB" } },
	{ 100, { "MIDI_CONTROLLER_RPN_LSB" } },
	{ 101, { "MIDI_CONTROLLER_RPN_MSB" } },
	{ 122, { "MIDI_CONTROLLER_LOCAL_CONTROL" } },
	{ 123, { "MIDI_CONTROLLER_ALL_NOTES_OFF" } },
	{ 124, { "MIDI_CONTROLLER_OMNI_MODE_OFF" } },
	{ 125, { "MIDI_CONTROLLER_OMNI_MODE_ON" } },
	{ 126, { "MIDI_CONTROLLER_MONO_MODE_ON" } },
	{ 127, { "MIDI_CONTROLLER_POLY_MODE_ON" } },
};

void inputCallback(double delta, vector<unsigned char>* message, void* userData)
{
	if (!message->empty())
	{
		inputQueue.push(*message);
	}
}

int openPort(ILuaBase* LUA, RtMidi* midi, const char* callback)
{
	if (midi->isPortOpen())
	{
		midi->closePort();
	}

	unsigned int port = (unsigned int)LUA->CheckNumber(1);

	try
	{
		midi->openPort(port);
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	if (midi->isPortOpen())
	{
		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString(callback);
					LUA->PushNumber((double)port);
				LUA->Call(2, 0);
			LUA->Pop();
		LUA->Pop();

		LUA->PushBool(true);
	}
	else
	{
		LUA->PushBool(false);
	}

	return 1;
}

int closePort(ILuaBase* LUA, RtMidi* midi, const char* callback)
{
	if (midi->isPortOpen())
	{
		try
		{
			midi->closePort();
		}
		catch (RtMidiError& error)
		{
			LUA->ThrowError(error.getMessage().c_str());
		}

		if (!midi->isPortOpen())
		{
			LUA->PushSpecial(SPECIAL_GLOB);
				LUA->GetField(-1, "hook");
					LUA->GetField(-1, "Run");
						LUA->PushString(callback);
					LUA->Call(1, 0);
				LUA->Pop();
			LUA->Pop();

			LUA->PushBool(true);
		}
		else
		{
			LUA->PushBool(false);
		}
	}
	else
	{
		LUA->PushBool(false);
	}

	return 1;
}

int isPortOpen(ILuaBase* LUA, RtMidi* midi)
{
	bool open;

	try
	{
		open = midi->isPortOpen();
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushBool(open);

	return 1;
}

int getPortCount(ILuaBase* LUA, RtMidi* midi)
{
	unsigned int count;

	try
	{
		count = midi->getPortCount();
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushNumber(count);

	return 1;
}

int getPortName(ILuaBase* LUA, RtMidi* midi)
{
	string name;

	try
	{
		name = midi->getPortName((unsigned int)LUA->CheckNumber(1));
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushString(name.c_str());

	return 1;
}

int getEnumName(ILuaBase* LUA, map<unsigned char, vector<const char*>> map)
{
	unsigned char key = (unsigned char)LUA->CheckNumber(1);

	if (map.count(key) > 0)
	{
		const auto& name = map.find(key)->second;

		for (const auto& name : name)
		{
			LUA->PushString(name);
		}

		return (int)name.size();
	}
	else
	{
		LUA->PushNil();

		return 1;
	}
}

int sendOutputMessage(ILuaBase* LUA)
{
	if (output->isPortOpen())
	{
		vector<unsigned char> message;

		for (int i = 1; i <= LUA->Top(); i++)
		{
			message.push_back((unsigned char)LUA->CheckNumber(i));
		}

		if (message.empty())
		{
			LUA->PushBool(false);
		}
		else
		{
			try
			{
				output->sendMessage(&message);
			}
			catch (RtMidiError& error)
			{
				LUA->ThrowError(error.getMessage().c_str());
			}

			LUA->PushSpecial(SPECIAL_GLOB);
				LUA->GetField(-1, "hook");
					LUA->GetField(-1, "Run");
						LUA->PushString("OnMidiOutputMessage");

						for (const auto& number : message)
						{
							LUA->PushNumber((double)number);
						}
					LUA->Call((int)message.size() + 1, 0);
				LUA->Pop();
			LUA->Pop();

			LUA->PushBool(true);
		}
	}
	else
	{
		LUA->PushBool(false);
	}

	return 1;
}

int inputThink(ILuaBase* LUA)
{
	while (!inputQueue.empty())
	{
		const auto& message = inputQueue.front();

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString("OnMidiInputMessage");

					for (const auto& number : message)
					{
						LUA->PushNumber((double)number);
					}
				LUA->Call((int)message.size() + 1, 0);
			LUA->Pop();
		LUA->Pop();

		inputQueue.pop();
	}

	return 0;
}

LUA_FUNCTION(OpenInputPort)
{
	return openPort(LUA, input, "OnMidiInputPortOpened");
}

LUA_FUNCTION(OpenOutputPort)
{
	return openPort(LUA, output, "OnMidiOutputPortOpened");
}

LUA_FUNCTION(CloseInputPort)
{
	return closePort(LUA, input, "OnMidiInputPortClosed");
}

LUA_FUNCTION(CloseOutputPort)
{
	return closePort(LUA, output, "OnMidiOutputPortClosed");
}

LUA_FUNCTION(IsInputPortOpen)
{
	return isPortOpen(LUA, input);
}

LUA_FUNCTION(IsOutputPortOpen)
{
	return isPortOpen(LUA, output);
}

LUA_FUNCTION(GetInputPortCount)
{
	return getPortCount(LUA, input);
}

LUA_FUNCTION(GetOutputPortCount)
{
	return getPortCount(LUA, output);
}

LUA_FUNCTION(GetInputPortName)
{
	return getPortName(LUA, input);
}

LUA_FUNCTION(GetOutputPortName)
{
	return getPortName(LUA, output);
}

LUA_FUNCTION(GetNoteName)
{
	return getEnumName(LUA, noteName);
}

LUA_FUNCTION(GetMessageName)
{
	return getEnumName(LUA, messageName);
}

LUA_FUNCTION(GetControllerName)
{
	return getEnumName(LUA, controllerName);
}

LUA_FUNCTION(SendOutputMessage)
{
	return sendOutputMessage(LUA);
}

LUA_FUNCTION(MidiInputThink)
{
	return inputThink(LUA);
}

GMOD_MODULE_OPEN()
{
	try
	{	
		input  = new RtMidiIn();
		input->setCallback(&inputCallback);
		output = new RtMidiOut();
	}
	catch (RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->CreateTable();
			LUA->PushCFunction(OpenInputPort);
			LUA->SetField(-2, "OpenInputPort");
			LUA->PushCFunction(OpenOutputPort);
			LUA->SetField(-2, "OpenOutputPort");
			LUA->PushCFunction(CloseInputPort);
			LUA->SetField(-2, "CloseInputPort");
			LUA->PushCFunction(CloseOutputPort);
			LUA->SetField(-2, "CloseOutputPort");
			LUA->PushCFunction(IsInputPortOpen);
			LUA->SetField(-2, "IsInputPortOpen");
			LUA->PushCFunction(IsOutputPortOpen);
			LUA->SetField(-2, "IsOutputPortOpen");
			LUA->PushCFunction(GetInputPortCount);
			LUA->SetField(-2, "GetInputPortCount");
			LUA->PushCFunction(GetOutputPortCount);
			LUA->SetField(-2, "GetOutputPortCount");
			LUA->PushCFunction(GetInputPortName);
			LUA->SetField(-2, "GetInputPortName");
			LUA->PushCFunction(GetOutputPortName);
			LUA->SetField(-2, "GetOutputPortName");
			LUA->PushCFunction(GetNoteName);
			LUA->SetField(-2, "GetNoteName");
			LUA->PushCFunction(GetMessageName);
			LUA->SetField(-2, "GetMessageName");
			LUA->PushCFunction(GetControllerName);
			LUA->SetField(-2, "GetControllerName");
			LUA->PushCFunction(SendOutputMessage);
			LUA->SetField(-2, "SendOutputMessage");
		LUA->SetField(-2, "midi");

		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Add");
				LUA->PushString("Think");
				LUA->PushString("MidiInputThink");
				LUA->PushCFunction(MidiInputThink);
			LUA->Call(3, 0);
		LUA->Pop();

		LUA->PushNumber((double)noteName.begin()->first);
		LUA->SetField(-2, "MIDI_NOTE_FIRST");
		LUA->PushNumber((double)noteName.rbegin()->first);
		LUA->SetField(-2, "MIDI_NOTE_LAST");
		LUA->PushNumber((double)noteName.size());
		LUA->SetField(-2, "MIDI_NOTE_COUNT");

		LUA->PushNumber((double)messageName.begin()->first);
		LUA->SetField(-2, "MIDI_MESSAGE_FIRST");
		LUA->PushNumber((double)messageName.rbegin()->first);
		LUA->SetField(-2, "MIDI_MESSAGE_LAST");
		LUA->PushNumber((double)messageName.size());
		LUA->SetField(-2, "MIDI_MESSAGE_COUNT");

		LUA->PushNumber((double)controllerName.begin()->first);
		LUA->SetField(-2, "MIDI_CONTROLLER_FIRST");
		LUA->PushNumber((double)controllerName.rbegin()->first);
		LUA->SetField(-2, "MIDI_CONTROLLER_LAST");
		LUA->PushNumber((double)controllerName.size());
		LUA->SetField(-2, "MIDI_CONTROLLER_COUNT");

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
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	delete input;
	delete output;

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->PushNil();
		LUA->SetField(-2, "midi");

		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Remove");
				LUA->PushString("Think");
				LUA->PushString("MidiInputThink");
			LUA->Call(2, 0);
		LUA->Pop();

		LUA->PushNil();
		LUA->SetField(-2, "MIDI_NOTE_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_NOTE_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_NOTE_COUNT");

		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_COUNT");

		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROLLER_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROLLER_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROLLER_COUNT");

		for (const auto& pair : noteName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : messageName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : controllerName)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}
	LUA->Pop();

	return 0;
}