maproute: maproute.cpp
	g++ -g maproute.cpp -o maproute
	sudo setcap cap_net_raw+ep maproute
clean:
	rm maproute
	rm *.out
