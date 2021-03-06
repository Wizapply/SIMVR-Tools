SIMVR-TOOLS (SIMVR SDK, ANTSEAT SDK)
==================

![simvrtools](resources/simvrtools.jpg) 

SIMVR、ANTSEATとは、VR(ヴァーチャルリアリティ)HMDに特化した多軸モーションシミュレータシステムです。ピッチ・ヨー・ロールの3軸回転にヒーブの上下揺れを加えた動きが可能で、様々なアタッチメント用いて、多岐にわたるVRシミュレータ用途に対応します。ここでは、SIMVR用のアプリケーションを開発するためのツール・プログラムがあります。  

ライセンスされたSIMVR用で、SIMVRが同時稼働1台につき1ライセンス必要です。追加ライセンス希望の方は[お問い合わせ先](#お問い合わせ先)までご連絡くださいますようお願い致します。 

また、プログラムに不具合がございまいたらご報告いただければ幸いです。

* SIMVR製品ページ ： <https://simvr.wizapply.com/>
* SIMVR 6DOF製品ページ： <https://simvr6dof.wizapply.com/>
* ANTSEAT製品ページ： <https://antseat.wizapply.com/>

## フォルダ構成
* `/build_c++` : C++(VisualStudio 2015, CMAKE)で開発可能なInclude、DLLファイル群
* `/build_python` : Python2で開発可能なpyファイル群
* `/build_unity5` : Unity5で開発可能なUnityPackageファイル
* `/build_ue4` : 最新UnrealEngine4で開発可能なプラグイン及びプロジェクト群
* `/cad_datas` : SIMVR用の筐体CAD情報
* `/tools` : SIMVR用のツール
* `/demo` : SIMVRの動作を確認するためのサンプルデモゲーム群
* `/driver` : SIMVR USB-CDCドライバ
* `/resources` : GitHub用リソースファイル（開発には必要ありません）

## 必須動作環境
* オペレーティングシステム : Windows 7以降、Linux 4.4.0以降
* インターフェース : USB2.0ポート1つ以上、LAN環境

※ 製品を動作させるためだけの環境であり、開発するVRなどのコンテンツ規模に合う**高性能パソコン**をご用意ください。Windows 10以降の場合、ドライバのインストールは必要ありません。

## 使い方
### 機材の取扱いについてのご説明は[**説明書**](https://github.com/Wizapply/SIMVR-Tools/blob/master/SIMVR_Manual.pdf)をご確認ください。  

Windows 10未満をご利用の場合は、`/driver/wizcdc.inf`ファイルをインストールしてからSIMVRをパソコンに接続してください。  
![WizapplySS002](resources/screenshot_002.png)    

デジタル署名情報が含まれていませんと表示された場合は以下を試してみてください。  
[デジタル署名のないドライバをインストールする方法](http://www.personal-media.co.jp/utronkb/support/install_sig_win8.html)  

### データダウンロード方法
リリースよりダウンロード  
[**リリース最新版をダウンロード**](https://github.com/Wizapply/SIMVR-Tools/releases) 

若しくはGitHub右上ボタンよりクローンかダウンロード  
![WizapplySS001](resources/screenshot_001.png)  

### 実行・セットアップ
実際の使い方・セットアップについては、[**SimVR-SDK Wiki**](https://github.com/Wizapply/SIMVR-Tools/wiki "SimVR-Tools Wiki")にまとめておりますので、そちらをご覧ください。  
[![WizapplySIMVR](resources/howtosimvr.jpg)](https://github.com/Wizapply/SIMVR-Tools/wiki)

## 開発向けライセンスについて
これらのファイルによって開発されたアプリケーションを実機で稼働させるためにライセンス認証が必要です。  
ご購入いただいたお持ちのSIMVRでのみご利用が認められておりますので、別のSIMVRによるビジネスや展示会等でご利用の際は追加でライセンスしてください。  
SIMVR1台にライセンスが付与されますので、所有者・利用者に制限はありません。  

開発しているアプリケーションを認証するには[**SIMVRキーのオンライン発行**](http://simvr01.com/simvrkeygen/)でキーを取得してください。  
※ANTSEATはアプリケーションの認証は必要ありません。  

## エミュレータについて
[![WizapplySS010](resources/screenshot_010.png)](https://github.com/Wizapply/SIMVR-Tools/tree/master/tools/simvr_emulator)   
エミュレータは実機が無くても動作確認ができるシステムです。  
「tools」内の「[**simvr_emulator**](https://github.com/Wizapply/SIMVR-Tools/tree/master/tools/simvr_emulator)」をご確認ください。

## シーケンサについて
[![WizapplySS011](resources/screenshot_011.jpg)](https://github.com/Wizapply/SIMVR-Tools/tree/master/tools/simvr_sequencer)   
シーケンサは動画やタイミングコマンドと連動した動きを付けるためのツール群です。  
ゲームエンジンやプログラミングの難しい知識なしにSIMVRを使用できます。  
「tools」内の「[**simvr_sequencer**](https://github.com/Wizapply/SIMVR-Tools/tree/master/tools/simvr_sequencer)」をご確認ください。


## お問い合わせ先
これらGitHubにアップロードされている全てのファイルについてご質問・ご報告は、下記ご連絡先までご確認ください。  
体感に関するご相談（コンサルティング）・ソフトウェア開発も行っておりますので、下記内容からご連絡くださいませ。  

![Wizapply logo](resources/logo.png)  
WIZAPPLY株式会社  
大阪府大阪市港区市岡元町３－７－１０　KSビル５F  
E-Mail : info@wizapply.com  
TEL : 06-4400-6308  
Web : <https://wizapply.com>  
