using System;
using System.Dynamic;
using Raylib_cs;

namespace Application
{
    public class UI
    {
        public Button mainLightButton;
        public Button roofLightButton;
        public Button doorButton;

        public String temprature { get; set; } = "";
        public String humidity { get; set; } = "";

        public UI()
        {
            Raylib.InitWindow(800, 600, "Smart Home Control");
            Raylib.SetTargetFPS(60);

            mainLightButton = new Button(100, 300, 200, 50, "Click Me");
            roofLightButton = new Button(200, 300, 200, 50, "Click Me");
            doorButton = new Button(300, 300, 200, 50, "Click Me");
        }

        public void Run()
        {
            while (!Raylib.WindowShouldClose())
            {
                Raylib.BeginDrawing();
                Raylib.ClearBackground(Color.White);

                mainLightButton.Draw();
                roofLightButton.Draw();
                doorButton.Draw();

                Raylib.DrawText($"Temperature: {temprature}", 50, 50, 20, Color.Black);
                Raylib.DrawText($"Humidity: {humidity}", 50, 80, 20, Color.Black);

                Raylib.EndDrawing();
            }
            Raylib.CloseWindow();
        }
    }

    public class Button
    {
        public Rectangle Rect;
        public string Text;
        public Color BgColor = Color.Gray;
        public Color TextColor = Color.Black;
        public int FontSize = 20;

        public Button(float x, float y, float width, float height, string text)
        {
            Rect = new Rectangle(x, y, width, height);
            Text = text;
        }

        public bool IsClicked()
        {
            if (Raylib.IsMouseButtonPressed(MouseButton.Left))
            {
                var mousePos = Raylib.GetMousePosition();
                return Raylib.CheckCollisionPointRec(mousePos, Rect);
            }
            return false;
        }

        public void Draw()
        {
            Raylib.DrawRectangleRec(Rect, BgColor);
            int textWidth = Raylib.MeasureText(Text, FontSize);
            Raylib.DrawText(
                Text,
                (int)(Rect.X + (Rect.Width - textWidth) / 2),
                (int)(Rect.Y + (Rect.Height - FontSize) / 2),
                FontSize,
                TextColor
            );
        }
    }
}
