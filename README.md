# Convert
## String
### 文字列を Base64 形式に変換する

<img width="500" src="https://github.com/user-attachments/assets/24f2414f-5849-4dca-b86c-b2048ecfca26">

### 文字列を UTF-8（``TArray<uint8>``）に変換する

<img width="500" src="https://github.com/user-attachments/assets/c1e5c668-d318-47ff-aaea-1cbee63f2db1">

## Texture
### TextureRenderTarget2D を Texture2D に変換する

<img width="500" src="https://github.com/user-attachments/assets/b23aea2c-449a-492f-a69b-b81f6e86130d">

### Texture2D を画像ファイル（``TArray<uint8>``）に変換する

<img width="500" src="https://github.com/user-attachments/assets/42523d97-52d8-495b-8a16-2c80fd6c907c">

### 画像ファイル（``TArray<uint8>``）を Texture2D に変換する

<img width="500" src="https://github.com/user-attachments/assets/bd45ab31-32ca-4413-8310-0d615f492793">

# Debug
## ログや警告、エラーをアウトプットログへ表示する
- ``bShowInViewport``：ログをビューポートにも表示するかどうか
- ``DisplayTime``：ログをビューポートにも表示する際に何秒間表示するか
- ``TextColor``：ログをビューポートにも表示する際に何色のテキストで表示するか

<img width="500" src="https://github.com/user-attachments/assets/e725f35c-66a8-4b10-b193-fa81d7e38636">

# Google
## Google Cloud の API を使用する際に必要なアクセストークンを取得する
- 引数
    - ``PrivateKey``：プライベートキー
    - ``ServiceAccountMailAddress``：サービスアカウントのメールアドレス
    - ``Scopes``：認可スコープ（``TArray<FString>``）
        - ``"https://www.googleapis.com/auth/drive"`` や ``"https://www.googleapis.com/auth/spreadsheets"`` 等
- 戻り値
    - ``AccessToken``：アクセストークン
    - ``ExpirationUnixTime``：アクセストークンの有効期限

<img width="500" src="https://github.com/user-attachments/assets/8690758d-0ee4-4f61-9633-f6f932c758af">

# Graphic
## 任意の CameraComponent の視点を Texture2D で取得する

<img width="500" src="https://github.com/user-attachments/assets/a3e5b2ed-93a5-4f66-bab0-1cd8c21140e8">

# HTTP
## URL にクエリパラメータを追加する
この例では ``"https://tsubasamusu.com?Key0=Value0&Key1=Value1"`` という文字列が返って来る。

<img width="500" src="https://github.com/user-attachments/assets/3417f932-4618-4f00-92b7-3344d8d2e1e7">

# Math
## 小数点を任意の位で切り捨てる
- ``DecimalPlace``：小数第何位まで残すか

この例では ``1.2345`` を小数第二位まで保持し、小数第三位以下の値を切り捨てるため ``1.23`` という値が返って来る。

※小数の桁数が増えたりすると正確な値を取得できなくなる可能性がある

<img width="500" src="https://github.com/user-attachments/assets/e7f8583d-e28d-48d9-948b-c205c5f15d7c">

# Slack
## Slack の任意のチャンネルにファイルを送信する
- ``Token``：「xoxb-」から始まるトークン
- ``FileName``：拡張子付きのファイル名
- ``ChannelId``：「C」から始まる、送信先のチャンネルの ID
- ``Message``：添付メッセージ
- ``FileData``：対象のファイル（``TArray<uint8>``）

※ [Slack API](https://api.slack.com/) にてボットの権限に ``files:write`` を追加する必要がある

<img width="500" src="https://github.com/user-attachments/assets/9e366864-63d8-4d58-bad7-8729ee4a8f74">
