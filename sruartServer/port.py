import serial.tools.list_ports
import random
import time
import  sys
from  Adafruit_IO import  MQTTClient

AIO_FEED_ID = "TKLL_humi"
AIO_USERNAME = "Chanhcei"
AIO_KEY = "aio_JDLU43oLcjdrqwRV0wLJuQF0jYx0"

def  connected(client):
    print("Ket noi thanh cong...")
    client.subscribe(AIO_FEED_ID)

def  subscribe(client , userdata , mid , granted_qos):
    print("Subscribe thanh cong...")

def  disconnected(client):
    print("Ngat ket noi...")
    sys.exit (1)

def  message(client , feed_id , payload):
    if feed_id == AIO_FEED_ID:
        print("Nhan du lieu: " + payload)
        ser.write((AIO_FEED_ID + ":"+ str(payload) + "#").encode())

def getPort():
    ports = serial.tools.list_ports.comports()
    N = len(ports)
    commPort = "None"
    for i in range(0, N):
        port = ports[i]
        strPort = str(port)
        if "ELTIMA Virtual Serial Port" in strPort:
            splitPort = strPort.split(" ")
            commPort = (splitPort[0])
    return commPort

ser = serial.Serial( port=getPort(), baudrate=115200)

client = MQTTClient(AIO_USERNAME , AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message
client.on_subscribe = subscribe
client.connect()
client.loop_background()

while True:
    value = random.randint(0, 100)
    print("Cap nhat:", value)
    client.publish("TKLL_humi", value)
    time.sleep(100)