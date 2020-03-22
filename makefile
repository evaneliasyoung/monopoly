CP = g++
CC = gcc
JV = javac

cpp:
	$(CP) *.cpp -o mono-cpp

c:
	$(CC) *.c -o mono-c

java:
	$(JV) *.java

clean:
	-rm -f mono-*.exe
	-rm -f *.class
