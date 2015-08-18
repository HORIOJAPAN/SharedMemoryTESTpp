#_共有メモリクラス_

###出来ること  

* 共有メモリへ読み書き  

___
###使い方
1.ShareMemory.hをインクルード  
>`#include "SharedMemory.h"`  
 
2.型と共有メモリ名を指定してインスタンス生成  
>`SharedMemory<型> インスタンス名(共有メモリ名);`  
>例)int型共有メモリNAMEのインスタンスshMemの宣言  
>`SharedMemory<int> shMem(NAME);`  
  
3.getShMemDataメソッドで共有メモリから読み込み  
>`shMem.getShMemData()`

4.setShMemDataメソッドで書き込み  
>`shMem.setShMemData(255);`
___

