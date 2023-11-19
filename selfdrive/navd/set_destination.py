#!/usr/bin/env python3
import json
import sys

from openpilot.common.params import Params

if __name__ == "__main__":
  params = Params()

  # set from google maps url
  if len(sys.argv) > 1:
    coords = sys.argv[1].split("/@")[-1].split("/")[0].split(",")
    dest = {
      "latitude": float(coords[0]),
      "longitude": float(coords[1])
    }
    params.put("NavDestination", json.dumps(dest))
    params.remove("NavDestinationWaypoints")
  else:
    print("Setting to Taco Bell")
    target_lat = 35.680717 #東京駅八重洲口付近緯度
    target_lon = 139.769282 #東京駅八重洲口付近軽度
    dest = {
      # "latitude": 32.71160109904473,
      # "longitude": -117.12556569985693,
      "latitude": target_lat,
      "longitude": target_lon,
    }
    params.put("NavDestination", json.dumps(dest))

    waypoints = [
      #(-117.16020713111648, 32.71997612490662),
      (target_lon,target_lat),
    ]
    params.put("NavDestinationWaypoints", json.dumps(waypoints))

    print(dest)
    print(waypoints)
