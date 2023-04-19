import paho.mqtt.client as mqtt


brokerHost="127.0.0.1"

client = mqtt.Client("demo")

print("Conecting to Broker..", brokerHost)
client.connect(brokerHost, 1883, 60)

#client.loop_start()
#time.sleep(10)

#client.loop_stop()
client.disconnect()
    