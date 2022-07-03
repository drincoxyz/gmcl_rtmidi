require "rtmidi"

hook.Add("OnMIDIMessageSent", "test", function(...)
	print("OnMIDIMessageSent", ...)
end)

hook.Add("OnMIDIMessageReceived", "test", function(...)
	print("OnMIDIMessageReceived", ...)
end)

hook.Add("OnMIDIInputPortOpened", "test", function(...)
	print("OnMIDIInputPortOpened", ...)
end)

hook.Add("OnMIDIInputPortClosed", "test", function(...)
	print("OnMIDIInputPortClosed", ...)
end)

hook.Add("OnMIDIOutputPortOpened", "test", function(...)
	print("OnMIDIOutputPortOpened", ...)
end)

hook.Add("OnMIDIOutputPortClosed", "test", function(...)
	print("OnMIDIOutputPortClosed", ...)
end)

hook.Add("ShouldSendMIDIMessage", "test", function(...)
	print("ShouldSendMIDIMessage", ...)
end)

hook.Add("ShouldReceiveMIDIMessage", "test", function(...)
	print("ShouldReceiveMIDIMessage", ...)
end)

hook.Add("ShouldOpenMIDIInputPort", "test", function(...)
	print("ShouldOpenMIDIInputPort", ...)
end)

hook.Add("ShouldCloseMIDIInputPort", "test", function(...)
	print("ShouldCloseMIDIInputPort", ...)
end)

hook.Add("ShouldOpenMIDIOutputPort", "ttest", function(...)
	print("ShouldOpenMIDIOutputPort", ...)
end)

hook.Add("ShouldCloseMIDIOutputPort", "test", function(...)
	print("ShouldCloseMIDIOutputPort", ...)
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