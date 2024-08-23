# 機能
## Convert
### TextureRenderTarget2D を Texture2D に変換する
※変換対象の TextureRenderTarget2D のフォーマットは ``RTF RGBA8``（``EPixelFormat::PF_B8G8R8A8``）である必要がある

![image](https://github.com/user-attachments/assets/820d1511-396a-42be-abfb-1ee70b14461e)
### Texture2D とバイト配列（``TArray<uint8>``）の相互変換を行う
※変換対象の Texture2D のフォーマットは ``RTF RGBA8``（``EPixelFormat::PF_B8G8R8A8``）である必要がある

※ ``ConvertByteArrayToTexture()`` で変換後の Texture2D のサイズを指定する引数は変換前の Texture2D のそれと一致させる必要がある

![image](https://github.com/user-attachments/assets/505b49a2-0b50-4e57-8a28-f96ae1bbdccf)
### Texture2D を PNG データ（``TArray<uint8>``）に変換する
※変換対象の Texture2D のフォーマットは ``RTF RGBA8``（``EPixelFormat::PF_B8G8R8A8``）である必要がある

![image](https://github.com/user-attachments/assets/36134ea8-cdef-437e-965c-dec63469340a)
## Debug
### ログや警告、エラーをアウトプットログへ表示する
- ``bShowInViewport``：ログをビューポートにも表示するかどうか
- ``DisplayTime``：ログをビューポートにも表示する際に何秒間表示するか

![image](https://github.com/user-attachments/assets/733ef8ee-f549-4765-add9-27e81cb8fb1d)
## HTTP
### URL にクエリパラメータを追加する
この例では ``"https://hoge?Key0=Value0&Key1=Value1"`` という文字列が返って来る。

![image](https://github.com/user-attachments/assets/75cba269-2469-4fde-aa43-1bc26266ccfe)
## Math
### 小数点を任意の位で切り捨てる
この例では ``1.2345`` を小数第二位まで保持し、小数第三位以下の値を切り捨てるため ``1.23`` という値が返って来る。

※小数の桁数が増えたりすると正確な値を取得できない可能性がある

![image](https://github.com/user-attachments/assets/e7f8583d-e28d-48d9-948b-c205c5f15d7c)
# 使用方法
1. 「**Releases**」から最新版（**Latest**）の「**Source code**」をダウンロードする
2. ダウンロードした ZIP ファイルを展開して UE プロジェクトのルートディレクトリにある **Plugins** フォルダ内に移動させる（ **Plugins** フォルダが無い場合は自分で作成する）
3. UE エディタを閉じている状態でビルドし、ビルド完了後に UE エディタを開く
4. 必要に応じて「**Edit** ＞ **Plugins** ＞ **PROJECT** ＞ **TSUBASAMUSU**」にある「**TsubasamusuUnrealLibrary**」を有効化する
