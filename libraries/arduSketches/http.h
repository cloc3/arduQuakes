#include <Arduino.h>
#include <Ethernet.h>

struct strBuf {
	char *buffer;
	int l;
	boolean isSerial;
	boolean initialized;
};

#define MAXCHARS 8 // pay attention: arduino memory is small!!!
struct chValPairs {
	char	key[MAXCHARS];
	char	value[MAXCHARS];
	int		nSensorReadings;
};

#define MAXPARAMS 4 // pay attention: arduino memory is small!!!

struct paramGet {
	chValPairs *stack;
	chValPairs *pointer;
	int l;
};

struct keyFind {
	chValPairs *keyName;
	boolean	keyFounded;
};

strBuf *getline(char*, int);
strBuf *readHeader(EthernetClient);
paramGet *paramFinder(char*, int, char);
keyFind *cmdFind(paramGet*, char);
strBuf *serialEvent();
boolean readInteger(chValPairs*);
