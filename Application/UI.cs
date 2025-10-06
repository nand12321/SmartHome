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

        public String hoveredAnimationState = "";
        Dictionary<string, Texture2D> animationStates = new Dictionary<string, Texture2D>();

        public UI()
        {
            Raylib.InitWindow(800, 600, "Smart Home Control");
            Raylib.SetTargetFPS(60);

            mainLightButton = new Button(560, 365, 50, 50);
            roofLightButton = new Button(560, 230, 50, 50);
            doorButton = new Button(410, 390, 50, 100);

            animationStates["default"] = Raylib.LoadTexture("resources/home_default.png");
            animationStates["mainLight"] = Raylib.LoadTexture("resources/home_main_light.png");
            animationStates["roofLight"] = Raylib.LoadTexture("resources/home_roof_light.png");
            animationStates["door"] = Raylib.LoadTexture("resources/home_door.png");
        }

        public void Run()
        {
            while (!Raylib.WindowShouldClose())
            {
                // Update
                if (mainLightButton.IsHovered())
                {
                    hoveredAnimationState = "mainLight";
                }
                else if (roofLightButton.IsHovered())
                {
                    hoveredAnimationState = "roofLight";
                }
                else if (doorButton.IsHovered())
                {
                    hoveredAnimationState = "door";
                }
                else
                {
                    hoveredAnimationState = "default";
                }

                // Draw
                Raylib.BeginDrawing();
                Raylib.ClearBackground(Color.White);

                // Drawing the home
                switch (hoveredAnimationState)
                {
                    case "mainLight":
                        Raylib.DrawTexture(animationStates["mainLight"], 0, 0, Color.White); 
                        break;
                    case "roofLight":
                        Raylib.DrawTexture(animationStates["roofLight"], 0, 0, Color.White); 
                        break;
                    case "door":
                        Raylib.DrawTexture(animationStates["door"], 0, 0, Color.White); 
                        break;
                    default:
                        Raylib.DrawTexture(animationStates["default"], 0, 0, Color.White);                      
                        break;
                }

                // mainLightButton.Draw();
                // roofLightButton.Draw();
                // doorButton.Draw();
                
                // Drawing sensor data
                Raylib.DrawText(temprature, 100, 330, 20, Color.Gray);
                Raylib.DrawText(humidity, 100, 425, 20, Color.Gray);

                Raylib.EndDrawing();
            }
            Raylib.CloseWindow();
        }
    }
    public class Button
    {
        public Rectangle Rect;
        public Color BgColor = Color.Gray;

        public Button(float x, float y, float width, float height)
        {
            Rect = new Rectangle(x, y, width, height);
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

        public bool IsHovered()
        {
            var mousePos = Raylib.GetMousePosition();
            return Raylib.CheckCollisionPointRec(mousePos, Rect);
        }

        public void Draw()
        {
            Raylib.DrawRectangleRec(Rect, BgColor);
        }
    }
}
