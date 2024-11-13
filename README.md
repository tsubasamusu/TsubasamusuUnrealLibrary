# Convert
## String
### 文字列を Base64 に変換する

<img width="500" src="https://github.com/user-attachments/assets/24f2414f-5849-4dca-b86c-b2048ecfca26">

### 文字列を UTF-8（``TArray<uint8>``）に変換する

<img width="500" src="https://github.com/user-attachments/assets/c1e5c668-d318-47ff-aaea-1cbee63f2db1">

## Texture
### TextureRenderTarget2D を Texture2D に変換する

<img width="500" src="https://github.com/user-attachments/assets/820d1511-396a-42be-abfb-1ee70b14461e">

### Texture2D とバイト配列（``TArray<uint8>``）の相互変換を行う

※ ``ConvertByteArrayToTexture()`` で変換後の Texture2D のサイズを指定する引数は変換前の Texture2D のそれと一致させる必要がある

<img width="500" src="https://github.com/user-attachments/assets/972f963b-9ffd-4ecf-b1bc-089d6c0774a5">

### Texture2D を PNG データ（``TArray<uint8>``）に変換する

<img width="500" src="https://github.com/user-attachments/assets/36134ea8-cdef-437e-965c-dec63469340a">

# Debug
## ログや警告、エラーをアウトプットログへ表示する
- ``bShowInViewport``：ログをビューポートにも表示するかどうか
- ``DisplayTime``：ログをビューポートにも表示する際に何秒間表示するか

<img width="500" src="https://github.com/user-attachments/assets/733ef8ee-f549-4765-add9-27e81cb8fb1d">

# Google
## Google Cloud の API を使用する際に必要なアクセストークンを取得する
- 引数
    - ``PrivateKey``：プライベートキー
    - ``ServiceAccountMailAddress``：サービスアカウントのメールアドレス
    - ``Scopes``：認可スコープ（FString 型の配列）
        - ``https://www.googleapis.com/auth/drive`` や ``https://www.googleapis.com/auth/spreadsheets`` 等
- 戻り値
    - ``AccessToken``：アクセストークン
    - ``ExpirationUnixTime``：アクセストークンの有効期限
    - ``bSucceeded``：アクセストークンの取得に成功したかどうか

<img width="500" src="https://github.com/user-attachments/assets/421be06e-7dd8-421c-a46c-02c0275bed6e">

# Graphic
## 任意の CameraComponent の視点の Texture2D を作成する

<img width="500" src="https://github.com/user-attachments/assets/c5f0f410-5269-469e-ba4c-773e5c41d4e7">

# HTTP
## URL にクエリパラメータを追加する
この例では ``"https://hoge?Key0=Value0&Key1=Value1"`` という文字列が返って来る。

<img width="500" src="https://github.com/user-attachments/assets/75cba269-2469-4fde-aa43-1bc26266ccfe">

# Math
## 小数点を任意の位で切り捨てる
この例では ``1.2345`` を小数第二位まで保持し、小数第三位以下の値を切り捨てるため ``1.23`` という値が返って来る。

※小数の桁数が増えたりすると正確な値を取得できない可能性がある

<img width="500" src="https://github.com/user-attachments/assets/e7f8583d-e28d-48d9-948b-c205c5f15d7c">

# Slack
## Slack の任意のチェンネルに任意のファイルを送信する
- ``Token``：「xoxb-」から始まるトークン
- ``FileName``：拡張子付きのファイル名
- ``ChannelID``：「C」から始まる、送信先のチャンネルの ID
- ``Message``：添付メッセージ
- ``FileData``：対象のファイルを ``TArray<uint8>`` に変換した値

※ [Slack API](https://api.slack.com/) にてボットの権限に ``files:write`` を追加する必要がある

<img width="500" src="https://github.com/user-attachments/assets/d6ae57c4-fdea-4a47-9b93-b5420ab6d0b1">
