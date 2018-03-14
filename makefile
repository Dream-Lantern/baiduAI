src = $(wildcard ./src/*.cpp)
object = $(patsubst %.cpp, %.o, $(src))
APP = disern

CC = g++
CFLAGS = -Wall -g
CPPFLAGS = -I ./inc/

$(APP):$(object)
	$(CC) -o $@ $^ $(CPPFLAGS) -lcurl -ljson

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY:clean
clean:
	-rm $(object) $(target) $(APP)
