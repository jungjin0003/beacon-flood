#Makefile
all: beacon-flood

beacon-flood:
					g++ main.cpp beacon-flood.cpp -o beacon-flood -lpcap

clean:
		rm -f beacon-flood