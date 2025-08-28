from pydantic import BaseModel

class User(BaseModel):
    id: int
    username: str
    password: str

class Home(BaseModel):
    id: int
    temperature: float = 0.0
    humidity: float = 0.0
    roof_lights: bool = False
    main_lights: bool = False
    is_door_open: bool = False