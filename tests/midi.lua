require "midi"

-- INPUT

hook.Add("ShouldReceiveMidiInputMessage", "midi", function(message, ...)
	print("ShouldReceiveMidiInputMessage", message .. " (" .. midi.GetMessageName(message) .. ")", ...)
end)

hook.Add("OnMidiInputMessageReceived", "midi", function(message, ...)
	print("OnMidiInputMessageReceived", message .. " (" .. midi.GetMessageName(message) .. ")", ...)
end)

hook.Add("ShouldOpenMidiInputPort", "midi", function(...)
	print("ShouldOpenMidiInputPort", ...)
end)

hook.Add("ShouldCloseMidiInputPort", "midi", function(...)
	print("ShouldCloseMidiInputPort", ...)
end)

hook.Add("OnMidiInputPortOpened", "midi", function(...)
	print("OnMidiInputPortOpened", ...)
end)

hook.Add("OnMidiInputPortClosed", "midi", function(...)
	print("OnMidiInputPortClosed", ...)
end)

-- OUTPUT

hook.Add("ShouldSendMidiOutputMessage", "midi", function(message, ...)
	print("ShouldSendMidiOutputMessage", message .. " (" .. midi.GetMessageName(message) .. ")", ...)
end)

hook.Add("OnMidiOutputMessageSent", "midi", function(message, ...)
	print("OnMidiOutputMessageSent", message .. " (" .. midi.GetMessageName(message) .. ")", ...)
end)

hook.Add("ShouldOpenMidiOutputPort", "midi", function(...)
	print("ShouldOpenMidiOutputPort", ...)
end)

hook.Add("ShouldCloseMidiOutputPort", "midi", function(...)
	print("ShouldCloseMidiOutputPort", ...)
end)

hook.Add("OnMidiOutputPortOpened", "midi", function(...)
	print("OnMidiOutputPortOpened", ...)
end)

hook.Add("OnMidiOutputPortClosed", "midi", function(...)
	print("OnMidiOutputPortClosed", ...)
end)

-- PORT

concommand.Add("midi_open_input_port", function(player, command, arguments)
	print(midi.OpenInputPort(unpack(arguments)))
end)

concommand.Add("midi_close_input_port", function(player, command, arguments)
	print(midi.CloseInputPort())
end)

concommand.Add("midi_list_input_ports", function()
	for port = 0, midi.GetInputPortCount() - 1 do
		print(port, midi.GetInputPortName(port))
	end
end)

concommand.Add("midi_is_input_port_open", function()
	print(midi.IsInputPortOpen())
end)

concommand.Add("midi_open_output_port", function(player, command, arguments)
	print(midi.OpenOutputPort(unpack(arguments)))
end)

concommand.Add("midi_close_output_port", function(player, command, arguments)
	print(midi.CloseOutputPort())
end)

concommand.Add("midi_send_output_message", function(player, command, arguments)
	print(midi.SendOutputMessage(unpack(arguments)))
end)

concommand.Add("midi_list_output_ports", function()
	for port = 0, midi.GetOutputPortCount() - 1 do
		print(port, midi.GetOutputPortName(port))
	end
end)

concommand.Add("midi_is_output_port_open", function()
	print(midi.IsOutputPortOpen())
end)