import pyrebase
import os
import json


def getconfig():
  Path =os.path.dirname(__file__)
  config=open(Path+'\Config.json')
  data = json.load(config)
  Modulejsonfile=["Config.json"]
  
  # print(Path)
  json_file_names = [filename for filename in os.listdir(Path) if filename.endswith('.json')]
  for file in json_file_names:
      # print(file)
      if file not in Modulejsonfile:
          data["serviceaccount"]=file
          break
  print(data)
  return data



deltefile('as')