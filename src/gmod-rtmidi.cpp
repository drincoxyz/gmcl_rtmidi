/*
	gmod-rtmidi: realtime MIDI I/O for Garry's Mod
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

#include <vector>
#include "RtMidi.h"
#include "GarrysMod/Lua/Interface.h"

RtMidiIn*  midi_in;
RtMidiOut* midi_out;

int open_port(GarrysMod::Lua::ILuaBase* LUA, RtMidi* midi, const char* should, const char* callback)
{
	const auto port = (unsigned int)LUA->CheckNumber(1);

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString(should);
				LUA->PushNumber((double)port);
			LUA->Call(2, 1);
				if (LUA->IsType(-1, GarrysMod::Lua::Type::Nil) || LUA->GetBool(-1))
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

int close_port(GarrysMod::Lua::ILuaBase* LUA, RtMidi* midi, const char* should, const char* callback)
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString(should);
			LUA->Call(1, 1);
				if (LUA->IsType(-1, GarrysMod::Lua::Type::Nil) || LUA->GetBool(-1))
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

int is_port_open(GarrysMod::Lua::ILuaBase* LUA, RtMidi* midi)
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

int get_port_name(GarrysMod::Lua::ILuaBase* LUA, RtMidi* midi)
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

int get_port_count(GarrysMod::Lua::ILuaBase* LUA, RtMidi* midi)
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
	std::vector<unsigned char> message = { (unsigned char)LUA->CheckNumber(1) };

	for (int i = 2; i <= LUA->Top(); i++)
	{
		message.push_back((unsigned char)LUA->CheckNumber(i));
	}

	const auto size = (int)message.size();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Run");
				LUA->PushString("ShouldSendMidiMessage");

				for (const double byte : message)
				{
					LUA->PushNumber(byte);
				}
			LUA->Call(size + 1, 1);
				if (LUA->IsType(-1, GarrysMod::Lua::Type::Nil) || LUA->GetBool(-1))
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
	std::vector<unsigned char> message;

	for (midi_in->getMessage(&message); !message.empty(); midi_in->getMessage(&message))
	{
		const auto size = (int)message.size();

		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString("ShouldReceiveMidiMessage");

					for (const double byte : message)
					{
						LUA->PushNumber(byte);
					}
				LUA->Call(size + 1, 1);
					if (LUA->IsType(-1, GarrysMod::Lua::Type::Nil) || LUA->GetBool(-1))
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

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Add");
				LUA->PushString("Think");
				LUA->PushString("MidiReceiveMessage");
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
		LUA->SetField(-2, "rtmidi");
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

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Remove");
				LUA->PushString("Think");
				LUA->PushString("ReceiveMidiMessage");
			LUA->Call(2, 0);
		LUA->Pop();

		LUA->PushNil();
		LUA->SetField(-2, "rtmidi");
	LUA->Pop();

	return 0;
}