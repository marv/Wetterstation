%LoadDataFromSerial.m
% TB 11.11.2014
% L�dt Daten von der seriellen Schnittstelle des PCs und speichert diese in
% Matlab ab.
close all, clear

%Zeige verf�gbare serielle Verbindungen.
instrhwinfo ('serial') 

disp('Das Programm muss durch den Druck der Taste q beendet werden.')
disp('Die ge�ffnete figure muss hierf�r das aktive Fenster sein.')
disp('Nur so wird die serielle Verbindung korrekt geschlossen.')

filename = 'Datei1.txt';
serialPort = 'COM10';
filename=inputs('Dateiname zum abspeichern der Daten eingeben.',filename);
serialPort=inputs('Serielle Verbindung ausw�hlen.',serialPort);
baud_rate=inputs('Baud Rate eingeben',9600);
%�ffne Figure, die figure l�uft in einem zweiten Thread, und erlaubt somit
%das reagieren auf den Tastendruck. Dieser figure wird die OnKeyPress
%Funktion zugeordnet um die Endlosschleife abzubrechen.
f=figure();
pause(1); % to give the figure window time to open
global stop_flag;
stop_flag=1;
set(f, 'KeyPressFcn', @end_on_keypress);


% W�hle entsprechende serielle Verbindung
s = serial(serialPort); 
% Baudrate und die Flusssteuerung setzen 
set(s,'BaudRate',baud_rate,'FlowControl','hardware');

%�ffne Serielle Verbindung
fopen(s);

%�ffne Datei, append, text-mode
fid = fopen(filename,'at');
if(fid<0)
    error('Fehler beim Datei erzeugen');
end

%
while(stop_flag)
%fscanf oder fgetl alle Daten abrufen
%Plotte Daten, sobald bekannt ist in welchem Format diese gesendet werden.
%SOlange nur den Rohstring abspeichern.
line = fgetl(s);
if ischar(line)
    disp(line);
    fprintf(fid,'%s\n',line);
end

pause(0.0001);
%fopen fwrite fclose Schreibe Daten in Datei. .csv
end
%Datei schlie�en
fclose(fid);
%Wichtig! Serielle Verbindung am Ende schliessen.
fclose(s);
clear s

close all
