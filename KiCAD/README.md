M302-solar-radiation 日射量観測ノード
======================================================

# 概要

Arduino UNOのシールドとして UECS で農業環境計測で使用しやすい
センサ類の接続に特化した基板。


## 初版 (1.00)

# Interface仕様

 ADCはSX8725Cを用いる。  
 single-endでVref=1.22V
 AC2が正、AC3がGND
 D0 => A0
 D1 => A1
 READY => A2


# プログラム

''git@github.com:mhorimoto/M302-solar-radiation.git''

