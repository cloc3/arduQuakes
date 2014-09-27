//routines to parse input events

#include <Arduino.h>
#include <Ethernet.h>
#include <http.h>
#define MAXLINE 512
#define	 MAXL MAXCHARS -1

struct chValPairs stack[MAXPARAMS];
struct paramGet _cmdParser, *cmdParser = &_cmdParser;
struct strBuf _tr, *tr = &_tr;
struct strBuf _header, *header = &_header;
struct keyFind _command, *command = &_command;

char	headerContent[MAXLINE];
char	_strBuf[MAXLINE];

boolean digit2int( int cifra, int *n) {
	if ((cifra >= '0') && (cifra <= '9')) {
		*n = *n*10 + cifra -'0';
	} else {
		return false;
	}
	return true;
}

boolean readInteger(chValPairs *parametro) {
	int _n; int *n = &_n;
	boolean datoValido = true;
	char cifra;
	char *p = parametro->value;
	_n = 0;
	while ((cifra = *p++) != '\0' && datoValido) {
		digit2int(cifra, n);
	}
	if (datoValido) {
		parametro->nSensorReadings = *n;
		return true;
	}
	else return false;
}

strBuf *getline(char *inputText, int lim) { //reads inputText and store it in tr->buffer, one char at once
	char c;
	char *p;
	p=_strBuf;
	while (--lim>0 && (c = *inputText++) != '\n') *p++ = c;
	if (c == '\n') *p++ = '\n';
	*p='\0';
	tr->buffer = _strBuf;
	tr->l = p - _strBuf;
	return tr;
}

strBuf *serialEvent() {
	char c;
	static char *p;
	if (!tr->initialized) {
		tr = getline((char*)"?",2); //adds a starting "?" symbol to input, compatibly with http syntax.
		p = tr->buffer +1;
		tr->initialized = true;
		tr->isSerial = false;
	}
	while (Serial.available()) {
			if ((*p++ = Serial.read()) == '\n') {	//newline stops reading.
				*--p='\0';
				tr->l = p - tr->buffer +1;
				tr->isSerial = true;
				return tr;
		}
	}
	return tr;
}

strBuf *readHeader(EthernetClient client) {
	char c;
	int lim = MAXLINE;
	char *p; p=headerContent;
	while (client.connected()) {
		if (client.available()) {
			if (--lim>0 && (c = client.read()) != EOF) { // reads new chars from ethernet channel
				*p++ = c;
				*p='\0';
				if ( *(p-3) == (c = *(p-1)) && c == '\n') { // a double newline stops reading
					header->buffer = headerContent;
					header->l = p - headerContent;
					return header;
				}
			}
		}
	}
}

paramGet *paramFinder(char *s, int lim, char formato) { //pars a string, looking for substrings of type param=value and store them on a stack
	cmdParser->pointer = stack;
	cmdParser->stack = stack;
	char	c;
	char	*ps;
	char	*pk;
	ps = s;
	while (--lim>0 && (c = *ps++) != '?'); //discharge contents up to "?" symbol
	while (lim>0) {
		pk = cmdParser->pointer->key;
		if (cmdParser->pointer - stack < MAXPARAMS) {
			while (--lim>0 && (c = *ps++) != '=') if (pk -cmdParser->pointer->key < MAXL) *(pk++) = c;
			*pk='\0';
			if (pk > cmdParser->pointer->key) {
				pk=cmdParser->pointer->value;
				while (--lim>0 && (c = *ps++) != '&') if (pk -cmdParser->pointer->value < MAXL) *pk++ = c;
				*pk='\0';
				if (formato == 'I') readInteger(cmdParser->pointer);
			}
			cmdParser->pointer++;
			continue;
		}
		cmdParser->l = cmdParser->pointer - stack -1;
		return cmdParser;
	}
	cmdParser->l = cmdParser->pointer - cmdParser->stack -1;
	return cmdParser;
}

keyFind *cmdFind(paramGet *cmdParser, char key) { //looks for a specific key-name and divide it from the corrispondent key-value.
	cmdParser->pointer = cmdParser->stack -1;
	while (cmdParser->pointer++ - cmdParser->stack < cmdParser->l) {
		if (*cmdParser->pointer->key == key) {
			command->keyName = cmdParser->pointer;
			command->keyFounded = true;
			return command;
		}
	}
	command->keyFounded = false;
	return command;
}
