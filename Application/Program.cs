using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Application
{
    class Client
    {
        static async Task Main(string[] args)
        {
            await ExecuteClient();
        }

        static async Task ExecuteClient()
        {
            try
            {
                IPAddress ipAddr = IPAddress.Parse("10.125.43.159");
                IPEndPoint esp32EndPoint = new IPEndPoint(ipAddr, 80);

                using Socket sender = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
                await sender.ConnectAsync(esp32EndPoint);

                Console.WriteLine($"Socket connected to -> {sender.RemoteEndPoint}");

                _ = Task.Run(async () =>
                {
                    var buffer = new byte[1024];
                    while (true)
                    {
                        try
                        {
                            int byteRecv = await sender.ReceiveAsync(buffer, SocketFlags.None);
                            if (byteRecv == 0) break;
                            string msg = Encoding.ASCII.GetString(buffer, 0, byteRecv);
                            Console.WriteLine($"[Server] {msg}");
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine($"Receive error: {ex.Message}");
                            break;
                        }
                    }
                });


                while (true)
                {
                    string input = Console.ReadLine();
                    if (string.IsNullOrWhiteSpace(input)) continue;
                    if (input.ToLower() == "exit") break;

                    byte[] messageSent = Encoding.ASCII.GetBytes(input);
                    await sender.SendAsync(messageSent, SocketFlags.None);
                }

                sender.Shutdown(SocketShutdown.Both);
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error: {e}");
            }
        }
    }
}
