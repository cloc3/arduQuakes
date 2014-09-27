#!/bin/bash

handle_req()
{
	read req file proto
	# la risposta deve contenere un header per indurre il browser di includere il contenuto della risposta in una pagina web.
	echo -e "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nAccess-Control-Allow-Origin: *\r\nKeep-Alive: timeout=0, max=100\r\nConnection: keep-alive\r\n\r\n"
	head -n4 <&5 & echo n=4 >&5 # Prima leggere e poi scrivere
}

exec 5<>/dev/arduino  # apre la porta seriale in lettura e scrittura
sleep 3
typeset -fx handle_req
while  true;do nc -l -p 8000 -e "handle_req" -q0;done # apre il servizio web
exec 5<&- # questo è il modo corretto di chiudere la porta seriale. Questo esempio è acerbo, e nasconde il comando dietro un loop infinito. Da correggere.
