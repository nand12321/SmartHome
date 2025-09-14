using MQTTnet;
using MQTTnet.Client;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace AppClient
{
    class Program
    {
        public static async Task Main(string[] args)
        {
            var mqttFactory = new MqttFactory();
            using var mqttClient = mqttFactory.CreateMqttClient();

            var options = new MqttClientOptionsBuilder()
                .WithTcpServer("127.0.0.1", 1883)
                .Build();

            var result = await mqttClient.ConnectAsync(options, CancellationToken.None);
            Console.WriteLine("Connected to broker.");

            var topics = new[] { "smarthome/mainlight", "smarthome/rooflight", "smarthome/door" };

            foreach (var topic in topics)
            {
                var message = new MqttApplicationMessageBuilder()
                    .WithTopic(topic)
                    .WithPayload("ON")
                    .Build();

                await mqttClient.PublishAsync(message, CancellationToken.None);
                Console.WriteLine($"Published to {topic}");
            }

            await mqttClient.DisconnectAsync();
            Console.WriteLine("Disconnected.");
        }
    }
}
