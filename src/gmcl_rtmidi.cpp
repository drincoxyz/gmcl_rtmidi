/*
	gmcl_rtmidi: real-time MIDI I/O for Garry's Mod
	Copyright (c) 2021-2022 Brandon Little

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice,
	   this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from this
	   software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

#include <vector>
#include "RtMidi.h"
#include "GarrysMod/Lua/Interface.h"

using namespace std;
using namespace GarrysMod::Lua;

RtMidiIn* midiIn;
RtMidiOut* midiOut;

int isPortOpen(ILuaBase* LUA, RtMidi* midi) {
	try {
		LUA->PushBool(midi->isPortOpen());
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 1;
}
int getPortCount(ILuaBase* LUA, RtMidi* midi) {
	try {
		LUA->PushNumber(midi->getPortCount());
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 1;
}
int getPortName(ILuaBase* LUA, RtMidi* midi) {
	try {
		LUA->PushString(midi->getPortName((unsigned int)LUA->CheckNumber(1)).c_str());
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 1;
}
int closePort(ILuaBase* LUA, RtMidi* midi, const char* should, const char* callback) {
	try {
		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString(should);
				LUA->Call(1, 1);
					if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1)) {
						midi->closePort();

						LUA->GetField(-2, "Run");
							LUA->PushString(callback);
						LUA->Call(1, 0);
					}
				LUA->Pop();
			LUA->Pop();
		LUA->Pop();
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 0;
}
int openPort(ILuaBase* LUA, RtMidi* midi, const char* should, const char* callback) {
	try {
		const auto port = LUA->CheckNumber(1);

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString(should);
					LUA->PushNumber(port);
				LUA->Call(2, 1);
					if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1)) {
						midi->openPort((unsigned int)port);

						LUA->GetField(-2, "Run");
							LUA->PushString(callback);
							LUA->PushNumber(port);
						LUA->Call(2, 0);
					}
				LUA->Pop();
			LUA->Pop();
		LUA->Pop();
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 0;
}

LUA_FUNCTION(isInputPortOpen) {
	return isPortOpen(LUA, midiIn);
}
LUA_FUNCTION(getInputPortCount) {
	return getPortCount(LUA, midiIn);
}
LUA_FUNCTION(getInputPortName) {
	return getPortName(LUA, midiIn);
}
LUA_FUNCTION(closeInputPort) {
	return closePort(LUA, midiIn, "ShouldCloseMIDIInputPort", "OnMIDIInputPortClosed");
}
LUA_FUNCTION(openInputPort) {
	return openPort(LUA, midiIn, "ShouldOpenMIDIInputPort", "OnMIDIInputPortOpened");
}
LUA_FUNCTION(isOutputPortOpen) {
	return isPortOpen(LUA, midiOut);
}
LUA_FUNCTION(getOutputPortCount) {
	return getPortCount(LUA, midiOut);
}
LUA_FUNCTION(getOutputPortName) {
	return getPortName(LUA, midiOut);
}
LUA_FUNCTION(closeOutputPort) {
	return closePort(LUA, midiOut, "ShouldCloseMIDIOutputPort", "OnMIDIOutputPortClosed");
}
LUA_FUNCTION(openOutputPort) {
	return openPort(LUA, midiOut, "ShouldOpenMIDIInputPort", "OnMIDIInputPortOpened");
}

LUA_FUNCTION(sendMessage) {
	try {
		vector<unsigned char> message;

		for (int i = 0; i < LUA->Top(); ++i) {
			message.push_back((unsigned char)LUA->CheckNumber(i));
		}

		const auto size = (int)message.size();

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Run");
					LUA->PushString("ShouldSendMIDIMessage");

					for (const double byte : message) {
						LUA->PushNumber(byte);
					}
				LUA->Call(size + 1, 1);
					if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1)) {
						midiOut->sendMessage(&message);

						LUA->GetField(-2, "Run");
							LUA->PushString("OnMIDIMessageSent");

							for (const double byte : message) {
								LUA->PushNumber(byte);
							}
						LUA->Call(size + 1, 0);
					}
				LUA->Pop();
			LUA->Pop();
		LUA->Pop();
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 0;
}
LUA_FUNCTION(receiveMessage) {
	try {
		vector<unsigned char> message;

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				for (midiIn->getMessage(&message); !message.empty(); midiIn->getMessage(&message)) {
					const auto size = (int)message.size();

					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldReceiveMIDIMessage");

						for (const double byte : message) {
							LUA->PushNumber(byte);
						}
					LUA->Call(size + 1, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool(-1)) {
							LUA->GetField(-2, "Run");
								LUA->PushString("OnMIDIMessageReceived");

								for (const double byte : message) {
									LUA->PushNumber(byte);
								}
							LUA->Call(size + 1, 0);
						}
					LUA->Pop();
				}
			LUA->Pop();
		LUA->Pop();
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 0;
}

GMOD_MODULE_OPEN() {
	try {
		midiIn = new RtMidiIn();
		midiOut = new RtMidiOut();

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Add");
					LUA->PushString("Think");
					LUA->PushString("ReceiveMIDIMessage");
					LUA->PushCFunction(receiveMessage);
				LUA->Call(3, 0);
			LUA->Pop();

			LUA->CreateTable();
				LUA->PushCFunction(isInputPortOpen);
				LUA->SetField(-2, "IsInputPortOpen");
				LUA->PushCFunction(getInputPortCount);
				LUA->SetField(-2, "GetInputPortCount");
				LUA->PushCFunction(getInputPortName);
				LUA->SetField(-2, "GetInputPortName");
				LUA->PushCFunction(closeInputPort);
				LUA->SetField(-2, "CloseInputPort");
				LUA->PushCFunction(openInputPort);
				LUA->SetField(-2, "OpenInputPort");
				LUA->PushCFunction(isOutputPortOpen);
				LUA->SetField(-2, "IsOutputPortOpen");
				LUA->PushCFunction(getOutputPortCount);
				LUA->SetField(-2, "GetOutputPortCount");
				LUA->PushCFunction(getOutputPortName);
				LUA->SetField(-2, "GetOutputPortName");
				LUA->PushCFunction(closeOutputPort);
				LUA->SetField(-2, "CloseOutputPort");
				LUA->PushCFunction(openOutputPort);
				LUA->SetField(-2, "OpenOutputPort");
				LUA->PushCFunction(sendMessage);
				LUA->SetField(-2, "SendMessage");
			LUA->SetField(-2, "rtmidi");
		LUA->Pop();
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 0;
}
GMOD_MODULE_CLOSE() {
	try {
		delete midiIn;
		delete midiOut;

		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				LUA->GetField(-1, "Remove");
					LUA->PushString("Think");
					LUA->PushString("ReceiveMIDIMessage");
				LUA->Call(2, 0);
			LUA->Pop();

			LUA->PushNil();
			LUA->SetField(-2, "rtmidi");
		LUA->Pop();
	}
	catch (exception& error) {
		LUA->ThrowError(error.what());
	}
	return 0;
}