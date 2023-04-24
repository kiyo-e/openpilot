#!/usr/bin/env python3
from abc import ABC, abstractmethod
import sqlite3
import datetime

from common.realtime import DT_TRML
from common.numpy_fast import interp
from system.swaglog import cloudlog
from selfdrive.controls.lib.pid import PIDController

class BaseFanController(ABC):
  @abstractmethod
  def update(self, max_cpu_temp: float, ignition: bool) -> int:
    pass


class TiciFanController(BaseFanController):
  def __init__(self) -> None:
    super().__init__()
    cloudlog.info("Setting up TICI fan handler")

    self.last_ignition = False
    self.controller = PIDController(k_p=0, k_i=4e-3, k_f=1, neg_limit=-80, pos_limit=0, rate=(1 / DT_TRML))

    #ここが2Hzだから、limitspeed.db操作に利用する。
    self.db_path = "../../../limitspeed.db" #例によって遅くないか？

    # テーブルを作成するSQL
    create_table_sql = """
    CREATE TABLE speeds (
        id INTEGER PRIMARY KEY,
        latitude REAL,
        longitude REAL,
        bearing REAL,
        velocity REAL,
        timestamp REAL
    );
    """

    # データベースに接続してカーソルを取得
    self.conn = sqlite3.connect(self.db_path)
    self.cur = self.conn.cursor()
    #self.date = datetime.datetime.now()

    # テーブルが存在しない場合にのみ作成する
    table_exists_query = "SELECT name FROM sqlite_master WHERE type='table' AND name='speeds'"
    result = self.cur.execute(table_exists_query)
    table_exists = bool(result.fetchone())

    if not table_exists:
      # テーブルを作成
      self.cur.execute(create_table_sql)
      # データベースに反映
      self.conn.commit()

    # 削除条件となる日時を計算
    delete_date = datetime.datetime.now().timestamp() - 30*24*3600 #30日前
    # テーブルから削除する
    self.cur.execute("DELETE FROM speeds WHERE timestamp < ?", (delete_date,))
    # 変更を保存,月単位の削除なら起動時に一回で十分。
    self.conn.commit()

    self.latitude = 0
    self.longitude = 0
    self.bearing = 0
    self.velocity = 0
    self.timestamp = 0
    self.get_limit_avg = 0
    self.get_limitspeed_old = 0
    self.velo_ave_ct_old = 0
    self.db_add = 0
    self.db_none = 0
    self.min_distance_old = 0
    
    # # カーソルと接続を閉じる
    # self.cur.close()
    # self.conn.close()

  def __del__(self):
    # カーソルと接続を閉じる
    self.cur.close()
    self.conn.close()
    pass
  
  def update(self, max_cpu_temp: float, ignition: bool) -> int:
    self.controller.neg_limit = -(80 if ignition else 30)
    self.controller.pos_limit = -(30 if ignition else 0)

    if ignition != self.last_ignition:
      self.controller.reset()

    error = 70 - max_cpu_temp
    fan_pwr_out = -int(self.controller.update(
                      error=error,
                      feedforward=interp(max_cpu_temp, [60.0, 100.0], [0, -80])
                    ))

    self.last_ignition = ignition

    #"/tmp/limitspeed_info.txt"からlatitude, longitude, bearing, velocity,timestampを読み出して速度30km/h以上ならspeedsに挿入する
    limitspeed_info_ok = False
    limitspeed_min = 1 #30,debug
    try:
      with open('/tmp/limitspeed_info.txt','r') as fp:
        limitspeed_info_str = fp.read()
        if limitspeed_info_str:
          limitspeed_info_ok = True
          #pythonを用い、カンマで区切られた文字列を分離して変数a,b,cに格納するプログラムを書いてください。
          #ただしa,b,cはdouble型とします
          self.latitude, self.longitude, self.bearing, self.velocity,self.timestamp = map(float, limitspeed_info_str.split(","))
          limit_m = 0 #self.velocity/3.6
          # if limit_m < 10:
          #   limit_m = 10 #10m以内の範囲には登録しない。
          if self.velocity >= limitspeed_min #and self.get_limit_avg * 0.9 < self.velocity and ((int(self.get_limit_avg/5) * 5) != int(self.velocity/5) * 5 or self.get_limitspeed_old == 0 or ((self.min_distance_old**0.5) * 100 / 0.0009 > limit_m and self.velo_ave_ct_old < 10)):
            # データを挿入するSQL , self.velocityが平均速度と同等であれば登録しない。もしくは平均より10km/h遅くても登録しない。
            insert_data_sql = """
            INSERT INTO speeds (latitude, longitude, bearing, velocity,timestamp)
            VALUES (?, ?, ?, ?, ?);
            """
            self.cur.execute(insert_data_sql,(self.latitude, self.longitude, self.bearing, self.velocity,self.timestamp))
            #print("緯度: {}, 経度: {}, 方位: {}, 速度: {}, 日時: {}".format(self.latitude, self.longitude, self.bearing, self.velocity,self.timestamp))
            # 変更を保存
            self.conn.commit()
            self.db_add += 1
          else:
            self.db_none += 1
    except Exception as e:
      pass
    #speedsから距離と方位が近いデータを100個読み、100m以内で速度の上位20パーセントの平均を計算する。int(それ/10)*10を現在道路の制限速度と見做す。
    get_limitspeed = 0
    if limitspeed_info_ok or (self.latitude != 0 or self.longitude != 0):
      #self.latitude, self.longitude, self.bearing, self.velocity,self.timestamp
      query = '''
          SELECT *
          FROM (SELECT * , ABS(bearing - ?) AS abs_bear FROM speeds)
          WHERE
              CASE
                  WHEN abs_bear > 180 THEN 360 - abs_bear
              ELSE
                  abs_bear
              END <= 10
              OR
              CASE
                  WHEN abs_bear > 180 THEN 360 - abs_bear
              ELSE
                  abs_bear
              END >= 170
          ORDER BY ((latitude - ?) * (latitude - ?) + (longitude - ?) * (longitude - ?))
          LIMIT 100
      '''

      # クエリを実行し、結果を取得
      self.cur.execute(query, (self.bearing, self.latitude, self.latitude, self.longitude, self.longitude , ))
      if limitspeed_info_ok == False: #limitspeed_info_ok == False(ファイル読み込み失敗)のフォローは一度きり
        self.latitude = 0
        self.longitude = 0

      # データ内容を検査して走行速度を推定する。
      earth_ang = 0.0009 #大体200m四方
      earth_ang *= interp(self.velocity, [0, 50.0], [0.3, 1.0])#検出範囲に速度を反映する。０〜50km/h -> 0.3〜1倍
      rows = []
      velo_max = 0
      velo_max_ct = 0
      for row in self.cur: #一度ループさせると消える。
        rows.append(row) #取っておく
        row_id , latitude, longitude, bearing, velocity,timestamp , abs_bear = row #サブクエリ使うとabs_bearがくっついてしまう
        velo_max_ct += 1
        if velo_max < velocity and abs(latitude-self.latitude) < earth_ang and abs(longitude-self.longitude) < earth_ang:
          velo_max = velocity

      if velo_max > 0:
        velo_80 = (velo_max - limitspeed_min) * 0.8 + limitspeed_min
        velo_ave = 0
        velo_ave_ct = 0
        self.min_distance_old = 0
        for row in rows: #rowsは何度でも使える。
          row_id , latitude, longitude, bearing, velocity,timestamp , abs_bear = row #サブクエリ使うとabs_bearがくっついてしまう
          if velo_80 <= velocity and abs(latitude-self.latitude) < earth_ang and abs(longitude-self.longitude) < earth_ang:
            velo_ave_ct += 1
            velo_ave += velocity
            distance = (latitude-self.latitude) **2 + (longitude-self.longitude) **2
            if distance != 0 and (distance < self.min_distance_old or self.min_distance_old == 0):
              self.min_distance_old = distance #最も近い距離のポイント

        self.velo_ave_ct_old = velo_ave_ct
        if velo_ave_ct > 0:
          velo_ave /= velo_ave_ct
          get_limitspeed = velo_ave
          self.get_limit_avg = get_limitspeed
          #ここでもしself.get_limit_avgとself.velocityが近ければ、rows内のrow_idの中から近傍の値を消すと最適化として具合いいかも。

    #制限速度があれば"/tmp/limitspeed_data.txt"へ数値で書き込む。なければ"/tmp/limitspeed_data.txt"を消す。
    self.get_limitspeed_old = get_limitspeed
    if get_limitspeed > 0:
      with open('/tmp/limitspeed_data.txt','w') as fp:
        fp.write('%d,%.2f,999,%.1fm,+%d' % (int(get_limitspeed/10) * 10 , get_limitspeed , (self.min_distance_old**0.5) * 100 / 0.0009 , self.db_add))
    else:
      with open('/tmp/limitspeed_data.txt','w') as fp:
        fp.write('%d,%.2f,111,%.1fm,-%d' % (int(self.get_limit_avg/10) * 10 , self.get_limit_avg , (self.min_distance_old**0.5) * 100 / 0.0009 , self.db_none))

    # # もしここで削除するなら、近傍の古いデータだけにするとか、単純な月単位よりも細かく制御したい。
    # # 変更を保存
    # self.conn.commit()

    return fan_pwr_out

