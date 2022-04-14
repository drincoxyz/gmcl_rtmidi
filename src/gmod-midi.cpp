/*
	gmod-midi: real-time MIDI I/O for Garry's Mod
	Copyright (c) 2021-2022 Brandon Little

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
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
	ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <map>
#include <cmath>
#include <vector>
#include "RtMidi.h"
#include "GarrysMod/Lua/Interface.h"

using namespace std;
using namespace GarrysMod::Lua;

RtMidiIn*  midi_in;
RtMidiOut* midi_out;

const map<unsigned char, const char*> message_name
{
	{ 0x78, "MIDI_MESSAGE_ALL_SOUND_OFF" },
	{ 0x79, "MIDI_MESSAGE_RESET_ALL_CONTROLLERS" },
	{ 0x7A, "MIDI_MESSAGE_LOCAL_CONTROL" },
	{ 0x7B, "MIDI_MESSAGE_ALL_NOTES_OFF" },
	{ 0x7C, "MIDI_MESSAGE_OMNI_OFF" },
	{ 0x7D, "MIDI_MESSAGE_OMNI_ON" },
	{ 0x7E, "MIDI_MESSAGE_MONO_ON" },
	{ 0x7F, "MIDI_MESSAGE_POLY_ON" },
	{ 0x80, "MIDI_MESSAGE_NOTE_OFF" },
	{ 0x90, "MIDI_MESSAGE_NOTE_ON" },
	{ 0xA0, "MIDI_MESSAGE_KEY_PRESSURE" },
	{ 0xB0, "MIDI_MESSAGE_CONTROL_CHANGE" },
	{ 0xC0, "MIDI_MESSAGE_PROGRAM_CHANGE" },
	{ 0xD0, "MIDI_MESSAGE_CHANNEL_PRESSURE" },
	{ 0xE0, "MIDI_MESSAGE_PITCH_BEND" },
	{ 0xF0, "MIDI_MESSAGE_SYSTEM_EXCLUSIVE" },
	{ 0xF1, "MIDI_MESSAGE_TIME_CODE" },
	{ 0xF2, "MIDI_MESSAGE_SONG_POSITION" },
	{ 0xF3, "MIDI_MESSAGE_SONG_SELECT" },
	{ 0xF6, "MIDI_MESSAGE_TUNE_REQUEST" },
	{ 0xF7, "MIDI_MESSAGE_END_OF_EXCLUSIVE" },
	{ 0xF8, "MIDI_MESSAGE_TIMING_CLOCK" },
	{ 0xFA, "MIDI_MESSAGE_START" },
	{ 0xFB, "MIDI_MESSAGE_CONTINUE" },
	{ 0xFC, "MIDI_MESSAGE_STOP" },
	{ 0xFE, "MIDI_MESSAGE_ACTIVE_SENSING" },
	{ 0xFF, "MIDI_MESSAGE_SYSTEM_RESET" },
};

const map<unsigned char, const char*> message_type_name
{
	{ 0, "MIDI_MESSAGE_TYPE_CHANNEL_MODE" },
	{ 1, "MIDI_MESSAGE_TYPE_CHANNEL_VOICE" },
	{ 2, "MIDI_MESSAGE_TYPE_SYSTEM_EXCLUSIVE" },
	{ 3, "MIDI_MESSAGE_TYPE_SYSTEM_COMMON" },
	{ 4, "MIDI_MESSAGE_TYPE_SYSTEM_REAL_TIME" },
};

const map<unsigned char, const char*> control_name
{
	{ 0x00, "MIDI_CONTROL_BANK_SELECT" },
	{ 0x01, "MIDI_CONTROL_MODULATION_WHEEL" },
	{ 0x02, "MIDI_CONTROL_BREATH_CONTROLLER" },
	{ 0x04, "MIDI_CONTROL_FOOT_CONTROLLER" },
	{ 0x05, "MIDI_CONTROL_PORTAMENTO_TIME" },
	{ 0x06, "MIDI_CONTROL_DATA_ENTRY" },
	{ 0x07, "MIDI_CONTROL_CHANNEL_VOLUME" },
	{ 0x08, "MIDI_CONTROL_BALANCE" },
	{ 0x0A, "MIDI_CONTROL_PAN" },
	{ 0x0B, "MIDI_CONTROL_EXPRESSION_CONTROLLER" },
	{ 0x0C, "MIDI_CONTROL_EFFECT_CONTROL_1" },
	{ 0x0D, "MIDI_CONTROL_EFFECT_CONTROL_2" },
	{ 0x10, "MIDI_CONTROL_GENERAL_PURPOSE_1" },
	{ 0x11, "MIDI_CONTROL_GENERAL_PURPOSE_2" },
	{ 0x12, "MIDI_CONTROL_GENERAL_PURPOSE_3" },
	{ 0x13, "MIDI_CONTROL_GENERAL_PURPOSE_4" },
	{ 0x40, "MIDI_CONTROL_DAMPER_PEDAL" },
	{ 0x41, "MIDI_CONTROL_PORTAMENTO_TOGGLE" },
	{ 0x42, "MIDI_CONTROL_SOSTENUTO" },
	{ 0x43, "MIDI_CONTROL_SOFT_PEDAL" },
	{ 0x44, "MIDI_CONTROL_LEGATO_FOOTSWITCH" },
	{ 0x45, "MIDI_CONTROL_HOLD_2" },
	{ 0x46, "MIDI_CONTROL_SOUND_CONTROLLER_1" },
	{ 0x47, "MIDI_CONTROL_SOUND_CONTROLLER_2" },
	{ 0x48, "MIDI_CONTROL_SOUND_CONTROLLER_3" },
	{ 0x49, "MIDI_CONTROL_SOUND_CONTROLLER_4" },
	{ 0x4A, "MIDI_CONTROL_SOUND_CONTROLLER_5" },
	{ 0x4B, "MIDI_CONTROL_SOUND_CONTROLLER_6" },
	{ 0x4C, "MIDI_CONTROL_SOUND_CONTROLLER_7" },
	{ 0x4D, "MIDI_CONTROL_SOUND_CONTROLLER_8" },
	{ 0x4E, "MIDI_CONTROL_SOUND_CONTROLLER_9" },
	{ 0x4F, "MIDI_CONTROL_SOUND_CONTROLLER_10" },
	{ 0x50, "MIDI_CONTROL_GENERAL_PURPOSE_5" },
	{ 0x51, "MIDI_CONTROL_GENERAL_PURPOSE_6" },
	{ 0x52, "MIDI_CONTROL_GENERAL_PURPOSE_7" },
	{ 0x53, "MIDI_CONTROL_GENERAL_PURPOSE_8" },
	{ 0x54, "MIDI_CONTROL_PORTAMENTO_AMOUNT" },
	{ 0x5B, "MIDI_CONTROL_EFFECTS_1_DEPTH" },
	{ 0x5C, "MIDI_CONTROL_EFFECTS_2_DEPTH" },
	{ 0x5D, "MIDI_CONTROL_EFFECTS_3_DEPTH" },
	{ 0x5E, "MIDI_CONTROL_EFFECTS_4_DEPTH" },
	{ 0x5F, "MIDI_CONTROL_EFFECTS_5_DEPTH" },
	{ 0x60, "MIDI_CONTROL_DATA_INCREMENT" },
	{ 0x61, "MIDI_CONTROL_DATA_DECREMENT" },
	{ 0x63, "MIDI_CONTROL_NON_REGISTERED_PARAMETER_NUMBER" },
	{ 0x65, "MIDI_CONTROL_REGISTERED_PARAMETER_NUMBER" },
};

const map<unsigned char, const char*> control_bit_significance_name
{
	{ 0, "MIDI_CONTROL_BIT_SIGNIFICANCE_MOST" },
	{ 1, "MIDI_CONTROL_BIT_SIGNIFICANCE_LEAST" },
};

int open_port(ILuaBase* LUA, RtMidi* midi, const char* should, const char* callback)
{
	const auto port = (unsigned int)LUA->CheckNumber(1);

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString(should);
				LUA->PushNumber((double)port);
			LUA->Call(2, 1);
				if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1))
				{
					try
					{
						midi->openPort(port);
					}
					catch (const RtMidiError& error)
					{
						LUA->ThrowError(error.what());
					}

					LUA->GetField(-2, "Run");
						LUA->PushString(callback);
						LUA->PushNumber((double)port);
					LUA->Call(2, 0);
				}
			LUA->Pop();
		LUA->Pop();
	LUA->Pop();

	return 0;
}

int close_port(ILuaBase* LUA, RtMidi* midi, const char* should, const char* callback)
{
	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString(should);
			LUA->Call(1, 1);
				if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1))
				{
					try
					{
						midi->closePort();
					}
					catch (const RtMidiError& error)
					{
						LUA->ThrowError(error.what());
					}

					LUA->GetField(-2, "Run");
						LUA->PushString(callback);
					LUA->Call(1, 0);
				}
			LUA->Pop();
		LUA->Pop();
	LUA->Pop();

	return 0;
}

int is_port_open(ILuaBase* LUA, RtMidi* midi)
{
	try
	{
		LUA->PushBool(midi->isPortOpen());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.what());
	}

	return 1;
}

int get_port_name(ILuaBase* LUA, RtMidi* midi)
{
	try
	{
		LUA->PushString(midi->getPortName((unsigned int)LUA->CheckNumber(1)).c_str());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.what());
	}

	return 1;
}

int get_port_count(ILuaBase* LUA, RtMidi* midi)
{
	try
	{
		LUA->PushNumber((double)midi->getPortCount());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.what());
	}

	return 1;
}

LUA_FUNCTION(open_input_port)
{
	return open_port(LUA, midi_in, "ShouldOpenMidiInputPort", "OnMidiInputPortOpened");
}

LUA_FUNCTION(close_input_port)
{
	return close_port(LUA, midi_in, "ShouldCloseMidiInputPort", "OnMidiInputPortClosed");
}

LUA_FUNCTION(is_input_port_open)
{
	return is_port_open(LUA, midi_in);
}

LUA_FUNCTION(get_input_port_name)
{
	return get_port_name(LUA, midi_in);
}

LUA_FUNCTION(get_input_port_count)
{
	return get_port_count(LUA, midi_in);
}

LUA_FUNCTION(open_output_port)
{
	return open_port(LUA, midi_out, "ShouldOpenMidiOutputPort", "OnMidiOutputPortOpened");
}

LUA_FUNCTION(close_output_port)
{
	return close_port(LUA, midi_out, "ShouldCloseMidiOutputPort", "OnMidiOutputPortClosed");
}

LUA_FUNCTION(is_output_port_open)
{
	return is_port_open(LUA, midi_out);
}

LUA_FUNCTION(get_output_port_name)
{
	return get_port_name(LUA, midi_out);
}

LUA_FUNCTION(get_output_port_count)
{
	return get_port_count(LUA, midi_out);
}

LUA_FUNCTION(send_message)
{
	vector<unsigned char> message = { (unsigned char)LUA->CheckNumber(1) };

	for (int i = 2; i <= LUA->Top(); i++)
	{
		message.push_back((unsigned char)LUA->CheckNumber(i));
	}

	const auto size = (int)message.size();

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString("ShouldSendMidiMessage");

				for (const double byte : message)
				{
					LUA->PushNumber(byte);
				}
			LUA->Call(size + 1, 1);
				if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1))
				{
					try
					{
						midi_out->sendMessage(&message);
					}
					catch (const RtMidiError& error)
					{
						LUA->ThrowError(error.what());
					}

					LUA->GetField(-2, "Run");
						LUA->PushString("OnMidiMessageSent");

						for (const double byte : message)
						{
							LUA->PushNumber(byte);
						}
					LUA->Call(size + 1, 0);
				}
			LUA->Pop();
		LUA->Pop();
	LUA->Pop();

	return 0;
}

LUA_FUNCTION(receive_message)
{
	vector<unsigned char> message;

	for (midi_in->getMessage(&message); !message.empty(); midi_in->getMessage(&message))
	{
		const auto size = (int)message.size();

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString("ShouldReceiveMidiMessage");

					for (const double byte : message)
					{
						LUA->PushNumber(byte);
					}
				LUA->Call(size + 1, 1);
					if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1))
					{
						LUA->GetField(-2, "Run");
							LUA->PushString("OnMidiMessageReceived");

							for (const double byte : message)
							{
								LUA->PushNumber(byte);
							}
						LUA->Call(size + 1, 0);
					}
				LUA->Pop();
			LUA->Pop();
		LUA->Pop();
	}

	return 0;
}

LUA_FUNCTION(get_message_name)
{
	const auto message = (unsigned char)LUA->CheckNumber(1);

	if (message_name.count(message))
	{
		LUA->PushString(message_name.at(message));
	}
	else
	{
		if (message >= 0x80 && message < 0xF0)
		{
			LUA->PushString(message_name.at(0x80 + (unsigned char)floor(((double)message - 0x80) / 16) * 16));
		}
		else
		{
			LUA->PushString("MIDI_MESSAGE_INVALID");
		}
	}

	return 1;
}

LUA_FUNCTION(get_message_channel)
{
	const auto message = (unsigned char)LUA->CheckNumber(1);

	if (message >= 0x80 && message < 0xF0)
	{
		LUA->PushNumber((message % 16) + 1);
	}
	else
	{
		LUA->PushNumber(-1);
	}

	return 1;
}

LUA_FUNCTION(get_message_type)
{
	const auto message = (unsigned char)LUA->CheckNumber(1);

	if (message_name.count(message))
	{
		if (message < 0x80)
		{
			LUA->PushNumber(0);
		}
		else if (message < 0xF0)
		{
			LUA->PushNumber(1);
		}
		else if (message < 0xF1)
		{
			LUA->PushNumber(2);
		}
		else if (message < 0xF8)
		{
			LUA->PushNumber(3);
		}
		else
		{
			LUA->PushNumber(4);
		}
	}
	else
	{
		if (message >= 0x80 && message < 0xF0)
		{
			LUA->PushNumber(1);
		}
		else
		{
			LUA->PushNumber(-1);
		}
	}

	return 1;
}

LUA_FUNCTION(get_message_type_name)
{
	const auto message_type = (unsigned char)LUA->CheckNumber(1);

	if (message_type_name.count(message_type))
	{
		LUA->PushString(message_type_name.at(message_type));
	}
	else
	{
		LUA->PushString("MIDI_MESSAGE_TYPE_INVALID");
	}

	return 1;
}

LUA_FUNCTION(get_control_name)
{
	const auto control = (unsigned char)LUA->CheckNumber(1);

	if (control_name.count(control))
	{
		LUA->PushString(control_name.at(control));
	}
	else
	{
		if (control >= 0x20 && control < 0x40 && control_name.count(control - 0x20))
		{
			LUA->PushString(control_name.at(control - 0x20));
		}
		else
		{
			LUA->PushString("MIDI_CONTROL_INVALID");
		}
	}

	return 1;
}

LUA_FUNCTION(get_control_bit_significance)
{
	const auto control = (unsigned char)LUA->CheckNumber(1);

	if (control_name.count(control))
	{
		if (control == 0x63 || control == 0x65 || (control >= 0x00 && control < 0x20))
		{
			LUA->PushNumber(0);
		}
		else if (control >= 0x46 && control < 0x55)
		{
			LUA->PushNumber(1);
		}
		else
		{
			LUA->PushNumber(-1);
		}
	}
	else
	{
		if (control == 0x62 || control == 0x64 || (control >= 0x20 && control < 0x40 && control_name.count(control - 0x20)))
		{
			LUA->PushNumber(1);
		}
		else
		{
			LUA->PushNumber(-1);
		}
	}

	return 1;
}

LUA_FUNCTION(get_control_bit_significance_name)
{
	const auto control_bit_significance = (unsigned char)LUA->CheckNumber(1);

	if (control_bit_significance_name.count(control_bit_significance))
	{
		LUA->PushString(control_bit_significance_name.at(control_bit_significance));
	}
	else
	{
		LUA->PushString("MIDI_CONTROL_BIT_SIGNIFICANCE_INVALID");
	}

	return 1;
}

GMOD_MODULE_OPEN()
{
	try
	{
		midi_in  = new RtMidiIn();
		midi_out = new RtMidiOut();
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.what());
	}

	LUA->PushSpecial(SPECIAL_GLOB);
		for (const auto& pair : message_name)
		{
			LUA->PushNumber((double)pair.first);
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : message_type_name)
		{
			LUA->PushNumber((double)pair.first);
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : control_name)
		{
			LUA->PushNumber((double)pair.first);
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : control_bit_significance_name)
		{
			LUA->PushNumber((double)pair.first);
			LUA->SetField(-2, pair.second);
		}

		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_MESSAGE_INVALID");
		LUA->PushNumber((double)message_name.begin()->first);
		LUA->SetField(-2, "MIDI_MESSAGE_FIRST");
		LUA->PushNumber((double)message_name.rbegin()->first);
		LUA->SetField(-2, "MIDI_MESSAGE_LAST");
		LUA->PushNumber((double)message_name.size());
		LUA->SetField(-2, "MIDI_MESSAGE_COUNT");
		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_INVALID");
		LUA->PushNumber((double)message_type_name.begin()->first);
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_FIRST");
		LUA->PushNumber((double)message_type_name.rbegin()->first);
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_LAST");
		LUA->PushNumber((double)message_type_name.size());
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_COUNT");
		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_CONTROL_INVALID");
		LUA->PushNumber((double)control_name.begin()->first);
		LUA->SetField(-2, "MIDI_CONTROL_FIRST");
		LUA->PushNumber((double)control_name.rbegin()->first);
		LUA->SetField(-2, "MIDI_CONTROL_LAST");
		LUA->PushNumber((double)control_name.size());
		LUA->SetField(-2, "MIDI_CONTROL_COUNT");
		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_INVALID");
		LUA->PushNumber((double)control_bit_significance_name.begin()->first);
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_FIRST");
		LUA->PushNumber((double)control_bit_significance_name.rbegin()->first);
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_LAST");
		LUA->PushNumber((double)control_bit_significance_name.size());
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_COUNT");

		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Add");
				LUA->PushString("Think");
				LUA->PushString("ReceiveMidiMessage");
				LUA->PushCFunction(receive_message);
			LUA->Call(3, 0);
		LUA->Pop();

		LUA->CreateTable();
			LUA->PushCFunction(open_input_port);
			LUA->SetField(-2, "OpenInputPort");
			LUA->PushCFunction(close_input_port);
			LUA->SetField(-2, "CloseInputPort");
			LUA->PushCFunction(is_input_port_open);
			LUA->SetField(-2, "IsInputPortOpen");
			LUA->PushCFunction(get_input_port_name);
			LUA->SetField(-2, "GetInputPortName");
			LUA->PushCFunction(get_input_port_count);
			LUA->SetField(-2, "GetInputPortCount");
			LUA->PushCFunction(open_output_port);
			LUA->SetField(-2, "OpenOutputPort");
			LUA->PushCFunction(close_output_port);
			LUA->SetField(-2, "CloseOutputPort");
			LUA->PushCFunction(is_output_port_open);
			LUA->SetField(-2, "IsOutputPortOpen");
			LUA->PushCFunction(get_output_port_name);
			LUA->SetField(-2, "GetOutputPortName");
			LUA->PushCFunction(get_output_port_count);
			LUA->SetField(-2, "GetOutputPortCount");
			LUA->PushCFunction(send_message);
			LUA->SetField(-2, "SendMessage");
			LUA->PushCFunction(get_message_name);
			LUA->SetField(-2, "GetMessageName");
			LUA->PushCFunction(get_message_channel);
			LUA->SetField(-2, "GetMessageChannel");
			LUA->PushCFunction(get_message_type);
			LUA->SetField(-2, "GetMessageType");
			LUA->PushCFunction(get_message_type_name);
			LUA->SetField(-2, "GetMessageTypeName");
			LUA->PushCFunction(get_control_name);
			LUA->SetField(-2, "GetControlName");
			LUA->PushCFunction(get_control_bit_significance);
			LUA->SetField(-2, "GetControlBitSignificance");
			LUA->PushCFunction(get_control_bit_significance_name);
			LUA->SetField(-2, "GetControlBitSignificanceName");
		LUA->SetField(-2, "midi");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	try
	{
		delete midi_in;
		delete midi_out;
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.what());
	}

	LUA->PushSpecial(SPECIAL_GLOB);
		for (const auto& pair : message_name)
		{
			LUA->PushNil();
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : message_type_name)
		{
			LUA->PushNil();
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : control_name)
		{
			LUA->PushNil();
			LUA->SetField(-2, pair.second);
		}

		for (const auto& pair : control_bit_significance_name)
		{
			LUA->PushNil();
			LUA->SetField(-2, pair.second);
		}

		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_COUNT");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_COUNT");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_COUNT");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_COUNT");

		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Remove");
				LUA->PushString("Think");
				LUA->PushString("ReceiveMidiMessage");
			LUA->Call(2, 0);
		LUA->Pop();

		LUA->PushNil();
		LUA->SetField(-2, "midi");
	LUA->Pop();

	return 0;
}