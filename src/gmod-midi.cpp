#include <map>
#include <cmath>
#include <queue>
#include <vector>
#include <string>
#include "RtMidi.h"
#include "GarrysMod/Lua/Interface.h"

using namespace std;
using namespace GarrysMod::Lua;

RtMidiIn*  inputApi;
RtMidiOut* outputApi;

queue<vector<unsigned char>> inputMessages;

const map<const unsigned char, const vector<const char*>> noteNames
{
	{   0, { "MIDI_NOTE_C_1" } },
	{   1, { "MIDI_NOTE_Cs_1", "MIDI_NOTE_Db_1" } },
	{   2, { "MIDI_NOTE_D_1" } },
	{   3, { "MIDI_NOTE_Ds_1", "MIDI_NOTE_Eb_1" } },
	{   4, { "MIDI_NOTE_E_1" } },
	{   5, { "MIDI_NOTE_F_1" } },
	{   6, { "MIDI_NOTE_Fs_1", "MIDI_NOTE_Gb_1" } },
	{   7, { "MIDI_NOTE_G_1" } },
	{   8, { "MIDI_NOTE_Gs_1", "MIDI_NOTE_Ab_1" } },
	{   9, { "MIDI_NOTE_A_1" } },
	{  10, { "MIDI_NOTE_As_1", "MIDI_NOTE_Bb_1" } },
	{  11, { "MIDI_NOTE_B_1" } },
	{  12, { "MIDI_NOTE_C0" } },
	{  13, { "MIDI_NOTE_Cs0", "MIDI_NOTE_Db0" } },
	{  14, { "MIDI_NOTE_D0" } },
	{  15, { "MIDI_NOTE_Ds0", "MIDI_NOTE_Eb0" } },
	{  16, { "MIDI_NOTE_E0" } },
	{  17, { "MIDI_NOTE_F0" } },
	{  18, { "MIDI_NOTE_Fs0", "MIDI_NOTE_Gb0" } },
	{  19, { "MIDI_NOTE_G0" } },
	{  20, { "MIDI_NOTE_Gs0", "MIDI_NOTE_Ab0" } },
	{  21, { "MIDI_NOTE_A0" } },
	{  22, { "MIDI_NOTE_As0", "MIDI_NOTE_Bb0" } },
	{  23, { "MIDI_NOTE_B0" } },
	{  24, { "MIDI_NOTE_C1" } },
	{  25, { "MIDI_NOTE_Cs1", "MIDI_NOTE_Db1" } },
	{  26, { "MIDI_NOTE_D1" } },
	{  27, { "MIDI_NOTE_Ds1", "MIDI_NOTE_Eb1" } },
	{  28, { "MIDI_NOTE_E1" } },
	{  29, { "MIDI_NOTE_F1" } },
	{  30, { "MIDI_NOTE_Fs1", "MIDI_NOTE_Gb1" } },
	{  31, { "MIDI_NOTE_G1" } },
	{  32, { "MIDI_NOTE_Gs1", "MIDI_NOTE_Ab1" } },
	{  33, { "MIDI_NOTE_A1" } },
	{  34, { "MIDI_NOTE_As1", "MIDI_NOTE_Bb1" } },
	{  35, { "MIDI_NOTE_B1" } },
	{  36, { "MIDI_NOTE_C2" } },
	{  37, { "MIDI_NOTE_Cs2", "MIDI_NOTE_Db2" } },
	{  38, { "MIDI_NOTE_D2" } },
	{  39, { "MIDI_NOTE_Ds2", "MIDI_NOTE_Eb2" } },
	{  40, { "MIDI_NOTE_E2" } },
	{  41, { "MIDI_NOTE_F2" } },
	{  42, { "MIDI_NOTE_Fs2", "MIDI_NOTE_Gb2" } },
	{  43, { "MIDI_NOTE_G2" } },
	{  44, { "MIDI_NOTE_Gs2", "MIDI_NOTE_Ab2" } },
	{  45, { "MIDI_NOTE_A2" } },
	{  46, { "MIDI_NOTE_As2", "MIDI_NOTE_Bb2" } },
	{  47, { "MIDI_NOTE_B2" } },
	{  48, { "MIDI_NOTE_C3" } },
	{  49, { "MIDI_NOTE_Cs3", "MIDI_NOTE_Db3" } },
	{  50, { "MIDI_NOTE_D3" } },
	{  51, { "MIDI_NOTE_Ds3", "MIDI_NOTE_Eb3" } },
	{  52, { "MIDI_NOTE_E3" } },
	{  53, { "MIDI_NOTE_F3" } },
	{  54, { "MIDI_NOTE_Fs3", "MIDI_NOTE_Gb3" } },
	{  55, { "MIDI_NOTE_G3" } },
	{  56, { "MIDI_NOTE_Gs3", "MIDI_NOTE_Ab3" } },
	{  57, { "MIDI_NOTE_A3" } },
	{  58, { "MIDI_NOTE_As3", "MIDI_NOTE_Bb3" } },
	{  59, { "MIDI_NOTE_B3" } },
	{  60, { "MIDI_NOTE_C4" } },
	{  61, { "MIDI_NOTE_Cs4", "MIDI_NOTE_Db4" } },
	{  62, { "MIDI_NOTE_D4" } },
	{  63, { "MIDI_NOTE_Ds4", "MIDI_NOTE_Eb4" } },
	{  64, { "MIDI_NOTE_E4" } },
	{  65, { "MIDI_NOTE_F4" } },
	{  66, { "MIDI_NOTE_Fs4", "MIDI_NOTE_Gb4" } },
	{  67, { "MIDI_NOTE_G4" } },
	{  68, { "MIDI_NOTE_Gs4", "MIDI_NOTE_Ab4" } },
	{  69, { "MIDI_NOTE_A4" } },
	{  70, { "MIDI_NOTE_As4", "MIDI_NOTE_Bb4" } },
	{  71, { "MIDI_NOTE_B4" } },
	{  72, { "MIDI_NOTE_C5" } },
	{  73, { "MIDI_NOTE_Cs5", "MIDI_NOTE_Db5" } },
	{  74, { "MIDI_NOTE_D5" } },
	{  75, { "MIDI_NOTE_Ds5", "MIDI_NOTE_Eb5" } },
	{  76, { "MIDI_NOTE_E5" } },
	{  77, { "MIDI_NOTE_F5" } },
	{  78, { "MIDI_NOTE_Fs5", "MIDI_NOTE_Gb5" } },
	{  79, { "MIDI_NOTE_G5" } },
	{  80, { "MIDI_NOTE_Gs5", "MIDI_NOTE_Ab5" } },
	{  81, { "MIDI_NOTE_A5" } },
	{  82, { "MIDI_NOTE_As5", "MIDI_NOTE_Bb5" } },
	{  83, { "MIDI_NOTE_B5" } },
	{  84, { "MIDI_NOTE_C6" } },
	{  85, { "MIDI_NOTE_Cs6", "MIDI_NOTE_Db6" } },
	{  86, { "MIDI_NOTE_D6" } },
	{  87, { "MIDI_NOTE_Ds6", "MIDI_NOTE_Eb6" } },
	{  88, { "MIDI_NOTE_E6" } },
	{  89, { "MIDI_NOTE_F6" } },
	{  90, { "MIDI_NOTE_Fs6", "MIDI_NOTE_Gb6" } },
	{  91, { "MIDI_NOTE_G6" } },
	{  92, { "MIDI_NOTE_Gs6", "MIDI_NOTE_Ab6" } },
	{  93, { "MIDI_NOTE_A6" } },
	{  94, { "MIDI_NOTE_As6", "MIDI_NOTE_Bb6" } },
	{  95, { "MIDI_NOTE_B6" } },
	{  96, { "MIDI_NOTE_C6" } },
	{  97, { "MIDI_NOTE_Cs6", "MIDI_NOTE_Db6" } },
	{  98, { "MIDI_NOTE_D6" } },
	{  99, { "MIDI_NOTE_Ds6", "MIDI_NOTE_Eb6" } },
	{ 100, { "MIDI_NOTE_E6" } },
	{ 101, { "MIDI_NOTE_F6" } },
	{ 102, { "MIDI_NOTE_Fs6", "MIDI_NOTE_Gb6" } },
	{ 103, { "MIDI_NOTE_G6" } },
	{ 104, { "MIDI_NOTE_Gs6", "MIDI_NOTE_Ab6" } },
	{ 105, { "MIDI_NOTE_A6" } },
	{ 106, { "MIDI_NOTE_As6", "MIDI_NOTE_Bb6" } },
	{ 107, { "MIDI_NOTE_B6" } },
	{ 108, { "MIDI_NOTE_C7" } },
	{ 109, { "MIDI_NOTE_Cs7", "MIDI_NOTE_Db7" } },
	{ 110, { "MIDI_NOTE_D7" } },
	{ 111, { "MIDI_NOTE_Ds7", "MIDI_NOTE_Eb7" } },
	{ 112, { "MIDI_NOTE_E7" } },
	{ 113, { "MIDI_NOTE_F7" } },
	{ 114, { "MIDI_NOTE_Fs7", "MIDI_NOTE_Gb7" } },
	{ 115, { "MIDI_NOTE_G7" } },
	{ 116, { "MIDI_NOTE_Gs7", "MIDI_NOTE_Ab7" } },
	{ 117, { "MIDI_NOTE_A7" } },
	{ 118, { "MIDI_NOTE_As7", "MIDI_NOTE_Bb7" } },
	{ 119, { "MIDI_NOTE_B7" } },
	{ 120, { "MIDI_NOTE_C8" } },
	{ 121, { "MIDI_NOTE_Cs8", "MIDI_NOTE_Db8" } },
	{ 122, { "MIDI_NOTE_D8" } },
	{ 123, { "MIDI_NOTE_Ds8", "MIDI_NOTE_Eb8" } },
	{ 124, { "MIDI_NOTE_E8" } },
	{ 125, { "MIDI_NOTE_F8" } },
	{ 126, { "MIDI_NOTE_Fs8", "MIDI_NOTE_Gb8" } },
	{ 127, { "MIDI_NOTE_G8" } },
};

const map<const unsigned char, const vector<const char*>> messageNames
{
	{  120, { "MIDI_MESSAGE_ALL_SOUND_OFF" } },
	{  121, { "MIDI_MESSAGE_RESET_ALL_CONTROLLERS" } },
	{  122, { "MIDI_MESSAGE_LOCAL_CONTROL" } },
	{  123, { "MIDI_MESSAGE_ALL_NOTES_OFF" } },
	{  124, { "MIDI_MESSAGE_OMNI_OFF" } },
	{  125, { "MIDI_MESSAGE_OMNI_ON" } },
	{  126, { "MIDI_MESSAGE_MONO_ON", "MIDI_MESSAGE_POLY_OFF" } },
	{  127, { "MIDI_MESSAGE_POLY_ON", "MIDI_MESSAGE_MONO_OFF" } },
	{ 0x80, { "MIDI_MESSAGE_NOTE_OFF" } },
	{ 0x90, { "MIDI_MESSAGE_NOTE_ON" } },
	{ 0xA0, { "MIDI_MESSAGE_KEY_PRESSURE" } },
	{ 0xB0, { "MIDI_MESSAGE_CONTROL_CHANGE" } },
	{ 0xC0, { "MIDI_MESSAGE_PROGRAM_CHANGE" } },
	{ 0xD0, { "MIDI_MESSAGE_CHANNEL_PRESSURE" } },
	{ 0xE0, { "MIDI_MESSAGE_PITCH_BEND" } },
	{ 0xF0, { "MIDI_MESSAGE_SYSTEM_EXCLUSIVE" } },
	{ 0xF1, { "MIDI_MESSAGE_TIME_CODE_QUARTER_FRAME" } },
	{ 0xF2, { "MIDI_MESSAGE_SONG_POSITION" } },
	{ 0xF3, { "MIDI_MESSAGE_SONG_SELECT" } },
	{ 0xF6, { "MIDI_MESSAGE_TUNE_REQUEST" } },
	{ 0xF7, { "MIDI_MESSAGE_END_OF_EXCLUSIVE" } },
	{ 0xF8, { "MIDI_MESSAGE_TIMING_CLOCK" } },
	{ 0XFA, { "MIDI_MESSAGE_START" } },
	{ 0XFB, { "MIDI_MESSAGE_CONTINUE" } },
	{ 0XFC, { "MIDI_MESSAGE_STOP" } },
	{ 0XFE, { "MIDI_MESSAGE_ACTIVE_SENSING" } },
	{ 0XFF, { "MIDI_MESSAGE_SYSTEM_RESET" } },
};

const map<const unsigned char, const vector<const char*>> messageTypeNames
{
	{ 0, { "MIDI_MESSAGE_TYPE_CHANNEL_MODE" } },
	{ 1, { "MIDI_MESSAGE_TYPE_CHANNEL_VOICE" } },
	{ 2, { "MIDI_MESSAGE_TYPE_SYSTEM_EXCLUSIVE" } },
	{ 3, { "MIDI_MESSAGE_TYPE_SYSTEM_COMMON" } },
	{ 4, { "MIDI_MESSAGE_TYPE_SYSTEM_REALTIME" } },
};

const map<const unsigned char, const vector<const char*>> controlNames
{
	{   0, { "MIDI_CONTROL_BANK_SELECT" } },
	{   1, { "MIDI_CONTROL_MODULATION_WHEEL" } },
	{   2, { "MIDI_CONTROL_BREATH_CONTROLLER" } },
	{   4, { "MIDI_CONTROL_FOOT_CONTROLLER" } },
	{   5, { "MIDI_CONTROL_PORTAMENTO_TIME" } },
	{   6, { "MIDI_CONTROL_DATA_ENTRY" } },
	{   7, { "MIDI_CONTROL_CHANNEL_VOLUME" } },
	{   8, { "MIDI_CONTROL_BALANCE" } },
	{  10, { "MIDI_CONTROL_PAN" } },
	{  11, { "MIDI_CONTROL_EXPRESSION_CONTROLLER" } },
	{  12, { "MIDI_CONTROL_EFFECT_CONTROL_1" } },
	{  13, { "MIDI_CONTROL_EFFECT_CONTROL_2" } },
	{  16, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_1" } },
	{  17, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_2" } },
	{  18, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_3" } },
	{  19, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_4" } },
	{  64, { "MIDI_CONTROL_DAMPER_PEDAL" } },
	{  65, { "MIDI_CONTROL_PORTAMENTO_TOGGLE" } },
	{  66, { "MIDI_CONTROL_SOSTENUTO" } },
	{  67, { "MIDI_CONTROL_SOFT_PEDAL" } },
	{  68, { "MIDI_CONTROL_LEGATO_FOOTSWITCH" } },
	{  69, { "MIDI_CONTROL_HOLD_2" } },
	{  70, { "MIDI_CONTROL_SOUND_CONTROLLER_1", "MIDI_CONTROL_SOUND_VARIATION" } },
	{  71, { "MIDI_CONTROL_SOUND_CONTROLLER_2", "MIDI_CONTROL_TIMBRE_INTENSITY" } },
	{  72, { "MIDI_CONTROL_SOUND_CONTROLLER_3", "MIDI_CONTROL_RELEASE_TIME" } },
	{  73, { "MIDI_CONTROL_SOUND_CONTROLLER_4", "MIDI_CONTROL_ATTACK_TIME" } },
	{  74, { "MIDI_CONTROL_SOUND_CONTROLLER_5", "MIDI_CONTROL_BRIGHTNESS" } },
	{  75, { "MIDI_CONTROL_SOUND_CONTROLLER_6" } },
	{  76, { "MIDI_CONTROL_SOUND_CONTROLLER_7" } },
	{  77, { "MIDI_CONTROL_SOUND_CONTROLLER_8" } },
	{  78, { "MIDI_CONTROL_SOUND_CONTROLLER_9" } },
	{  79, { "MIDI_CONTROL_SOUND_CONTROLLER_10" } },
	{  80, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_5" } },
	{  81, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_6" } },
	{  82, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_7" } },
	{  83, { "MIDI_CONTROL_GENERAL_PURPOSE_CONTROLLER_8" } },
	{  84, { "MIDI_CONTROL_PORTAMENTO_CONTROL" } },
	{  91, { "MIDI_CONTROL_EFFECTS_1_DEPTH" "MIDI_CONTROL_EXTERNAL_EFFECTS_DEPTH" } },
	{  92, { "MIDI_CONTROL_EFFECTS_2_DEPTH" "MIDI_CONTROL_TREMOLO_DEPTH" } },
	{  93, { "MIDI_CONTROL_EFFECTS_3_DEPTH" "MIDI_CONTROL_CHORUS_DEPTH" } },
	{  94, { "MIDI_CONTROL_EFFECTS_4_DEPTH" "MIDI_CONTROL_CELESTE_DEPTH" } },
	{  95, { "MIDI_CONTROL_EFFECTS_5_DEPTH" "MIDI_CONTROL_PHASER_DEPTH" } },
	{  96, { "MIDI_CONTROL_DATA_INCREMENT" } },
	{  97, { "MIDI_CONTROL_DATA_DECREMENT" } },
	{  99, { "MIDI_CONTROL_NON_REGISTERED_PARAMETER_NUMBER" } },
	{ 101, { "MIDI_CONTROL_REGISTERED_PARAMETER_NUMBER" } },
};

const map<const unsigned char, const vector<const char*>> controlBitSignificanceNames
{
	{ 0, { "MIDI_CONTROL_BIT_SIGNIFICANCE_NONE" } },
	{ 1, { "MIDI_CONTROL_BIT_SIGNIFICANCE_MOST" } },
	{ 2, { "MIDI_CONTROL_BIT_SIGNIFICANCE_LEAST" } },
};

void inputCallback(double deltaTime, vector<unsigned char>* messagePointer, void* userData)
{
	if (!messagePointer->empty())
	{
		vector<unsigned char> message = *messagePointer;

		if (message[0] >= 0x90 && message[0] < 0xA0 && message[2] < 1)
		{
			message[0] = message[0] - 16;
		}

		inputMessages.push(message);
	}
}

LUA_FUNCTION(inputThink)
{
	if (!inputMessages.empty())
	{
		LUA->PushSpecial(SPECIAL_GLOB);
			LUA->GetField(-1, "hook");
				while (!inputMessages.empty())
				{
					const auto message = inputMessages.front();

					inputMessages.pop();

					const auto& size = (int)message.size();

					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldReceiveMidiInputMessage");

						for (const auto& byte : message)
						{
							LUA->PushNumber((double)byte);
						}
					LUA->Call(size + 1, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							LUA->GetField(-2, "Run");
								LUA->PushString("OnMidiInputMessageReceived");

								for (const auto& byte : message)
								{
									LUA->PushNumber((double)byte);
								}
							LUA->Call(size + 1, 0);
						}
					LUA->Pop();
				}
			LUA->Pop();
		LUA->Pop();
	}

	return 0;
}

LUA_FUNCTION(openInputPort)
{
	const auto& port = (unsigned int)LUA->CheckNumber(1);

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			try
			{
				if (inputApi->isPortOpen())
				{
					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldCloseMidiInputPort");
					LUA->Call(1, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							inputApi->closePort();

							if (!inputApi->isPortOpen())
							{
								LUA->GetField(-2, "Run");
									LUA->PushString("OnMidiInputPortClosed");
								LUA->Call(1, 0);
							}
						}
					LUA->Pop();
				}

				if (!inputApi->isPortOpen())
				{
					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldOpenMidiInputPort");
						LUA->PushNumber((double)port);
					LUA->Call(2, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							inputApi->openPort(port);

							if (inputApi->isPortOpen())
							{
								LUA->GetField(-2, "Run");
									LUA->PushString("OnMidiInputPortOpened");
									LUA->PushNumber((double)port);
								LUA->Call(2, 0);

								LUA->PushBool(true);

								return 1;
							}
						}
					LUA->Pop();
				}
			}
			catch (const RtMidiError& error)
			{
				LUA->ThrowError(error.getMessage().c_str());
			}
		LUA->Pop();
	LUA->Pop();

	LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION(closeInputPort)
{
	try
	{
		if (inputApi->isPortOpen())
		{
			LUA->PushSpecial(SPECIAL_GLOB);
				LUA->GetField(-1, "hook");
					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldCloseMidiInputPort");
					LUA->Call(1, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							inputApi->closePort();

							if (!inputApi->isPortOpen())
							{
								LUA->GetField(-2, "Run");
									LUA->PushString("OnMidiInputPortClosed");
								LUA->Call(1, 0);

								LUA->PushBool(true);

								return 1;
							}
						}
					LUA->Pop();
				LUA->Pop();
			LUA->Pop();
		}
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION(isInputPortOpen)
{
	try
	{
		LUA->PushBool(inputApi->isPortOpen());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	return 1;
}

LUA_FUNCTION(getInputPortName)
{
	try
	{
		LUA->PushString(inputApi->getPortName((unsigned int)LUA->CheckNumber(1)).c_str());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	return 1;
}

LUA_FUNCTION(getInputPortCount)
{
	try
	{
		LUA->PushNumber((double)inputApi->getPortCount());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	return 1;
}

LUA_FUNCTION(openOutputPort)
{
	const auto& port = (unsigned int)LUA->CheckNumber(1);

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			try
			{
				if (outputApi->isPortOpen())
				{
					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldCloseMidiOutputPort");
					LUA->Call(1, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							outputApi->closePort();

							if (!outputApi->isPortOpen())
							{
								LUA->GetField(-2, "Run");
									LUA->PushString("OnMidiOutputPortClosed");
								LUA->Call(1, 0);
							}
						}
					LUA->Pop();
				}

				if (!outputApi->isPortOpen())
				{
					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldOpenMidiOutputPort");
						LUA->PushNumber((double)port);
					LUA->Call(2, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							outputApi->openPort(port);

							if (outputApi->isPortOpen())
							{
								LUA->GetField(-2, "Run");
									LUA->PushString("OnMidiOutputPortOpened");
									LUA->PushNumber((double)port);
								LUA->Call(2, 0);

								LUA->PushBool(true);

								return 1;
							}
						}
					LUA->Pop();
				}
			}
			catch (const RtMidiError& error)
			{
				LUA->ThrowError(error.getMessage().c_str());
			}
		LUA->Pop();
	LUA->Pop();

	LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION(closeOutputPort)
{
	try
	{
		if (outputApi->isPortOpen())
		{
			LUA->PushSpecial(SPECIAL_GLOB);
				LUA->GetField(-1, "hook");
					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldCloseMidiOutputPort");
					LUA->Call(1, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							outputApi->closePort();

							if (!outputApi->isPortOpen())
							{
								LUA->GetField(-2, "Run");
									LUA->PushString("OnMidiOutputPortClosed");
								LUA->Call(1, 0);

								LUA->PushBool(true);

								return 1;
							}
						}
					LUA->Pop();
				LUA->Pop();
			LUA->Pop();
		}
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION(isOutputPortOpen)
{
	try
	{
		LUA->PushBool(outputApi->isPortOpen());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	return 1;
}

LUA_FUNCTION(getOutputPortName)
{
	try
	{
		LUA->PushString(outputApi->getPortName((unsigned int)LUA->CheckNumber(1)).c_str());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	return 1;
}

LUA_FUNCTION(getOutputPortCount)
{
	try
	{
		LUA->PushNumber((double)outputApi->getPortCount());
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	return 1;
}

LUA_FUNCTION(sendOutputMessage)
{
	vector<unsigned char> message = { (unsigned char)LUA->CheckNumber(1) };

	for (int i = 2; i <= LUA->Top(); i++)
	{
		message.push_back((unsigned char)LUA->CheckNumber(i));
	}

	const auto& size = (int)message.size();

	try
	{
		if (outputApi->isPortOpen())
		{
			LUA->PushSpecial(SPECIAL_GLOB);
				LUA->GetField(-1, "hook");
					LUA->GetField(-1, "Run");
						LUA->PushString("ShouldSendMidiOutputMessage");

						for (const auto& byte : message)
						{
							LUA->PushNumber((double)byte);
						}
					LUA->Call(size + 1, 1);
						if (LUA->IsType(-1, Type::Nil) || LUA->GetBool())
						{
							outputApi->sendMessage(&message);

							LUA->GetField(-2, "Run");
								LUA->PushString("OnMidiOutputMessageSent");

								for (const auto& byte : message)
								{
									LUA->PushNumber((double)byte);
								}
							LUA->Call(size + 1, 0);

							LUA->PushBool(true);

							return 1;
						}
					LUA->Pop();
				LUA->Pop();
			LUA->Pop();
		}
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION(getNoteName)
{
	const auto& note = (unsigned char)LUA->CheckNumber(1);

	if (noteNames.count(note))
	{
		const auto& names = noteNames.at(note);

		for (const auto& name : names)
		{
			LUA->PushString(name);
		}

		return (int)names.size();
	}
	else
	{
		LUA->PushString("MIDI_NOTE_INVALID");

		return 1;
	}
}

LUA_FUNCTION(getMessageName)
{
	const auto& message = (unsigned char)LUA->CheckNumber(1);

	if (messageNames.count(message))
	{
		const auto& names = messageNames.at(message);

		for (const auto& name : names)
		{
			LUA->PushString(name);
		}

		return (int)names.size();
	}
	else
	{
		if (message >= 0x80 && message < 0xF0)
		{
			const auto& names = messageNames.at(0x80 + (unsigned char)floor(((double)message - 0x80) / 16) * 16);

			for (const auto& name : names)
			{
				LUA->PushString(name);
			}

			return (int)names.size();
		}
		else
		{
			LUA->PushString("MIDI_MESSAGE_INVALID");

			return 1;
		}
	}
}

LUA_FUNCTION(getMessageType)
{
	const auto& message = (unsigned char)LUA->CheckNumber(1);

	if (messageNames.count(message))
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

LUA_FUNCTION(getMessageChannel)
{
	const auto& message = (unsigned char)LUA->CheckNumber(1);

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

LUA_FUNCTION(getMessageTypeName)
{
	const auto& type = (unsigned char)LUA->CheckNumber(1);

	if (messageTypeNames.count(type))
	{
		const auto& names = messageTypeNames.at(type);

		for (const auto& name : names)
		{
			LUA->PushString(name);
		}

		return (int)names.size();
	}
	else
	{
		LUA->PushString("MIDI_MESSAGE_TYPE_INVALID");

		return 1;
	}
}

LUA_FUNCTION(getControlName)
{
	const auto& control = (unsigned char)LUA->CheckNumber(1);

	if (controlNames.count(control))
	{
		const auto& names = controlNames.at(control);

		for (const auto& name : names)
		{
			LUA->PushString(name);
		}

		return (int)names.size();
	}
	else
	{
		if (control == 98 || control == 100)
		{
			const auto& names = controlNames.at(control + 1);

			for (const auto& name : names)
			{
				LUA->PushString(name);
			}

			return (int)names.size();
		}
		else if (control >= 32 && control < 64)
		{
			if (controlNames.count(control - 32))
			{
				const auto& names = controlNames.at(control - 32);

				for (const auto& name : names)
				{
					LUA->PushString(name);
				}

				return (int)names.size();
			}
			else
			{
				LUA->PushString("MIDI_CONTROL_INVALID");

				return 1;
			}
		}
		else
		{
			LUA->PushString("MIDI_CONTROL_INVALID");

			return 1;
		}
	}
}

LUA_FUNCTION(getControlBitSignificance)
{
	const auto& control = (unsigned char)LUA->CheckNumber(1);

	if (controlNames.count(control))
	{
		if (control >= 0 && control < 32)
		{
			LUA->PushNumber(1);
		}
		else if (control >= 70 && control < 85)
		{
			LUA->PushNumber(2);
		}
		else if (control == 99 || control == 100)
		{
			LUA->PushNumber(1);
		}
		else
		{
			LUA->PushNumber(0);
		}
	}
	else
	{
		if (control >= 32 && control < 64)
		{
			if (controlNames.count(control - 32))
			{
				LUA->PushNumber(2);
			}
			else
			{
				LUA->PushNumber(-1);
			}
		}
		else if (control == 98 || control == 100)
		{
			LUA->PushNumber(2);
		}
		else
		{
			LUA->PushNumber(-1);
		}
	}

	return 1;
}

LUA_FUNCTION(getControlBitSignificanceName)
{
	const auto& bitSignificance = (unsigned char)LUA->CheckNumber(1);

	if (controlBitSignificanceNames.count(bitSignificance))
	{
		const auto& names = controlBitSignificanceNames.at(bitSignificance);

		for (const auto& name : names)
		{
			LUA->PushString(name);
		}

		return (int)names.size();
	}
	else
	{
		LUA->PushString("MIDI_CONTROL_BIT_SIGNIFICANCE_INVALID");

		return 1;
	}
}

GMOD_MODULE_OPEN()
{
	try
	{
		inputApi  = new RtMidiIn();
		outputApi = new RtMidiOut();
		
		inputApi->setCallback(&inputCallback);
	}
	catch (const RtMidiError& error)
	{
		LUA->ThrowError(error.getMessage().c_str());
	}

	LUA->PushSpecial(SPECIAL_GLOB);
		for (const auto& pair : noteNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber(pair.first);
				LUA->SetField(-2, name);
			}

		}

		for (const auto& pair : messageNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber(pair.first);
				LUA->SetField(-2, name);
			}

		}

		for (const auto& pair : messageTypeNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber(pair.first);
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : controlNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber(pair.first);
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : controlBitSignificanceNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNumber(pair.first);
				LUA->SetField(-2, name);
			}
		}

		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_NOTE_INVALID");
		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_MESSAGE_INVALID");
		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_CONTROL_INVALID");
		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_INVALID");
		LUA->PushNumber(-1);
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_INVALID");

		LUA->PushNumber(0);
		LUA->SetField(-2, "MIDI_NOTE_FIRST");
		LUA->PushNumber(127);
		LUA->SetField(-2, "MIDI_NOTE_LAST");
		LUA->PushNumber(128);
		LUA->SetField(-2, "MIDI_NOTE_COUNT");

		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Add");
				LUA->PushString("Think");
				LUA->PushString("MidiInputThink");
				LUA->PushCFunction(&inputThink);
			LUA->Call(3, 0);
		LUA->Pop();

		LUA->CreateTable();
			LUA->PushCFunction(&openInputPort);
			LUA->SetField(-2, "OpenInputPort");
			LUA->PushCFunction(&closeInputPort);
			LUA->SetField(-2, "CloseInputPort");
			LUA->PushCFunction(&isInputPortOpen);
			LUA->SetField(-2, "IsInputPortOpen");
			LUA->PushCFunction(&getInputPortName);
			LUA->SetField(-2, "GetInputPortName");
			LUA->PushCFunction(&getInputPortCount);
			LUA->SetField(-2, "GetInputPortCount");
			LUA->PushCFunction(&openOutputPort);
			LUA->SetField(-2, "OpenOutputPort");
			LUA->PushCFunction(&closeOutputPort);
			LUA->SetField(-2, "CloseOutputPort");
			LUA->PushCFunction(&isOutputPortOpen);
			LUA->SetField(-2, "IsOutputPortOpen");
			LUA->PushCFunction(&getOutputPortName);
			LUA->SetField(-2, "GetOutputPortName");
			LUA->PushCFunction(&getOutputPortCount);
			LUA->SetField(-2, "GetOutputPortCount");
			LUA->PushCFunction(&sendOutputMessage);
			LUA->SetField(-2, "SendOutputMessage");

			LUA->PushCFunction(&getNoteName);
			LUA->SetField(-2, "GetNoteName");
			LUA->PushCFunction(&getMessageName);
			LUA->SetField(-2, "GetMessageName");
			LUA->PushCFunction(&getMessageType);
			LUA->SetField(-2, "GetMessageType");
			LUA->PushCFunction(&getMessageChannel);
			LUA->SetField(-2, "GetMessageChannel");
			LUA->PushCFunction(&getMessageTypeName);
			LUA->SetField(-2, "GetMessageTypeName");
			LUA->PushCFunction(&getControlName);
			LUA->SetField(-2, "GetControlName");
			LUA->PushCFunction(&getControlBitSignificance);
			LUA->SetField(-2, "GetControlBitSignificance");
			LUA->PushCFunction(&getControlBitSignificanceName);
			LUA->SetField(-2, "GetControlBitSignificanceName");
		LUA->SetField(-2, "midi");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	delete inputApi;
	delete outputApi;

	LUA->PushSpecial(SPECIAL_GLOB);
		for (const auto& pair : noteNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : messageNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : messageTypeNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : controlNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		for (const auto& pair : controlBitSignificanceNames)
		{
			for (const auto& name : pair.second)
			{
				LUA->PushNil();
				LUA->SetField(-2, name);
			}
		}

		LUA->PushNil();
		LUA->SetField(-2, "MIDI_NOTE_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_MESSAGE_TYPE_INVALID");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_CONTROL_BIT_SIGNIFICANCE_INVALID");

		LUA->PushNil();
		LUA->SetField(-2, "MIDI_NOTE_FIRST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_NOTE_LAST");
		LUA->PushNil();
		LUA->SetField(-2, "MIDI_NOTE_COUNT");

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