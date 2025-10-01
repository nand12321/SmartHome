using System;
using Raylib_cs;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Application
{
    class Program
    {
        static String serverIP = "10.202.154.159";

        static void Main(string[] args)
        {
            Console.WriteLine("Starting Client...");

            UI ui = new UI();

            _ = Task.Run(() => ExecuteClient(ui));

            ui.Run();
        }

        public static async Task ExecuteClient(UI ui)
        {
            try
            {
                IPAddress ipAddr = IPAddress.Parse(serverIP);
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

                            if (msg.Contains(','))
                            {
                                var parts = msg.Split(',');
                                if (parts.Length >= 2)
                                {
                                    ui.temprature = parts[0];
                                    ui.humidity = parts[1];
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine($"Receive error: {ex.Message}");
                            break;
                        }
                    }
                });


                bool lastMainClick = false;
                bool lastRoofClick = false;
                bool lastDoorClick = false;

                while (true)
                {
                    string input = "";

                    if (ui.mainLightButton.IsClicked() && !lastMainClick)
                    {
                        input = "MAIN_LIGHT";
                    }
                    if (ui.roofLightButton.IsClicked() && !lastRoofClick)
                    {
                        input = "ROOF_LIGHT";
                    }
                    if (ui.doorButton.IsClicked() && !lastDoorClick)
                    {
                        input = "DOOR";
                    }

                    lastMainClick = ui.mainLightButton.IsClicked();
                    lastRoofClick = ui.roofLightButton.IsClicked();
                    lastDoorClick = ui.doorButton.IsClicked();

                    if (!string.IsNullOrWhiteSpace(input))
                    {
                        byte[] messageSent = Encoding.ASCII.GetBytes(input + "\n");
                        await sender.SendAsync(messageSent, SocketFlags.None);
                    }

                    await Task.Delay(10);
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