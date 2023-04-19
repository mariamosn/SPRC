import paho.mqtt.client as mqtt
import time


brokerHost="127.0.0.1"


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

def on_message(client, userdata, msg):
    m = str(msg.payload)
    m = m.decode()
    print(msg.topic+" "+m)

client = mqtt.Client("task2")

print("Conecting to Broker..", brokerHost)
client.connect(brokerHost, 1883, 60)

client.on_connect = on_connect
client.on_message = on_message

client.loop_start()

client.subscribe("sprc/chat/Bogdan")
client.publish("sprc/chat/Bogdan","Hello SPRC!")
time.sleep(2)

client.loop_stop()
client.disconnect()
