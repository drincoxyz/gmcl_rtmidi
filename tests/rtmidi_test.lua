require "rtmidi"

hook.Add("OnMidiMessageSent", "test", function(...)
	print("OnMidiMessageSent", ...)
end)

hook.Add("OnMidiMessageReceived", "test", function(...)
	print("OnMidiMessageReceived", ...)
end)

hook.Add("OnMidiInputPortOpened", "test", function(...)
	print("OnMidiInputPortOpened", ...)
end)

hook.Add("OnMidiInputPortClosed", "test", function(...)
	print("OnMidiInputPortClosed", ...)
end)

hook.Add("OnMidiOutputPortOpened", "test", function(...)
	print("OnMidiOutputPortOpened", ...)
end)

hook.Add("OnMidiOutputPortClosed", "test", function(...)
	print("OnMidiOutputPortClosed", ...)
end)

hook.Add("ShouldSendMidiMessage", "test", function(...)
	print("ShouldSendMidiMessage", ...)
end)

hook.Add("ShouldReceiveMidiMessage", "test", function(...)
	print("ShouldReceiveMidiMessage", ...)
end)

hook.Add("ShouldOpenMidiInputPort", "test", function(...)
	print("ShouldOpenMidiInputPort", ...)
end)

hook.Add("ShouldCloseMidiInputPort", "test", function(...)
	print("ShouldCloseMidiInputPort", ...)
end)

hook.Add("ShouldOpenMidiOutputPort", "test", function(...)
	print("ShouldOpenMidiOutputPort", ...)
end)

hook.Add("ShouldCloseMidiOutputPort", "test", function(...)
	print("ShouldCloseMidiOutputPort", ...)
end)

concommand.Add("rtmidi_send_message", function(player, command, arguments)
	print(rtmidi.SendMessage(unpack(arguments)))
end)

concommand.Add("rtmidi_open_input_port", function(player, command, arguments)
	print(rtmidi.OpenInputPort(unpack(arguments)))
end)

concommand.Add("rtmidi_close_input_port", function(player, command, arguments)
	print(rtmidi.CloseInputPort(unpack(arguments)))
end)

concommand.Add("rtmidi_is_input_port_open", function(player, command, arguments)
	print(rtmidi.IsInputPortOpen(unpack(arguments)))
end)

concommand.Add("rtmidi_get_input_port_name", function(player, command, arguments)
	print(rtmidi.GetInputPortName(unpack(arguments)))
end)

concommand.Add("rtmidi_get_input_port_count", function(player, command, arguments)
	print(rtmidi.GetInputPortCount(unpack(arguments)))
end)

concommand.Add("rtmidi_open_output_port", function(player, command, arguments)
	print(rtmidi.OpenOutputPort(unpack(arguments)))
end)

concommand.Add("rtmidi_close_output_port", function(player, command, arguments)
	print(rtmidi.CloseOutputPort(unpack(arguments)))
end)

concommand.Add("rtmidi_is_output_port_open", function(player, command, arguments)
	print(rtmidi.IsOutputPortOpen(unpack(arguments)))
end)

concommand.Add("rtmidi_get_output_port_name", function(player, command, arguments)
	print(rtmidi.GetOutputPortName(unpack(arguments)))
end)

concommand.Add("rtmidi_get_output_port_count", function(player, command, arguments)
	print(rtmidi.GetOutputPortCount(unpack(arguments)))
end)