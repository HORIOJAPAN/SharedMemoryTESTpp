#_共有メモリクラス_

###出来ること  

* 共有メモリ内の任意の場所への読み書き  

___
###未実装機能
* 排他処理
* テンプレ型の判別
* その他例外処理

___
###使い方
1.ShareMemory.hをインクルード  
>`#include "SharedMemory.h"`  
 
2.型と共有メモリ名を指定してインスタンス生成  
>`SharedMemory<型> インスタンス名(共有メモリ名);`  
>>例)int型共有メモリNAMEのインスタンスshMemの宣言  
>>`SharedMemory<int> shMem(NAME);`  
  
3.getShMemDataメソッドで共有メモリから読み込み  
>`shMem.getShMemData()`

4.setShMemDataメソッドで書き込み  
>`shMem.setShMemData(255);`
___

