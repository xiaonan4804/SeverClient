CFLAGS+=-pthread
LDFLAGS+=-pthread


all:Server Client

Server:Server.o ServerFunc.o 
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS)

Client:Client.o ClientFunc.o
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o Server Client *~
