# 機能
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

![image](https://github.com/user-attachments/assets/2035f9e4-96f4-4c7c-8585-cb92d6a764cb)
# 使用方法
1. 「**Releases**」から最新版（**Latest**）の「**Source code**」をダウンロードする
2. ダウンロードした ZIP ファイルを展開して UE プロジェクトのルートディレクトリにある **Plugins** フォルダ内に移動させる（ **Plugins** フォルダが無い場合は自分で作成する）
3. UE エディタを閉じている状態でビルドし、ビルド完了後に UE エディタを開く
4. 必要に応じて「**Edit** ＞ **Plugins** ＞ **PROJECT** ＞ **TSUBASAMUSU**」にある「**TsubasamusuUnrealLibrary**」を有効化する
