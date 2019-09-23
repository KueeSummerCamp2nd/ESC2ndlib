# KUEEサマーキャンプ二回生用ライブラリ

京都大学電気電子工学科サマーキャンプ2回生「ライントレーサー製作」のためのライブラリ。
モータとセンサを簡単に扱うインターフェイスを提供する。

## 前提
- Arduino unoがダウンロードされていること
- Arduino（https://www.arduino.cc/en/Main/Software ）をダウンロードすると、Macでは`/Users/ユーザ名/Documents/`以下に、Windowsでは`¥Users¥ユーザ名¥Documents¥`以下に次のような構成でディレクトリが作成される。これを前提とする。

      Arduino
        └ libraries/
			  └ sketch_xxx/
				└ sketch_xxx.ino

- サマーキャンプで使われる機体を想定する。すなわち接続されるモータやセンサなどのピン配置を前提とする。

## ダウンロード
`libraries`以下に配置する方法
1. `cd Documents/Arduino/libraries `
1. `git clone git@github.com:KUsummercamp2nd/ESC2ndlib.git`

もしくは、プロジェクトファイルと同階層に`ESC2nd.cpp`と`ESC2nd.h`を配置してもいい。この時のディレクトリ構成は次のようになる。その時は、各プロジェクトごとに`ESC2nd.cpp`と`ESC2nd.h`を配置する必要がある。

    Arduino
      └ libraries/
      └ YOUR_PROJECT/
        └ YOUR_PROJECT.ino
        └ ESC2nd.cpp
        └ ESC2nd.h

## 読み込み

- `#include "ESC2nd.h"`で読み込む。

## 使い方
 `Motor`クラスと`Sensor`クラスを提供する。
- ### Motorクラス
  - コンストラクタは`'l','r'`のどちらかを引数にとる。
  - `drive`メソッドを持つ
   - driveメソッドはint型の**-250〜250**の値を受け取り、それに比例した電圧をモーターに与える。正確には、PWM制御のduty比を設定することによって実現される。
   - 正の値を受け取ると前進方向にモータが回転し、負の値を受け取ると後退方向に回転する。
   - 範囲外の値を受け取った時は、-250 or 250にクリップされる。
  - 使用例:
    ```C++
    #include "ESC2nd.h"
    Motor motorR('r');  //右のモーターインスタンスを生成

    void setup(){
      delay(1000);
    }
    void loop(){
      motorR.drive(80); //右のモータが回転する
    }
    ```

- ### Sensorクラス
  - コンストラクタは`DEVICE_ADDR1, DEVICE_ADDR2`のいずれかを引数にとる。これらは二つのAD変換用ICのアドレスを示すマクロであり、ヘッダファイル内で定義されている。
  - `DEVICE_ADDR1`を受け取ると、ライントレース用の8つのフォトリフレクタの値を読み取る。
  - `DEVICE_ADDR2`を受け取ると、バトンパス用赤外線センサ、カラーセンサ、電池電圧の値を読み取る。
  - `read`メソッドを持つ。
    - readメソッドは長さ8の配列の先頭アドレスを受け取り、各要素にセンサーの値を格納する（返り値はない）
    - `DEVICE_ADDR1`のとき、進行方向に向かって左のセンサーが配列の先頭に対応するように順番に値が格納される。
    - `DEVICE_ADDR2`のとき、
      - 0: 進行方向に対して左側の3ピン（赤外線センサ）の値
      - 1-3: カラーセンサーの{green, red, blue}の値にそれぞれ対応する
      - 4: 未定義
      - 5: バッテリー電圧 * 0.5 の値
      - 6: 予備
      - 7: 未定義
  - 使用例
    ```C++
    #include "ESC2nd.h" //ライブラリ読み込み
    Sensor sensor1(DEVICE_ADDR1);  // フォトリフレクタを読むSensorクラスインスタンス
    Sensor sensor2(DEVICE_ADDR2);  // その他のセンサを読むSensorクラスインスタンス
    void setup(){
      delay(1000);  
    }

    void loop(){
      int trace_value[8]; //フォトリフレクタの値を格納するための配列
      int other_value[8]; //その他のセンサの値を格納するための配列

      sensor1.read(trace_value); //各要素にセンサーの値が入る
      sensor2.read(other_value); //各要素にセンサーの値が入る

      for(int i=0;i<8;i++){
        Serial.print(trace_value[i]);  //シリアル通信で（PC側に）送信
        Serial.print(" ");
      }
      or(int i=0;i<8;i++){
        Serial.print(other_value[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
    ```
