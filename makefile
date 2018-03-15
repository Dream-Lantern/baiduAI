# 可执行文件
APP = Face Food Animal Botany
# 工具类目录
COMM_PATH = comm
# 业务类目录
SRC_PATH = src

CC = g++
CFLAGS = -Wall -g
CPPFLAGS = -I ./inc/

# 最终目标
all:$(APP)

Face:$(COMM_PATH)/base64.o \
		$(COMM_PATH)/fileOP.o \
		$(COMM_PATH)/getAccessToken.o \
		$(COMM_PATH)/init.o \
		$(COMM_PATH)/myJson.o \
		$(SRC_PATH)/Face.o \
		$(SRC_PATH)/faceDetect.o
	$(CC) -o $@ $^ -lcurl -ljson

Food:$(COMM_PATH)/base64.o \
		$(COMM_PATH)/fileOP.o \
		$(COMM_PATH)/getAccessToken.o \
		$(COMM_PATH)/init.o \
		$(COMM_PATH)/myJson.o \
		$(SRC_PATH)/Food.o \
		$(SRC_PATH)/foodDetect.o
	$(CC) -o $@ $^ -lcurl -ljson

Animal:$(COMM_PATH)/base64.o \
		$(COMM_PATH)/fileOP.o \
		$(COMM_PATH)/getAccessToken.o \
		$(COMM_PATH)/init.o \
		$(COMM_PATH)/myJson.o \
		$(SRC_PATH)/Animal.o \
		$(SRC_PATH)/animalDetect.o
	$(CC) -o $@ $^ -lcurl -ljson

Botany:$(COMM_PATH)/base64.o \
		$(COMM_PATH)/fileOP.o \
		$(COMM_PATH)/getAccessToken.o \
		$(COMM_PATH)/init.o \
		$(COMM_PATH)/myJson.o \
		$(SRC_PATH)/Botany.o \
		$(SRC_PATH)/botanyDetect.o
	$(CC) -o $@ $^ -lcurl -ljson

# 生成所有.o文件
%.o:%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

# 声明伪文件
.PHONY:clean

# 清楚项目
clean:
	-rm $(APP) $(COMM_PATH)/*.o $(SRC_PATH)/*.o
