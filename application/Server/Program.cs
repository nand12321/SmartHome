using System;
using System.Threading.Tasks;
using MQTTnet;
using MQTTnet.Server;

namespace Server
{
    public class Program
    {
        public static async Task Main()
        {
            var mqttFactory = new MqttFactory();

            var mqttServerOptions = new MqttServerOptionsBuilder()
                .WithDefaultEndpoint()
                .Build();

            var mqttServer = mqttFactory.CreateMqttServer(mqttServerOptions);
            await mqttServer.StartAsync();

            Console.WriteLine("MQTT server running on localhost:1883");
            Console.ReadLine();

            await mqttServer.StopAsync();
        }
    }
}
