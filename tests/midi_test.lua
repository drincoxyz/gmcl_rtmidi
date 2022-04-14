require "midi"

--------------------------------------------------------------------------------
-- INPUT
--------------------------------------------------------------------------------

hook.Add("ShouldOpenMidiInputPort", "midi_test", function(...)
	print("ShouldOpenMidiInputPort", ...)
end)

hook.Add("ShouldCloseMidiInputPort", "midi_test", function(...)
	print("ShouldCloseMidiInputPort", ...)
end)

hook.Add("OnMidiInputPortOpened", "midi_test", function(...)
	print("OnMidiInputPortOpened", ...)
end)

hook.Add("OnMidiInputPortClosed", "midi_test", function(...)
	print("OnMidiInputPortClosed", ...)
end)

concommand.Add("midi_test_open_input_port", function(player, command, arguments)
	print(midi.OpenInputPort(unpack(arguments)))
end)

concommand.Add("midi_test_close_input_port", function(player, command, arguments)
	print(midi.CloseInputPort(unpack(arguments)))
end)

concommand.Add("midi_test_is_input_port_open", function(player, command, arguments)
	print(midi.IsInputPortOpen(unpack(arguments)))
end)

concommand.Add("midi_test_get_input_port_name", function(player, command, arguments)
	print(midi.GetInputPortName(unpack(arguments)))
end)

concommand.Add("midi_test_get_input_port_count", function(player, command, arguments)
	print(midi.GetInputPortCount(unpack(arguments)))
end)

--------------------------------------------------------------------------------
-- OUTPUT
--------------------------------------------------------------------------------

hook.Add("ShouldOpenMidiOutputPort", "midi_test", function(...)
	print("ShouldOpenMidiOutputPort", ...)
end)

hook.Add("ShouldCloseMidiOutputPort", "midi_test", function(...)
	print("ShouldCloseMidiOutputPort", ...)
end)

hook.Add("OnMidiOutputPortOpened", "midi_test", function(...)
	print("OnMidiOutputPortOpened", ...)
end)

hook.Add("OnMidiOutputPortClosed", "midi_test", function(...)
	print("OnMidiOutputPortClosed", ...)
end)

concommand.Add("midi_test_open_output_port", function(player, command, arguments)
	print(midi.OpenOutputPort(unpack(arguments)))
end)

concommand.Add("midi_test_close_output_port", function(player, command, arguments)
	print(midi.CloseOutputPort(unpack(arguments)))
end)

concommand.Add("midi_test_is_output_port_open", function(player, command, arguments)
	print(midi.IsOutputPortOpen(unpack(arguments)))
end)

concommand.Add("midi_test_get_output_port_name", function(player, command, arguments)
	print(midi.GetOutputPortName(unpack(arguments)))
end)

concommand.Add("midi_test_get_output_port_count", function(player, command, arguments)
	print(midi.GetOutputPortCount(unpack(arguments)))
end)

--------------------------------------------------------------------------------
-- MESSAGE
--------------------------------------------------------------------------------

hook.Add("ShouldSendMidiMessage", "midi_test", function(...)
	print("ShouldSendMidiMessage", ...)
end)

hook.Add("ShouldReceiveMidiMessage", "midi_test", function(...)
	print("ShouldReceiveMidiMessage", ...)
end)

hook.Add("OnMidiMessageSent", "midi_test", function(...)
	print("OnMidiMessageSent", ...)
end)

hook.Add("OnMidiMessageReceived", "midi_test", function(...)
	print("OnMidiMessageReceived", ...)
end)

concommand.Add("midi_test_send_message", function(player, command, arguments)
	print(midi.SendMessage(unpack(arguments)))
end)

concommand.Add("midi_test_get_message_name", function(player, command, arguments)
	print(midi.GetMessageName(unpack(arguments)))
end)

concommand.Add("midi_test_get_message_type", function(player, command, arguments)
	print(midi.GetMessageType(unpack(arguments)))
end)

concommand.Add("midi_test_get_message_channel", function(player, command, arguments)
	print(midi.GetMessageChannel(unpack(arguments)))
end)

concommand.Add("midi_test_get_message_type_name", function(player, command, arguments)
	print(midi.GetMessageTypeName(unpack(arguments)))
end)

--------------------------------------------------------------------------------
-- CONTROL
--------------------------------------------------------------------------------

concommand.Add("midi_test_get_control_name", function(player, command, arguments)
	print(midi.GetControlName(unpack(arguments)))
end)

concommand.Add("midi_test_get_control_bit_significance", function(player, command, arguments)
	print(midi.GetControlBitSignificance(unpack(arguments)))
end)

concommand.Add("midi_test_get_control_bit_significance_name", function(player, command, arguments)
	print(midi.GetControlBitSignificanceName(unpack(arguments)))
end)