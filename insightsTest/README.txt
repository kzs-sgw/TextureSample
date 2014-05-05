
---[ kzsGLObject.h ] の [ ****UserDef**** ] に変数および関数を追加して使用してください。

---OpenGLの描画に用いられるコールバック関数は [ kzsGLObject.cpp ] に定義されているので、適宜書き換えて使用してください。

---[ kzsGLmain.h ] および [ main.cpp ] は基本的にいじらないでください。

---[ main.cpp ] では表示するウィンドウの名前を変更することができます。

---glutMouseWheelFunc() は Mac でうまく動かないようなので除外しています。必要な場合は [ kzsGLObject::runOpenGL() ] を書き換えてください。


---GLEWおよびPhysXの環境が必要。


( 2013/09/25 ) ---最終更新日時