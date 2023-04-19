import paho.mqtt.client as mqtt
import time


brokerHost="broker.hivemq.com"


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

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
