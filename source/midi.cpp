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

map<unsigned char, const char*> messageEnum
{
	{ 0x80, "MIDI_MESSAGE_NOTE_OFF" },
	{ 0x90, "MIDI_MESSAGE_NOTE_ON" },
	{ 0xA0, "MIDI_MESSAGE_KEY_PRESSURE" },
	{ 0xB0, "MIDI_MESSAGE_CONTROLLER_CHANGE" },
	{ 0xC0, "MIDI_MESSAGE_PROGRAM_CHANGE" },
	{ 0xD0, "MIDI_MESSAGE_CHANNEL_PRESSURE" },
	{ 0xE0, "MIDI_MESSAGE_PITCH_BEND" },
	{ 0xF0, "MIDI_MESSAGE_SYSEX_START" },
	{ 0xF2, "MIDI_MESSAGE_SONG_POSITION" },
	{ 0xF3, "MIDI_MESSAGE_SONG_SELECT" },
	{ 0xF5, "MIDI_MESSAGE_BUS_SELECT" },
	{ 0xF6, "MIDI_MESSAGE_TUNE_REQUEST" },
	{ 0xF7, "MIDI_MESSAGE_SYSEX_END" },
	{ 0xF8, "MIDI_MESSAGE_TIMING_TICK" },
	{ 0xFA, "MIDI_MESSAGE_START_SONG" },
	{ 0xFB, "MIDI_MESSAGE_CONTINUE_SONG" },
	{ 0xFC, "MIDI_MESSAGE_STOP_SONG" },
	{ 0xFE, "MIDI_MESSAGE_ACTIVE_SENSING" },
	{ 0xFF, "MIDI_MESSAGE_SYSTEM_RESET" },
};

map<unsigned char, const char*> controllerEnum
{
	{ 1,   "MIDI_CONTROLLER_MODULATION_WHEEL_MSB" },
	{ 2,   "MIDI_CONTROLLER_BREATH_CONTROLLER_MSB" },
	{ 4,   "MIDI_CONTROLLER_FOOT_PEDAL_MSB" },
	{ 5,   "MIDI_CONTROLLER_PORTAMENTO_TIME_MSB" },
	{ 6,   "MIDI_CONTROLLER_DATA_ENTRY_MSB" },
	{ 7,   "MIDI_CONTROLLER_VOLUME_MSB" },
	{ 8,   "MIDI_CONTROLLER_BALANCE_MSB" },
	{ 10,  "MIDI_CONTROLLER_PAN_MSB" },
	{ 11,  "MIDI_CONTROLLER_EXPRESSION_MSB" },
	{ 16,  "MIDI_CONTROLLER_GENERAL_PURPOSE_1_MSB" },
	{ 17,  "MIDI_CONTROLLER_GENERAL_PURPOSE_2_MSB" },
	{ 18,  "MIDI_CONTROLLER_GENERAL_PURPOSE_3_MSB" },
	{ 19,  "MIDI_CONTROLLER_GENERAL_PURPOSE_4_MSB" },
	{ 33,  "MIDI_CONTROLLER_MODULATION_WHEEL_LSB" },
	{ 34,  "MIDI_CONTROLLER_BREATH_CONTROLLER_LSB" },
	{ 36,  "MIDI_CONTROLLER_FOOT_PEDAL_LSB" },
	{ 37,  "MIDI_CONTROLLER_PORTAMENTO_TIME_LSB" },
	{ 38,  "MIDI_CONTROLLER_DATA_ENTRY_LSB" },
	{ 39,  "MIDI_CONTROLLER_VOLUME_LSB" },
	{ 40,  "MIDI_CONTROLLER_BALANCE_LSB" },
	{ 42,  "MIDI_CONTROLLER_PAN_LSB" },
	{ 43,  "MIDI_CONTROLLER_EXPRESSION_LSB" },
	{ 48,  "MIDI_CONTROLLER_GENERAL_PURPOSE_1_LSB" },
	{ 49,  "MIDI_CONTROLLER_GENERAL_PURPOSE_2_LSB" },
	{ 50,  "MIDI_CONTROLLER_GENERAL_PURPOSE_3_LSB" },
	{ 51,  "MIDI_CONTROLLER_GENERAL_PURPOSE_4_LSB" },
	{ 64,  "MIDI_CONTROLLER_DAMPER_PEDAL" },
	{ 65,  "MIDI_CONTROLLER_PORTAMENTO" },
	{ 66,  "MIDI_CONTROLLER_SOSTENUTO" },
	{ 67,  "MIDI_CONTROLLER_SOFT_PEDAL" },
	{ 69,  "MIDI_CONTROLLER_HOLD_2" },
	{ 80,  "MIDI_CONTROLLER_GENERAL_PURPOSE_5" },
	{ 81,  "MIDI_CONTROLLER_GENERAL_PURPOSE_6" },
	{ 82,  "MIDI_CONTROLLER_GENERAL_PURPOSE_7" },
	{ 83,  "MIDI_CONTROLLER_GENERAL_PURPOSE_8" },
	{ 92,  "MIDI_CONTROLLER_TEMOLO_DEPTH" },
	{ 93,  "MIDI_CONTROLLER_CHORUS_DEPTH" },
	{ 94,  "MIDI_CONTROLLER_CELESTE_DEPTH" },
	{ 95,  "MIDI_CONTROLLER_PHASER_DEPTH" },
	{ 96,  "MIDI_CONTROLLER_DATA_INCREMENT" },
	{ 97,  "MIDI_CONTROLLER_DATA_DECREMENT" },
	{ 98,  "MIDI_CONTROLLER_NRPN_LSB" },
	{ 99,  "MIDI_CONTROLLER_NRPN_MSB" },
	{ 100, "MIDI_CONTROLLER_RPN_LSB" },
	{ 101, "MIDI_CONTROLLER_RPN_MSB" },
	{ 122, "MIDI_CONTROLLER_LOCAL_CONTROL" },
	{ 123, "MIDI_CONTROLLER_ALL_NOTES_OFF" },
	{ 124, "MIDI_CONTROLLER_OMNI_MODE_OFF" },
	{ 125, "MIDI_CONTROLLER_OMNI_MODE_ON" },
	{ 126, "MIDI_CONTROLLER_MONO_MODE_ON" },
	{ 127, "MIDI_CONTROLLER_POLY_MODE_ON" },
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
	if (!midi->isPortOpen())
	{
		return 0;
	}

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

int getEnumName(ILuaBase* LUA, map<unsigned char, const char*> map)
{
	unsigned char key = (unsigned char)LUA->CheckNumber(1);

	if (map.count(key) > 0)
	{
		LUA->PushString(map.find(key)->second);
	}
	else
	{
		LUA->PushNil();
	}

	return 1;
}

int sendOutputMessage(ILuaBase* LUA)
{
	vector<unsigned char> message;

	while (LUA->Top() > 0)
	{
		message.push_back((unsigned char)LUA->CheckNumber(1));

		LUA->Pop();
	}

	if (!message.empty())
	{
		try
		{
			output->sendMessage(&message);
		}
		catch (RtMidiError& error)
		{
			LUA->ThrowError(error.getMessage().c_str());
		}
	}

	return 0;
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

LUA_FUNCTION(GetMessageName)
{
	return getEnumName(LUA, messageEnum);
}

LUA_FUNCTION(GetControllerName)
{
	return getEnumName(LUA, controllerEnum);
}

LUA_FUNCTION(SendOutputMessage)
{
	return sendOutputMessage(LUA);
}

LUA_FUNCTION(InputThink)
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
				LUA->PushCFunction(InputThink);
			LUA->Call(3, 0);
		LUA->Pop();

		for (const auto& pair : messageEnum)
		{
			LUA->PushNumber(pair.first);
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : controllerEnum)
		{
			LUA->PushNumber(pair.first);
			LUA->SetField(-2, pair.second);
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

		for (const auto& pair : messageEnum)
		{
			LUA->PushNil();
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : controllerEnum)
		{
			LUA->PushNil();
			LUA->SetField(-2, pair.second);
		}
	LUA->Pop();

	return 0;
}