all:test

test:test.c unzip.c ioapi.c DocxToXml.c
	gcc -g -Wall -o $@ $^ -lz -std=gnu99

.PHONY:
clean:
	-$(RM) *.o
	-$(RM) test
