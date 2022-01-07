import serial.tools.list_ports
import random
import time
import  sys
from  Adafruit_IO import  MQTTClient

AIO_FEED_ID = "TKLL_COD"
AIO_FEED_ID1 = "TKLL_SS"
AIO_FEED_ID2 = "TKLL_NO3"
AIO_FEED_ID3 = "TKLL_pH"
AIO_FEED_ID4 = "TKLL_NH4"
AIO_FEED_ID5 = "TKLL_RL1"
AIO_FEED_ID6 = "TKLL_RL2"
AIO_FEED_ID7 = "TKLL_RLM"
manual = 0
rl1 = 0
rl2 = 0

AIO_USERNAME = "chanh"
AIO_KEY = "aio_nEth829iua4qknod0HA0crX4NjIA"

def  connected(client):
    print("Ket noi thanh cong...")
    client.subscribe(AIO_FEED_ID)
    client.subscribe(AIO_FEED_ID1)
    client.subscribe(AIO_FEED_ID2)
    client.subscribe(AIO_FEED_ID3)
    client.subscribe(AIO_FEED_ID4)
    client.subscribe(AIO_FEED_ID5)
    client.subscribe(AIO_FEED_ID6)
    client.subscribe(AIO_FEED_ID7)


def  subscribe(client , userdata , mid , granted_qos):
    print("Subscribe thanh cong...")

def  disconnected(client):
    print("Ngat ket noi...")
    sys.exit (1)

def  message(client , feed_id , payload):
    global  manual
    global rl1
    global rl2
    if feed_id == AIO_FEED_ID:
        print("Nhan du lieu COD: " + payload)
        # ser.write((AIO_FEED_ID + ":"+ str(payload) + "m").encode())
    if feed_id == AIO_FEED_ID1:
        print("Nhan du lieu SS: " + payload)
    if feed_id == AIO_FEED_ID2:
        print("Nhan du lieu NO3: " + payload)
    if feed_id == AIO_FEED_ID3:
        print("Nhan du lieu pH: " + payload)
    if feed_id == AIO_FEED_ID4:
        print("Nhan du lieu NH4: " + payload)
    if feed_id == AIO_FEED_ID5:
        print("Nhan du lieu RL1: " + payload)
        if payload == "0" and manual == 1 :
            rl1 = 0
            if rl2 == 0:
                ser.write(("d").encode())
            else:
                ser.write(("b").encode())
        if payload == "1" and manual == 1 :
            rl1 = 1
            if rl2 == 0:
                ser.write(("c").encode())
            else:
                ser.write(("a").encode())
    if feed_id == AIO_FEED_ID6:
        print("Nhan du lieu RL2: " + payload)
        if payload == "0" and manual == 1:
            rl2 = 0
            if rl1 == 0:
                ser.write(("d").encode())
            else:
                ser.write(("c").encode())
        if payload == "1" and manual == 1:
            rl2 = 1
            if rl1 == 0:
                ser.write(("b").encode())
            else:
                ser.write(("a").encode())
    if feed_id == AIO_FEED_ID7:
        print("Nhan du lieu RLM: " + payload)
        if payload == "0":
            manual = 0
            ser.write((AIO_FEED_ID7 + ":"+ str(payload) + "m").encode())
        if payload == "1":
            manual = 1
def getPort():
    ports = serial.tools.list_ports.comports()
    N = len(ports)
    commPort = "None"
    for i in range(0, N):
        port = ports[i]
        strPort = str(port)
        if "USB-SERIAL CH340" in strPort:
            splitPort = strPort.split(" ")
            commPort = (splitPort[0])
    return commPort
def readSerial():
    bytesToRead = ser.inWaiting()
    if (bytesToRead > 0):
        global mess
        mess = mess + ser.read(bytesToRead).decode("UTF-8")
        while ("#" in mess) and ("!" in mess):
            start = mess.find("!")
            end = mess.find("#")
            processData(mess[start:end + 1])
            if (end == len(mess)):
                mess = ""
            else:
                mess = mess[end + 1:]
mess = ""
def processData(data):
    data = data.replace("!", "")
    data = data.replace("#", "")
    print(data)
    client.publish("TKLL_COD", data[3:5])
    time.sleep(1)
    client.publish("TKLL_SS",  data[9:11])
    time.sleep(1)
    client.publish("TKLL_NH4", data[15:17])
    time.sleep(1)
    client.publish("TKLL_NO3", data[21:23])
    time.sleep(1)
    client.publish("TKLL_pH",  data[27:29])
    time.sleep(1)
    client.publish("TKLL_RL1", data[34:35])
    time.sleep(1)
    client.publish("TKLL_RL2", data[40:41])
ser = serial.Serial( port=getPort(), baudrate=9600)
mess = ""
client = MQTTClient(AIO_USERNAME , AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message
client.on_subscribe = subscribe
client.connect()
client.loop_background()

while True:
    readSerial()

