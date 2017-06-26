// ウィンドウ関連の処理
#define USE_OCULUS_RIFT
#include "Window.h"

// 標準ライブラリ
#include <memory>

// 光源
const GgSimpleLight lightData =
{
  { 0.2f, 0.2f, 0.2f, 0.4f },                           // 環境光成分
  { 1.0f, 1.0f, 1.0f, 0.0f },                           // 拡散反射光成分
  { 1.0f, 1.0f, 1.0f, 0.0f },                           // 鏡面光成分
  { 0.0f, 0.0f, 1.0f, 1.0f }                            // 位置
};

// 材質
const GgSimpleMaterial materialData =
{
  { 0.8f, 0.8f, 0.8f, 1.0f },                           // 環境光の反射係数
  { 0.8f, 0.8f, 0.8f, 1.0f },                           // 拡散反射係数
  { 0.2f, 0.2f, 0.2f, 1.0f },                           // 鏡面反射係数
  50.0f                                                 // 輝き係数
};

// 前方面・後方面
const GLfloat zNear(0.1f), zFar(5.0f);

// オブジェクトの位置
const GLfloat origin[] = { 0.0f, 0.0f, -1.0f, 1.0f };

int main()
{
  // ウィンドウを開く
  Window window("HMD Sample", 960, 540);

  // ウィンドウが開けなかったら終わる
  if (!window.get()) return EXIT_FAILURE;

  // 単純な陰影付けを行うシェーダ
  const GgSimpleShader shader("simple.vert", "simple.frag");

  // 表示する図形
  //const std::unique_ptr<const GgElements> obj(ggElementsSphere(0.2f));
  //const std::unique_ptr<const GgTriangles> obj(ggArraysObj("untitled.obj"));
  //const std::unique_ptr<const GgTriangles> obj(ggArraysObj("bunny.obj"));
  //const std::unique_ptr<const GgTriangles> obj(ggArraysObj("HondaS2000.obj"));
  //const std::unique_ptr<const GgElements> obj(ggElementsObj("untitled.obj"));
  //const std::unique_ptr<const GgElements> obj(ggElementsObj("bunny.obj"));
  //const std::unique_ptr<const GgElements> obj(ggElementsObj("HondaS2000.obj"));
  const std::unique_ptr<const GgObj> obj(new GgObj("untitled.obj", shader));
  //const std::unique_ptr<const GgObj> obj(new GgObj("bunny.obj", shader));
  //const std::unique_ptr<const GgObj> obj(new GgObj("HondaS2000.obj", shader));

  // 陰面消去処理を行う
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // 背景色
  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

  // 光源
  GgSimpleLightBuffer light(lightData);

  // 材質
  GgSimpleMaterialBuffer material(materialData);

  // ウィンドウが開いている間繰り返す
  while (!window.shouldClose())
  {
    window.start();

    for (int eye = 0; eye < ovrEye_Count; ++eye)
    {
      GLfloat screen[4], position[4], orientation[4];

      // 描画する目を選択してトラッキング情報を得る
      window.select(eye, screen, position, orientation);

      // 画面を消去する
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // 投影変換行列を求める
      const GgMatrix mp(ggFrustum(screen[0] * zNear, screen[1] * zNear, screen[2] * zNear, screen[3] * zNear, zNear, zFar));

      // 視野変換行列を求める
      const GgMatrix mv(GgQuaternion(orientation).getMatrix() * ggTranslate(position) * ggTranslate(origin));

      // モデル変換行列はトラックボール処理とする
      const GgMatrix mm(window.getLeftTrackball());

      // 描画用のシェーダプログラムの使用開始
      shader.use();
      shader.selectLight(light);
      shader.selectMaterial(material);
      shader.loadMatrix(mp, mv * mm);

      // 図形を描画する
      obj->draw();

      // 片目の描画を完了する
      window.commit(eye);
    }

    // ダブルバッファリング
    window.swapBuffers();
  }
}
