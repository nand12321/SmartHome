from fastapi import FastAPI
from pydantic import BaseModel
import models

app = FastAPI();

users_db = {}
dev = models.User(id=0, username="dev", password="secretpassword")
users_db[dev.id] = dev

homes_db = {}
devs_home = models.Home(
    id=dev.id,
    temperature=0.0,
    humidity=0.0,
    roof_lights=False,
    main_lights=False,
    is_door_open=False
)
homes_db[dev.id] = devs_home

@app.post("/login")
def login(username : str, password : str):
    for user in users_db.values():
        if user.username == username and user.password == password:
            home = homes_db[user.id]
            return {
                    "temperature" : home.temperature,
                    "humidity" : home.humidity,
                    "roof_lights" : home.roof_lights,
                    "main_lights" : home.main_lights,
                    "is_door_open" : home.is_door_open
                    }
        else:
            return {"message" : "username or password is wrong"}
        
@app.post("/home_data")
def get_home_data(home : models.Home):
    homes_db[home.id] = home
    return {"message" : "home data updated"}