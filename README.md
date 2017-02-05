## 構成
- touchDiplayInterface(MacOS Xcode)
- touchDisplayWIN(Windows Visual Studio)

## 実行ファイル
- MacOS: touchDiplayInterface/bin/touchDisplayInterface.app
- Windows: touchDisplayWIN/bin/touchDisplayWIN.exe

## 仕様
### フローチャート
OSCを受けてスタート，ディスプレイをタップしOSC送信，OSCを受けてループします．
![Flow](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/touchDisplayFlowChart.jpeg)

### スクリーンショット
1. 待機ブランク
![ST_BLWAIT](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/01.png)

2. エリア選択への移行演出
![ST_TOARANIM](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/02.png)

3. エリア選択タップ待機
![ST_ARWAIT](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/03.png)

4. 都市選択への移行演出
![ST_TOCIANIM](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/04.png)

5. 都市選択タップ待機
![ST_CIWAIT](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/05.png)

6. 決定への移行演出
![ST_TODEANIM](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/06.png)

7. 決定タップ待機
![ST_BLWAIT](https://github.com/5c0tt411en/touchDisplayInterface/blob/master/images/07.png)
