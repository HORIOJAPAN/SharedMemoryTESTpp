#_共有メモリクラス_

###出来ること  

* 共有メモリ内の任意の場所への読み書き  
* 排他処理はちゃんと動くか分かりません

___
###使い方
1.ShareMemory.hをインクルード  
>`#include "SharedMemory.h"`  
 
2.型と共有メモリ名を指定してインスタンス生成  
排他処理する場合はミューテックスオブジェクト名も指定
>`SharedMemory<型> インスタンス名(stirng 共有メモリ名, [string Mutex名]);`  
>>例)int型共有メモリSHMEMNAMEのインスタンスshMemの宣言  
>>`SharedMemory<int> shMem(SHMEMNAME);`  
>>例)float型共有メモリSHMEMNAMEのインスタンスshMemを排他制御ありで宣言  
>>`SharedMemory<float> shMem(SHMEMNAME,MTXNAME);`  

3.getShMemDataメソッドで共有メモリから読み込み  
>`shMem.getShMemData()`

4.setShMemDataメソッドで書き込み  
>`shMem.setShMemData(255);`
___
###今後実装予定の機能
* テンプレ型の判別
* その他もろもろ小回りきくようにしたい

___
