function end_on_keypress(src, event)
%Event Funktion zum verlassen einer Endlosschleife

global stop_flag;
if(event.Character=='q')
    stop_flag=0;
end
end