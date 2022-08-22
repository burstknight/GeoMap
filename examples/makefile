debug ?= 0

.PHONY: all clean

all:
	cd ./testWebMercator; $(MAKE) debug=$(debug)
	cd ./testMap; $(MAKE) debug=$(debug)

clean:
	cd ./testWebMercator; $(MAKE) clean
	cd ./testMap; $(MAKE) clean
